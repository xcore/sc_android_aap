// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

#ifndef __AAP_IF_H__
#define __AAP_IF_H__
/*------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Extern Variables
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Manifest constants
 ------------------------------------------------------------------------------*/

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
/**
 * Hook for EP Manager to handle requests from the application layer
 */
#ifdef __XC__
   int android_ctrl( streaming chanend c, int reqVal, int timeNow);
#else
   int android_ctrl(chanend c, int reqVal, int timeNow);
#endif

/**
 * Hook for EP manager to handle data coming in
 */
#ifdef __XC__
   int android_rx(streaming chanend c,
                  unsigned char data[],
                  int datalen,
                  int ackstate,
                  int timedTrans);
#else
   int android_rx(chanend c,
                  unsigned char data[],
                  int datalen,
                  int ackstate,
                  int timedTrans);
#endif

#endif // __AAP_IF_H__
/*============================================================================*/
