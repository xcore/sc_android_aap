// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*------------------------------------------------------------------------------
 Include files
 -----------------------------------------------------------------------------*/
#include <platform.h>
#include <xs1.h>
#include <stdio.h>

#include "aap_2v0.h"
#include "UsbHostLLD.h"
#include "epManager.h"
#include "remote.h"

/*------------------------------------------------------------------------------
 Extern Variables
 -----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Manifest constants
 -----------------------------------------------------------------------------*/
// Create reference constants
const int sysFreqMHz = DEF_SYS_FREQ_MHz;
const int refFreqMHz = DEF_REF_FREQ_MHz;
const int refClkDiv = DEF_REF_CLK_DIV;

#define IPOD_IR_REPEAT_INTERVAL (25000 * refFreqMHz)

/*------------------------------------------------------------------------------
 Ports and Clocks
 -----------------------------------------------------------------------------*/
// InfraRed Receiver Port
in port pIR = PORT_IR;
out port pGPIO = XS1_PORT_4C;

/*------------------------------------------------------------------------------
 Typedefs
 -----------------------------------------------------------------------------*/
/* USB ports */
/* USB ports */
struct_usbports s_usbports =
{  PORT_USB_RCV,
   PORT_USB_SE0_DETECT,
   PORT_USB_VO,
   PORT_USB_FSE0,
   PORT_USB_OEn,
   #ifndef PORT_USB_VBUS
     XS1_PORT_16B,
   #else
      PORT_USB_VBUS,
   #endif
   USB_CLKBLK_0,
   USB_CLKBLK_1,
   PORT_USB_SOF_TIMER };
/*------------------------------------------------------------------------------
 Global Variables
 -----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Static Variables
 -----------------------------------------------------------------------------*/
static unsigned int rpp = 0;
static unsigned int rvu = 0;
static unsigned int rvd = 0;
static unsigned int rtn = 0;

/*------------------------------------------------------------------------------
 Prototypes
 -----------------------------------------------------------------------------*/
static void application_thread(streaming chanend cCtrl[NUM_CTRL_CHANENDS],
                               streaming chanend cSOFGen,
                               streaming chanend cVBus,
                               out port pGPIO);

static void vBusCheck(streaming chanend ?cVBus, out port pGPIO);
/*------------------------------------------------------------------------------
 Implementation
 -----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Name: main
 Purpose:
 Input:
 Output:
 Notes:
 -----------------------------------------------------------------------------*/
int main(void)
{
   chan cUSBCtrl;
   streaming chan cUSBSOF;
   streaming chan cCtrl[NUM_CTRL_CHANENDS];
   streaming chan cVBus;

   par
   {
      usbHostLLD(s_usbports, cVBus, cUSBCtrl, cUSBSOF, SE0_DETECT_BITSHIFT);
      application_thread(cCtrl, cUSBSOF, cVBus, pGPIO);
      USBHostStackWrapper(cCtrl, cUSBCtrl);
   }
   return 0;
}

/*------------------------------------------------------------------------------
 Name: application_thread
 Purpose:
 Input:
 Output:
 Notes:
 -----------------------------------------------------------------------------*/
static void application_thread(streaming chanend cCtrl[NUM_CTRL_CHANENDS],
                               streaming chanend cSOFGen,
                               streaming chanend cVBus,
                               out port pGPIO)
{
   timer tIrRepeat;
   timer tIR;
   unsigned irRepeatTs;
   int newIrVal;
   int rtnval;

   int pkt[3];
   int tmp_cmd;
   int tmp_val;

   timer tSOF;
   int now;
   tSOF :> now;

   /* request disconnection notification from USB Manager */
   cCtrl[0] <: 1;
   cCtrl[0] :> int _;

   /* set USB Host to use internal SOF timing */
   cSOFGen <: 0;
   cSOFGen :> int _; // get acknowledgement

   tIrRepeat :> irRepeatTs;
   irRepeatTs += IPOD_IR_REPEAT_INTERVAL;

   pkt[0] = 0x01;
   while(1)
   {
      vBusCheck(cVBus, pGPIO);
      select
      {
         /* IR command handler */
         case tIrRepeat when timerafter(irRepeatTs) :> irRepeatTs:

         // increment to next TS to check for repeat
         irRepeatTs += IPOD_IR_REPEAT_INTERVAL;

         tmp_cmd = -1; // pkt[1]
         tmp_val = 0; // pkt[2]

         // get current IR value
         newIrVal = ir_receive(pIR, IR_PORT_BIT, tIR);
         if (newIrVal < 0)
         {
            newIrVal = remoteNoPress;
         }
         if (newIrVal != remoteNoPress)
         {
            switch(newIrVal)
            {
               case remotePlayPause:
               tmp_cmd = 0;
               rpp = ~rpp & 1;
               tmp_val = rpp;
               break;

               case remoteVolumeUp:
               tmp_cmd = 1;
               rvu = ~rvu & 1;
               tmp_val = rvu;
               break;

               case remoteVolumeDown:
               tmp_cmd = 2;
               rvd = ~rvd & 1;
               tmp_val = rvd;
               break;

               case remoteTrackNext:
               tmp_cmd = 4;
               rtn = ~rtn & 1;
               tmp_val = rtn;
               break;

               default: break;
            }
            if(tmp_cmd >= 0)
            {
               pkt[1] = tmp_cmd;
               pkt[2] = tmp_val;

               // send it across the channel
               // First three steps are polling req
               cCtrl[0] <: 0;
               cCtrl[0] :> int _;
               cCtrl[0] <: 1;
               // Data goes here
               cCtrl[0] <: pkt[0];
               cCtrl[0] <: pkt[1];
               cCtrl[0] <: pkt[2];
            }
         }
         break;
         default: break;
      }
   }
}

/*------------------------------------------------------------------------------
 Name: vBusCheck
 Purpose:
 Input:
 Output:
 Notes:
 -----------------------------------------------------------------------------*/
static void vBusCheck(streaming chanend ?cVBus, out port pGPIO)
{
   int portstate = 0;
   if (!isnull(cVBus))
   {
      select
      {
         case cVBus :> int val:
            #if (USB_VBUS_INV == 1)
               val = (!val); /* invert value */
            #endif
            portstate &= ~(1 << VBUS_BITSHIFT);
            portstate |= (val & 1) << VBUS_BITSHIFT;
            cVBus <: 1;
            break;
         default: break;
      }
   }
   pGPIO <: portstate;
}

/*============================================================================*/
