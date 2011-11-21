/*
 * USB Host Endpoint Manager
 * Talks to USB LLD via buffer manager
 */

#include <xccompat.h>
#include "chanabs.h"
#include "usb_descriptors.h"
#include "epManager.h"
#include "UsbHostLLD.h"
#include "string.h"
#include "freqConst.h"
#include <xs1.h>
#include "xc_support.h"

#include "device.h"
#include "usb_host_global.h"
#include "usb_host_app_conf.h"

int deviceAddress = 1;
DeviceDescriptor devicedesc;

int dummyrx(streaming_chanend c, unsigned char data[], int datalen, int ackstate) {return 0;}
int dummyfn(unsigned char data[], int datalen) {return 0;}
int dummyctrl(streaming_chanend c, int v, int timenow) {return 0;}
int dummytimeout(streaming_chanend c, int timenow) {return 0;}
int dummyflush(streaming_chanend c) {return 0; }
void dummyinit(void) {return;}

volatile int deviceConnected;

t_timed_transaction timed_trans[TIMED_EP_COUNT];
 
void setConnected(int x)
{
    deviceConnected = x;
    /* deactivate timed transactions */
    if (x != 1)
    {
        for (int i = 0; i < TIMED_EP_COUNT; i++)
            timed_trans[i].active = 0;
    }
}

void set_timed_ep_active( unsigned v )
{
    for (int i = 0; i < TIMED_EP_COUNT; i++)
    {
        if (timed_trans[i].trans_type == USB_TRANSTYPE_ISO)
            timed_trans[i].active = v;
    }
}

/*
 * WaitForResponse:
 * Wait for a USB Transfer to complete
 */
__attribute__((noinline)) int WaitForResponse(UsbTransfer **usbTrans)
{
  char sofstate;

  while (1)
  {
    if (!USBLLD_removesof((char *)&sofstate))
    {
      if (sofstate == 3)
      {
        setConnected(0);
        return 2;
      }
    }

    if (!USBLLD_getreadhandle(usbTrans))
    {
      if ((*usbTrans)->ackState == USB_TRANS_ACKSTATE_ACK)
      {
        return 0;
      }
      else if ((*usbTrans)->ackState == USB_TRANS_ACKSTATE_IGNORE)
      {
        setConnected(0);
        return 1;
      }
      else
      {
        return 1;
      }
    }
  }

  return 0;
}

 /*
  * USBHostStackParseDeviceDescriptor:
  * Check the device Descriptor matches the expected values for the device.
  * Returns 0 on success, 1 for error.
  */
int USBHostStackParseDeviceDescriptor(UsbTransfer *usbTrans, int *numDeviceConfigs)
{
    memcpy((char *)&devicedesc, usbTrans->data, usbTrans->dataLen);
    
    if (((devicedesc.idVendor & vendorIDMask) != vendorID) || ((devicedesc.idProduct & productIDMask) != productID))
    {
        return 1;
    }
    else
    {
        *numDeviceConfigs = devicedesc.bNumConfigurations;
        return 0;
    }
}

/*
 * USBHostStackParseConfigurationDescriptor:
 * Parse the device configuration descriptor.
 * Check for the expected number of device interfaces and endpoints.
 * nDeviceInterfaces is defined externally as it is device specific.
 */
