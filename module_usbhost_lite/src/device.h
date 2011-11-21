#ifndef DEVICE_H_
#define DEVICE_H_

#include "usb_descriptors.h"

#ifdef __STDC__
extern int nDeviceEPs;
extern s_usb_endpoint deviceEPs[];
extern int nDeviceInterfaces;
extern s_usb_interface deviceInterfaces[]; 
#endif

extern int deviceAddress;
extern const int vendorIDMask;
extern const int vendorID;
extern const int productIDMask;
extern const int productID;

#endif /*DEVICE_H_*/

