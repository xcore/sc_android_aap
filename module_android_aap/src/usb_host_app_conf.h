// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

#ifndef __USB_HOST_APP_CONF_H__
#define __USB_HOST_APP_CONF_H__
/*------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Extern Variables
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Manifest constants
 ------------------------------------------------------------------------------*/
/* Number of CTRL chanends used. This is defined by the number of interfaces that are in your descriptor */
#define NUM_CTRL_CHANENDS                 1

/* set the configuration to start with - useful if we know which configuration we want */
#define START_CONFIG_NUM                  0

/* maximum number of timed EP's we have (e.g. Isoc / Interrupt EPs) */
#define TIMED_EP_COUNT                    0

/* Initial get descriptor length to try, will retry if length is longer */
#define INITIAL_GET_DEV_DESC_LEN          18

/* Initial get descriptor length to try, will retry if length is longer */
#define DEFAULT_EP0_PIPE_WIDTH            64

/* Initial get configuration descriptor length to try, will retry if length is longer */
#define INITIAL_GET_CONF_DESC_LEN         32

/* Pause to re-introduce accessory - 1ms step - 10ms */
#define RECONNECT_PAUSE                   10

/* enable hook for VID/PID match failure */
#define USBHOST_VID_PID_HOOK_ON
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

#ifdef USBHOST_VID_PID_HOOK_ON
   #ifndef __XC__
      int USBHost_user_vid_pid_fail( DeviceDescriptor *devicedesc,
                                     int vendorIDMask,
                                     int vendorID,
                                     int productIDMask,
                                     int productID );
   #endif
#endif

#endif // __USB_HOST_APP_CONF_H__
/*============================================================================*/