int USBHostStackParseConfigurationDescriptor(UsbTransfer *usbTrans, int time, int configNum)
{
  int i;
  char *usbTransPtr;
  int currentIF = -1;
  int dataLen;
  char descriptorData[256];
  int configSet = 0;
  ConfigDescriptor *configdesc;
  InterfaceDescriptor *ifdesc;
  EndpointDescriptor *epdesc;

  dataLen = usbTrans->received_dataLen;
  memcpy(descriptorData, (char *)(usbTrans->data), dataLen);
  usbTransPtr = (char *)descriptorData;
  USBLLD_readfinished();

  // Wipe all IFs and EPs found flags
  for (i = 0; i < nDeviceInterfaces; i++)
  {
    deviceInterfaces[i].interfaceFound = 0;
  }
  for (i = 0; i < nDeviceEPs; i++)
  {
    deviceEPs[i].epfound = 0;
  }

  while (dataLen > 0)
  {
    
    // Check for zero length
    if (usbTransPtr[0] == 0)
    {
      vprint("Error on config parse");
    }

    // Switch on descriptor type
    switch (usbTransPtr[1])
    {

      case (0x2):

        // Configuration Descriptor
        configdesc = (ConfigDescriptor *)usbTransPtr;

        // Sanity check number of interfaces
        if (configdesc->bNumInterfaces != nDeviceInterfaces)
        {
          return 1;
        }
        break;

      case (0x4):

        // Interface Descriptor
        ifdesc = (InterfaceDescriptor *)usbTransPtr;
        currentIF = -1;

        // Seek to match interface with device.h
        for (i = 0; i < nDeviceInterfaces; i++)
        {
          if (ifdesc->bInterfaceClass == (deviceInterfaces[i].interfaceClass & 0xFF) &&
              ifdesc->bInterfaceSubClass == (deviceInterfaces[i].interfaceSubClass & 0xFF) &&
              ifdesc->bInterfaceProtocol == (deviceInterfaces[i].interfaceProtocol & 0xFF) &&
              ifdesc->bNumEndpoints == deviceInterfaces[i].numEndpoints &&
              deviceInterfaces[i].interfaceFound == 0)
          {
            deviceInterfaces[i].interfaceFound = 1;
            deviceInterfaces[i].interfaceNumber = ifdesc->bInterfaceNumber;
            currentIF = i;
            break;
          }
        }

        break;

      case (0x5):

        // Endpoint Descriptor
        epdesc = (EndpointDescriptor *)usbTransPtr;

        // Seek to match interface with device.h
        for (i = 0; i < nDeviceEPs; i++)
        {
          if (currentIF == deviceEPs[i].interfaceNumber &&
              epdesc->bmAttributes ==
                (deviceEPs[i].transferType |
                 (deviceEPs[i].syncType << 2) |
                 (deviceEPs[i].usageType << 4)) &&
              (epdesc->bEndpointAddress >> 7) == deviceEPs[i].dir)
          {
            deviceEPs[i].epfound = 1;
            deviceEPs[i].EPnum = epdesc->bEndpointAddress & 0xF;
            deviceEPs[i].maxPacketSize = epdesc->wMaxPacketSize;
            deviceEPs[i].interval = epdesc->bInterval; // mS frames
            deviceEPs[i].nextInterruptPacketTime = epdesc->bInterval;
            break;
          }
        }
        if (i == nDeviceEPs)
        {
          // Ignore unknown EP
        }

        break;

      case (0x21):
      case (0x24):
      case (0x25):
        // Class specific descriptor
        // Pass to class specific descriptor processor

        if (currentIF != -1)
        {
          if (!configSet)
          {
            {
              // Set configuration
              USBLLD_ControlTransferInDir(deviceAddress, BRT_TYPE_STANDARD, BRT_RECIPIENT_DEVICE, BREQ_SET_CONFIGURATION, configNum+1, 0, 0, /* null data */ 0, 64);
              if (WaitForResponse(&usbTrans))
              {
                USBLLD_readfinished();
                return 7;
              }
              USBLLD_readfinished();
              // 2mS pause to allow set configuration and for device to settle down
              pause(2000 * refFreqMHz);
            }
            configSet = 1;
          }
          (*deviceInterfaces[currentIF].parseDescriptor)(usbTransPtr, usbTransPtr[0]);
        }

        break;
      default:
        break;
    }

    // Now skip
    dataLen -= usbTransPtr[0];
    usbTransPtr += usbTransPtr[0];
  }

  // Check now that all IFs and EPs were found
  for (i = 0; i < nDeviceInterfaces; i++)
  {
    if (!deviceInterfaces[i].interfaceFound)
    {
      return 1;
    }
  }
  for (i = 0; i < nDeviceEPs; i++)
  {
    if (!deviceEPs[i].epfound)
    {
      return 1;
    }
  }

  if (!configSet)
  {
    {
      // Set configuration
      USBLLD_ControlTransferInDir(deviceAddress, BRT_TYPE_STANDARD, BRT_RECIPIENT_DEVICE, BREQ_SET_CONFIGURATION, configNum+1, 0, 0, /* null data */ 0, 64);
      if (WaitForResponse(&usbTrans))
      {
        USBLLD_readfinished();
        return 7;
      }
      USBLLD_readfinished();
      // 2mS pause to allow set configuration and for device to settle down
      pause(2000 * refFreqMHz);
    }
    configSet = 1;
  }
  return 0;
}

