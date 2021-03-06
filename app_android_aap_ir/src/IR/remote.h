// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

#ifndef _REMOTE_H_
#define _REMOTE_H_
/*------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------*/
#include "remote_cmds.h"

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
// Prototype functions
int ir_receive(in port pIR, int bitoffset, timer t);

#endif // _REMOTE_H_

/*============================================================================*/
