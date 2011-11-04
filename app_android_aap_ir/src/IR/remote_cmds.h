/*==============================================================================
Filename:
Project :
Author  :
Version :
Purpose
--------------------------------------------------------------------------------

Version History
--------------------------------------------------------------------------------

License
--------------------------------------------------------------------------------
The copyrights, all other intellectual and industrial property rights are
retained by XMOS and/or its licensors.
Terms and conditions covering the use of this code can be found in the Xmos
End User License Agreement.

Copyright XMOS Ltd 2011

In the case where this code is a modification of existing code under a
separate license, the separate license terms are shown below. The
modifications to the code are still covered by the copyright notice above.
==============================================================================*/
#ifndef REMOTE_CMDS_H_
#define REMOTE_CMDS_H_
/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Extern Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Manifest constants
------------------------------------------------------------------------------*/
// Values taken from Simple Lingo Context Button Status command
#define PLAY_PAUSE  0x00000001
#define VOL_UP      0x00000002
#define VOL_DOWN    0x00000004
#define NEXT_TRACK  0x00000008
#define LAST_TRACK  0x00000010
#define MENU        0x00400000
#define BEGIN_FF    0x00100000
#define BEGIN_RW    0x00200000
#define END_FFRW    0x00000100
#define PAUSE       0x00000200
#define POWER_OFF   0x00040000
#define SELECT      0x00800000
#define SHUFFLE     0x00008000
#define REPEAT      0x00010000
#define UP_ARROW    0x01000000
#define DOWN_ARROW  0x02000000
#define POWER_ON    0x00020000
#define VIDEO       0xffffffff

/*------------------------------------------------------------------------------
Typedefs
------------------------------------------------------------------------------*/
// Define the commands for the iPod
static const int nButtons = 19;
typedef enum
{
   remoteNoPress = 0,
   remotePlayPause = 1,
   remoteVolumeUp = 2,
   remoteVolumeDown = 3,
   remoteTrackNext = 4,
   remoteTrackPrev = 5,
   remoteMenu = 6,
   remoteBeginFastForward = 7,
   remoteBeginRewind = 8,
   remoteEndFFRew = 9,
   remotePowerOff = 10,
   remoteMenuUp = 11,
   remoteMenuDown = 12,
   remoteSelect = 13,
   remoteShuffle = 14,
   remoteVideo = 15,
   remoteRepeat = 16,
   remotePowerOn = 17,
   remotePause = 18,
} e_remotecommands;

/*------------------------------------------------------------------------------
Global Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Static Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Prototypes
------------------------------------------------------------------------------*/
extern const int simpleContextButtonValues[];
extern const int extendedButtonValues[];

#endif /*REMOTE_CMDS_H_*/
/*============================================================================*/