int cGlobalClients[NUM_CTRL_CHANENDS];
/*
 * usb_buf_pull:
 * Checks for returned packets from LLD. This includes SOFs as
 * well as responses to other requests.
 * Returns 0 on packet detected and 1 on packet not detected
 */
int usb_buf_pull(int *transType, unsigned timed_ep_count, int *ignoreCount)
{
    int i;
    char sofstate;
    int retval = 1;
    UsbTransfer *usbTrans;
    
    *transType = 0;
    // Check for SOFs
    if (!USBLLD_removesof((char *)&sofstate))
    {
        if (sofstate == 3)
        {
            setConnected(0);
        }
        else
        {
            int time = getTime();
            
            // Look for timeouts
            for (i = 0; i < nDeviceInterfaces; i++)
            {
                (*deviceInterfaces[i].timeout)(cGlobalClients[i], time);
            }
        }
    }

  
    /* this is timed trans stuff */
    while (!epManTimed_getReadHandle(&usbTrans) && deviceConnected)
    {
        if ((usbTrans)->transferComplete)
        {
            if ((usbTrans)->ackState == USB_TRANS_ACKSTATE_IGNORE)
            {
                *ignoreCount += 1;
                if (*ignoreCount >= 2) 
                    setConnected(0);
            }
            else
            {
                // Check address matches the expected device address
                if ((usbTrans)->address == deviceAddress)
                {
                    int i;
                    
                    // Look up EP
                    for (i = 0; i < nDeviceEPs; i++)
                    {
                        if ((usbTrans)->endpoint == deviceEPs[i].EPnum)
                            break;
                    }
                    
                    if (i < nDeviceEPs)
                    {
                        (*deviceEPs[i].rx)(cGlobalClients[deviceEPs[i].interfaceNumber], (char*)(usbTrans)->data, (usbTrans)->dataLen, (usbTrans)->ackState, 1);
                    }
                }
            }
            epManTimed_readFinished( );
        } else break; // exit loop if we hit an incomplete packet
    }
                
    
    
    // Check rx buffer to see if any transfers have completed
    while (!USBLLD_getreadhandle(&usbTrans) && deviceConnected)
    {
        if ((usbTrans)->transferComplete)
        {
            if ((usbTrans)->ackState == USB_TRANS_ACKSTATE_IGNORE)
            {
                setConnected(0);
            }
            else
            {
                // Check address matches the expected device address and it is a completed transfer
                if ((usbTrans)->address == deviceAddress)
                {
                    int i;
                
                    if ((usbTrans)->transferType == USB_TRANSTYPE_CONTROL)
                    {
                        *transType |= 1;
                        USBLLD_readfinished();
                    }
                    else
                    {
                        // Look up EP
                        for (i = 0; i < nDeviceEPs; i++)
                        {
                            if ((usbTrans)->endpoint == deviceEPs[i].EPnum)
                                break;
                        }
                    
                        // make sure we are dealing with a correct EP and an IN packet
                        if (i < nDeviceEPs && usbTrans->direction == BRT_DIRECTION_FROM_DEV) 
                        {
                            // non-timed data
                            (*deviceEPs[i].rx)(cGlobalClients[deviceEPs[i].interfaceNumber],    (char*)(usbTrans)->data, (usbTrans)->dataLen, (usbTrans)->ackState, 0);
                        }
                        else
                        {
                            USBLLD_readfinished();
                        }
                    }
                }
            }
            retval = 0;
        } else break; // exit loop if we hit an incomplete packet
    }

  return retval;
}

/*
 * USBHostStack_main:
 * This function is the mid-level of the USB stack. It tries to enumerate
 * a newly connected device, or enter a wait state on device disconnect.
 * In error cases it will do a bus reset and reconnect.
 *
 * Once enumeration has successfully completed, this layer stays in a loop
 * accepting requests from the audio layer and passing them down to the LLD.
 * Disconnect of the device is also detected and resets both this level and the LLD.
 *
 */
