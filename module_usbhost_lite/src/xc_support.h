#ifndef XC_SUPPORT_H_
#define XC_SUPPORT_H_

#include <xccompat.h>
#include "usb_host_global.h"
#include "usb_host_app_conf.h"

/*
 * getTime:
 * Get a reference time value from the local 100Mhz clock
 */
int getTime();

/*
 * pause:
 * Pause the current thread for a number of ticks from the
 * local 100Mhz clock
 */
#ifdef __XC__
void pause(int ticks);
#else
void pause(int ticks);
#endif

/*
 * startChans
 * Receive flags from every channel end
 * These say if the channel requires knowledge of device disconnection
 */
void startChans(streaming_chanend cClients[NUM_CTRL_CHANENDS], int disconnectflag[NUM_CTRL_CHANENDS]);

/*
 * pollChans:
 * Poll for requests on the client chanels
 */
void pollChans(streaming_chanend cClients[NUM_CTRL_CHANENDS], REFERENCE_PARAM(int, ctrlchan), REFERENCE_PARAM(int, val));

/*
 * endChans:
 * Called on device disconnection
 * Only channels which require knowledge of device disconnection will be told
 */
void endChans(streaming_chanend cClients[NUM_CTRL_CHANENDS], int disconnectflag[NUM_CTRL_CHANENDS]);

/*
 * USB_Ctrl
 * Signal control information to the USB LLD.
 * This includes power cycle
 */
int USB_Ctrl(chanend cUSBCtrl, unsigned data);

/*
 * Initialise the timed EP's via channel
 */
void init_timed_eps_chan(chanend cUSBCtrl, unsigned timed_ep_count);

/*
 * USBHostStackWrapper:
 * Wraps USBHostStack with fast mode if required
 */
#ifdef __XC__
void USBHostStackWrapper(streaming_chanend cClients[NUM_CTRL_CHANENDS], chanend cUSBCtrl);
#else
void USBHostStackWrapper(streaming_chanend cClients[NUM_CTRL_CHANENDS], chanend cUSBCtrl);
#endif

#endif /*XC_SUPPORT_H_*/
