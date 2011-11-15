// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

/*------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------*/
#include <xs1.h>
#include "remote_cmds.h"
#include "remote.h"

/*------------------------------------------------------------------------------
 Extern Variables
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Manifest constants
 ------------------------------------------------------------------------------*/
/*
 * Define the times for the different pulses
 *
 * The only one that ought to be tuned is ERROR_LEN; increase it to
 * increase resilience against bad environments, at the cost of increasing
 * more fantom remote control presses. decrease it in order to decrease
 * the number of fantom remote control presses, at the expense of less
 * resilience against environments with high efficient lights.
 */
#define ERROR_LEN               50000           // timing inaccuracy            = 0.50ms
#define START_LEN               900000          // start pulse                  = 9 ms
#define START_PAUSE             450000          // start pause                  = 4.5 ms
#define REPEAT_PAUSE            225000          // repeat pause                 = 2.25 ms
#define REPEAT_PERIOD_TIMEOUT   15000000        // timeout of repeat pulses     = 150 ms
#define ZERO_LEN                112000          // length of entire pulse for 0 = 1.12 ms
#define ONE_LEN                 225000          // length of entire pulse for 1 = 2.25 ms
const int simpleContextButtonValues[] =
{ 0x0,
  PLAY_PAUSE,
  VOL_UP,
  VOL_DOWN,
  NEXT_TRACK,
  LAST_TRACK,
  MENU,
  BEGIN_FF,
  BEGIN_RW,
  END_FFRW,
  POWER_OFF,
  UP_ARROW,
  DOWN_ARROW,
  SELECT,
  SHUFFLE,
  VIDEO,
  REPEAT,
  POWER_ON,
  PAUSE };

const int extendedButtonValues[] =
{ 0x00, 0x01, 0x00, 0x00, 0x03, 0x04, 0x00, 0x05, 0x06, 0x07, 0x00, };

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

/*------------------------------------------------------------------------------
 Name: waitForPort
 Purpose:
 Input:
 Output:
 Notes:
 ------------------------------------------------------------------------------*/
int waitForPort(in port p, int bit, int targetval)
{
   for (int i = 0; i < 4; i++)
   {
      int portval;
      int retrycount = 1 << 20;
      do
      {
         p :> portval;
         portval = (portval >> bit) & 1;
         retrycount--;
      } while (targetval != portval && retrycount);

      if (retrycount == 0)
      {
         return 1;
      }
   }
   return 0;
}

/*------------------------------------------------------------------------------
 Name: ir_receive
 Purpose:
 Input:
 Output:
 Notes:
 ------------------------------------------------------------------------------*/
int ir_receive(in port pIR, int bitoffset, timer t)
{
   unsigned ts, ts1, tsHalf, val, pulseLength;

   while (1)
   {
      val = 0xAA;
      t :> ts;
      while(1)
      {
         ts1 = ts;

         if (waitForPort(pIR, bitoffset, 1))
         {
            return -3;
         }
         t :> tsHalf;

         if (waitForPort(pIR, bitoffset, 0))
         {
            return -2;
         }

         t :> ts;
         pulseLength = ts - ts1;

         if ((pulseLength > (ZERO_LEN - ERROR_LEN)) &&
               (pulseLength < (ZERO_LEN + ERROR_LEN)) )
         {
            val <<= 1; // It's a zero.
         }
         else if ((pulseLength > (ONE_LEN - ERROR_LEN)) &&
               (pulseLength < (ONE_LEN + ERROR_LEN)) )
         {
            val = val << 1 | 1; // It's a one.
         }
         else
         {
            if ((pulseLength > (START_LEN+REPEAT_PAUSE - ERROR_LEN)) &&
                  (pulseLength < (START_LEN+REPEAT_PAUSE + ERROR_LEN)) &&
                  (ts-tsHalf > (REPEAT_PAUSE - ERROR_LEN)) &&
                  (ts-tsHalf < (REPEAT_PAUSE + ERROR_LEN)))
            {
               return -1;
            }

            val = 0xAA;
         }

         if ( ( val & 0xFFFF0000 ) == 0x77E10000 )
         {
            int remotecmd = (val >> 8) & 0xFF;

            switch (remotecmd)
            {
               case 0x20:
               case 0xA0:
               return remotePlayPause;
               case 0x10:
               case 0x90:
               return remoteTrackPrev;
               case 0xE0:
               case 0x60:
               return remoteTrackNext;
               case 0xD0:
               case 0x50:
               return remoteVolumeUp;
               case 0xB0:
               case 0x30:
               return remoteVolumeDown;
               case 0x40:
               return remoteMenu;
            }
         }
         else if ((val >> 16) == 0xFF && ((val ^ (val >>8)) & 0xFF) == 0xFF)
         {
            // NEC Protocol
            if ( ( val & 0x000FF00 ) == 0x0008800 )
            {
               return remotePlayPause;
            }
            else if (( val & 0x000FF00 ) == 0x0000000 )
            {
               return remoteVolumeUp;
            }
            else if ( ( val & 0x000FF00 ) == 0x0001000 )
            {
               return remoteVolumeDown;
            }
            else if ( ( val & 0x000FF00 ) == 0x0004000 )
            {
               return remoteTrackNext;
            }
            else if ( ( val & 0x000FF00 ) == 0x0005000 )
            {
               return remoteTrackPrev;
            }
         }
      } // while(1)
   } // while(1)
}

/*============================================================================*/