// Warning: This value may change and should be tested
#pragma stackfunction 1000
int USBHostStack_main(streaming_chanend cClients[NUM_CTRL_CHANENDS], chanend cUSBCtrl, int disconnectflag[NUM_CTRL_CHANENDS])
{
    int now, i;
    int configNum;
    UsbTransfer *usbTrans;
    int retlen;
    int numDeviceConfigs;
    int pipeWidth = DEFAULT_EP0_PIPE_WIDTH;
    unsigned timed_ep_count = 0;
    int ignoreCount = 0;
    
    for (i = 0; i  < NUM_CTRL_CHANENDS; i++)
        cGlobalClients[i] = cClients[i];
    
    deviceConnected = 1;
    
    // Get Device Descriptor
    USBLLD_ControlTransferIn(0, BRT_TYPE_STANDARD, BRT_RECIPIENT_DEVICE, BREQ_GET_DESCRIPTOR, DESC_TYPE_DEVICE << 8, 0, INITIAL_GET_DEV_DESC_LEN, pipeWidth );  
    if (WaitForResponse(&usbTrans))
    {
        USBLLD_readfinished();
        return 2;
    }
    
    /* get maximum packet length for this pipe */
    pipeWidth = ((usbTrans->data[1]>>24) & 0xFF);
    
    /* Check the length of device descriptor and get it again if we didn't get all of it */
    if ((usbTrans->data[0] & 0xFF) > INITIAL_GET_DEV_DESC_LEN)
    {
        /* length of the packet */
        retlen = (usbTrans->data[0] & 0xFF);
        USBLLD_readfinished();
        USBLLD_ControlTransferIn(deviceAddress, BRT_TYPE_STANDARD, BRT_RECIPIENT_DEVICE, BREQ_GET_DESCRIPTOR, DESC_TYPE_DEVICE << 8, 0, retlen, pipeWidth);
        if (WaitForResponse(&usbTrans))
        {
            USBLLD_readfinished();
            return 2;
        }
    }
    
    /* parse descriptor */
    if (USBHostStackParseDeviceDescriptor(usbTrans, &numDeviceConfigs))
    {
        // If the descriptor doesn't match the expected, call hook if defined otherwise reset and retry.
        USBLLD_readfinished();
        #ifdef USBHOST_VID_PID_HOOK_ON
        return USBHost_user_vid_pid_fail( &devicedesc, vendorIDMask, vendorID, productIDMask, productID );
        #else
        return 4;
        #endif
    }
    USBLLD_readfinished();
    
    /* now do a device reset */
    //USB_Ctrl(cUSBCtrl, USB_CMD_DEVICE_RESET);
    
    /* let the device get a grip (200ms) */
    pause(200000 * refFreqMHz);
    
    // Set device address - this is unacknowledged
    USBLLD_ControlTransferInDir(0, BRT_TYPE_STANDARD, BRT_RECIPIENT_DEVICE, BREQ_SET_ADDRESS, deviceAddress, 0, 0, /* null data */ 0, pipeWidth);
    if (WaitForResponse(&usbTrans))
    {
        USBLLD_readfinished();
        return 1;
    }
    USBLLD_readfinished();
    
    // wait 2ms to allow Set Address (USB Spec section 9.2.6.3)
    pause(2000 * refFreqMHz);
    
    configNum = START_CONFIG_NUM;
    while ( configNum < numDeviceConfigs )
    {
        for (i = 0; i < nDeviceInterfaces; i++)
        {
            deviceInterfaces[i].interfaceFound = 0;
            deviceInterfaces[i].alternateSetting &= 1;
        }
        for (i = 0; i < nDeviceEPs; i++)
        {
            deviceEPs[i].epfound = 0;
        }
        
        // Get configuration descriptor
        USBLLD_ControlTransferIn(deviceAddress, BRT_TYPE_STANDARD, BRT_RECIPIENT_DEVICE, BREQ_GET_DESCRIPTOR, (DESC_TYPE_CONFIGURATION << 8) | configNum, 0, INITIAL_GET_CONF_DESC_LEN, pipeWidth);
        if (WaitForResponse(&usbTrans))
        {
            USBLLD_readfinished();
            return 5;
        }
        
        /* check the length, if it is bigger than the initial length we first requested then request 
        * the full size
        */
        if ((usbTrans->data[0] >> 16) > INITIAL_GET_CONF_DESC_LEN) 
        {
            retlen = (usbTrans->data[0] >> 16);    
            USBLLD_readfinished();
            // Get config descriptor
            USBLLD_ControlTransferIn(deviceAddress, BRT_TYPE_STANDARD, BRT_RECIPIENT_DEVICE, BREQ_GET_DESCRIPTOR, (DESC_TYPE_CONFIGURATION << 8) | configNum, 0, retlen, pipeWidth);
            if (WaitForResponse(&usbTrans))
            {   
                USBLLD_readfinished();
                return 6;
            }
        }
        
        now = getTime();
        
        if (!USBHostStackParseConfigurationDescriptor(usbTrans, now, configNum))
        {
            break;
        }
        
        configNum++;
    }
    
    if (configNum==numDeviceConfigs)
    {
        vprint("No configurations matched");
        return 7;
    }
    
    // 200mS pause to allow set configuration and for device to settle down
    pause(200000 * refFreqMHz);
    
    // Run the initialisation functions for the clients
    for (i = 0; i < nDeviceInterfaces; i++)
    {
        (*deviceInterfaces[i].init)();
    }
    
    /* setup timed interval transactions */
    for (i = 0; i < nDeviceEPs; i++)
    {
        if (deviceEPs[i].transferType == EPTransferType_Iso || deviceEPs[i].transferType == EPTransferType_Interrupt )
        {
        
            if (timed_ep_count > TIMED_EP_COUNT)
            {
                return 11;
            }
            else
            {
                timed_trans[timed_ep_count].dev_addr = deviceAddress;
                timed_trans[timed_ep_count].EP_addr = deviceEPs[i].EPnum;
                timed_trans[timed_ep_count].trans_type = deviceEPs[i].transferType;
                timed_trans[timed_ep_count].direction = deviceEPs[i].dir;
                timed_trans[timed_ep_count].bInterval = deviceEPs[i].interval;
                timed_trans[timed_ep_count].count = 0;
                if (timed_trans[timed_ep_count].trans_type == USB_TRANSTYPE_INTERRUPT)
                    timed_trans[timed_ep_count].active = 1;
                else
                    timed_trans[timed_ep_count].active = 0;
                
                timed_ep_count++;
            }
        }
    }
    
    // send the pointer and array length to the LLD to setup the periodic EP's
    init_timed_eps_chan(cUSBCtrl, timed_ep_count );
    
    // Main usb loop. Poll the control channels for requests, and pass to the LLD.
    // If the device disconnects, quit.
    while (deviceConnected)
    {
        // Check control channels
        int ctrlchan;
        int val;
        int transType;
        
        pollChans(cClients, &ctrlchan, &val);
        
        
        if (ctrlchan != -1)
        {
            (*deviceInterfaces[ctrlchan].ctrlRequest)(cClients[ctrlchan], val, getTime());
        }
        
        usb_buf_pull(&transType, timed_ep_count, &ignoreCount);
        
    }
    
    // tell upper layers we have had a disconnection
    endChans(cClients, disconnectflag);
    
    // Flush the buffers out so we don't send anything on reconnect
    for (i = 0; i < nDeviceInterfaces; i++)
    {
        (*deviceInterfaces[i].flush)(cGlobalClients[i]);
    }
    
    return 12;
}

