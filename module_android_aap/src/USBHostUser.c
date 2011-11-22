// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------*/
#include "usb_descriptors.h"
#include "USBHostLLD.h" 
#include "usb_host_global.h"
#include "usb_host_app_conf.h"

/*------------------------------------------------------------------------------
 Extern Variables
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Manifest constants
 ------------------------------------------------------------------------------*/
#define ACC_GET_PROTOCOL   51 //33
#define ACC_SEND_STRING    52 //34
#define ACC_MODE_START     53 //35
#define ACC_SET_CONFIG     1

/*------------------------------------------------------------------------------
 Typedefs
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Global Variables
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Static Variables
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Prototypes
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Implementation
 -----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Name: sendString
 Purpose:
 Input:
 Output:
 Notes: sends accessory introduction string
 -----------------------------------------------------------------------------*/
int sendString(int addr, int index, char *str, int len)
{
   UsbTransfer *usbTrans;
   int retVal;

   USBLLD_ControlTransferOut(addr, BRT_TYPE_VENDOR, BRT_RECIPIENT_DEVICE,
                             ACC_SEND_STRING, 0, index, len, str);

   retVal = WaitForResponse(&usbTrans);
   USBLLD_readfinished();
   return retVal;
}

/*------------------------------------------------------------------------------
 Name: USBHost_user_vid_pid_fail
 Purpose:
 Input:
 Output:
 Notes: Device mismatch. Could be that the device is not in the 'special'
 accssory mode. Attempt to start in accessory mode.
 -----------------------------------------------------------------------------*/
int USBHost_user_vid_pid_fail(DeviceDescriptor *devicedesc,
                              int vendorIDMask,
                              int vendorID,
                              int productIDMask,
                              int productID)
{
   char protocol;
   int rv = 0;
   UsbTransfer *usbTrans;

   // Get protocol
   USBLLD_ControlTransferIn(0, BRT_TYPE_VENDOR, BRT_RECIPIENT_DEVICE,
                            ACC_GET_PROTOCOL, 0, 0, 2, DEFAULT_EP0_PIPE_WIDTH);

   if (WaitForResponse(&usbTrans))
   {
      USBLLD_readfinished();
      return 1;
   }

   protocol = (int) (usbTrans->data[0]);
   USBLLD_readfinished();

   if (protocol >= 1)
   {
      // Introduce accessory
      rv |= sendString(0, 0, "Google, Inc.", 12);
      rv |= sendString(0, 1, "DemoKit", 7);
      rv |= sendString(0, 2, "XMOS", 4);
      rv |= sendString(0, 3, "1.0", 3);
      rv |= sendString(0, 4, "http://www.android.com", 22);
      rv |= sendString(0, 5, "0000000012345678", 16);

      if (rv)
      {
         return 2; // error sending a string
      }

      // Put in Acc mode & restart
      USBLLD_ControlTransferInDir(0, BRT_TYPE_VENDOR, BRT_RECIPIENT_DEVICE,
                                  ACC_MODE_START, 0, 0, 0, 0,
                                  DEFAULT_EP0_PIPE_WIDTH);

      if (WaitForResponse(&usbTrans))
      {
         USBLLD_readfinished();
         return 3; // failure to go into acc mode
      }
      USBLLD_readfinished();

      // Return non-zero so that we restart enumeration
      return 4;
   }
   else
   {
      // protocol does not match. No accessory support available
      return 4;
   }
}

/*============================================================================*/
