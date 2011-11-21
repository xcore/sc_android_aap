#ifndef EP_MANAGER_H_
#define EP_MANAGER_H_

#include <xccompat.h>

#include "UsbHostLLD.h"
#include "usb_host_global.h"
#include "xc_support.h"

#ifdef __XC__
extern int deviceConnected;
#else
extern volatile int deviceConnected;
#endif

/*
 * USBHostStack
 * Provides EndPoint management and client channels
 */
#ifdef __XC__
void USBHostStack(streaming_chanend cClients[NUM_CTRL_CHANENDS], chanend cUSBCtrl);
#else
void USBHostStack(streaming_chanend cClients[NUM_CTRL_CHANENDS], chanend cUSBCtrl);
#endif

void usb_buf_wait_for_control();
#ifdef __STDC__
int WaitForResponse(UsbTransfer **usbTrans);
#endif

#endif /*EP_MANAGER_H_*/