/*
* USBHostStack
* Provides EndPoint management and client channels
* Wraps USBHostStack_main allowing it to exit on error and restart
* the device.
*/
void USBHostStack(streaming_chanend cClients[NUM_CTRL_CHANENDS], chanend cUSBCtrl)
{
    UsbTransfer *usbTrans;
    
    int disconnectflag[NUM_CTRL_CHANENDS];
    
    USB_Ctrl(cUSBCtrl, 0x1); // wait on wakeup from LLD
    
    // Receive info on which clients require disconnect notification
    // This also sends a wake-up to all connected channels.
    startChans(cClients, disconnectflag);
    
    while (1)
    {
        int status;
        
        // Run enumeration
        status = USBHostStack_main(cClients, cUSBCtrl, disconnectflag);
        
        // Disconnection event - this will put the bus in reset, blocks until buffer is cleared
        USB_Ctrl(cUSBCtrl, USB_CMD_DRAIN_BUFFER);
        
        // Drain our buffer from any old transactions that have appeared meantime
        while(!USBLLD_getreadhandle(&usbTrans))
        {
            USBLLD_readfinished();
        }
        
        // pause - this enforces the gap between unplug and replug, bus is in reset
        #ifdef RECONNECT_PAUSE
        pause(RECONNECT_PAUSE * 1000 * refFreqMHz);
        #endif
        
        // reconnection - pulls bus out of reset and does the device detection (blocking)
        USB_Ctrl(cUSBCtrl, USB_CMD_WAIT_FOR_DEVICE);
    }
}
