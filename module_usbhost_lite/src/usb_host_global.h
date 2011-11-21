#ifndef _USB_HOST_GLOBAL_H_
#define _USB_HOST_GLOBAL_H_

#define USE_STREAMING_CHANNELS

#include <xs1.h>

// -------------------------------------------------------------------------------------------------
#include "print.h"
#ifdef VERBOSE
#define vprintstr(a)   printstr(a)
#define vprinthexln(a) printhexln(a)
#define vprintstrln(a) printstrln(a)
#define vprintintln(a) printintln(a)
#else
#define vprintstr(a)
#define vprinthexln(a)
#define vprintstrln(a)
#define vprintintln(a)
#endif

#define vprint(a) { vprintstrln(a); return 1;}
#define vprinti(a) { vprintintln(a); return 1; }

#ifdef __XC__
#define streaming_chanend streaming chanend
#else
#define streaming_chanend unsigned
#endif

// -------------------------------------------------------------------------------------------------
#define USB_DISCONNECT_NOTIFICATION  0xD15C0

#endif /*_USB_HOST_GLOBAL_H_*/
