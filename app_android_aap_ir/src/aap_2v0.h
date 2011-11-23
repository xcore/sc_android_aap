// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

#ifndef AAP_2V0_H_
#define AAP_2V0_H_
/*------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Extern Variables
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Manifest constants
 ------------------------------------------------------------------------------*/
/* configuration defines */
#define LR_CLK_1B_PORT
#define USB_PORTS_2V0

#define USB_CLKBLK_0             XS1_CLKBLK_5
#define USB_CLKBLK_1             XS1_CLKBLK_2

/* set to 0 to disable, 1 to enable VBus inversion (active low) */
#define USB_VBUS_INV             1

#define SE0_DETECT_BITSHIFT      2
#define COPRORESET_BITSHIFT      0
#define OSC_BITSHIFT             1
#define LED_BITSHIFT             2
#define VBUS_BITSHIFT            3

/* set to 0 to disable, set to 1 to enable pull down on IR port */
#define IR_PULLDOWN              1
#define IR_PORT_BIT              0

#define AUDIO_CLKBLK_0           XS1_CLKBLK_3
#define AUDIO_CLKBLK_1           XS1_CLKBLK_4

#define MAX_CHARGING_CURRENT     2100
#define USB_CORE                 0
#define DEV_SPEED_C5
#define DEF_SYS_FREQ_MHz         480
#define DEF_REF_FREQ_MHz         96
#define DEF_REF_CLK_DIV          5
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
 ------------------------------------------------------------------------------*/

#endif // AAP_2V0_H_
/*============================================================================*/
