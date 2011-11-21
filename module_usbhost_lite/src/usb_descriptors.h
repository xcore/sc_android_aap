#ifndef __USB_DESCRIPTORS_H__
#define __USB_DESCRIPTORS_H__

#include "UsbHostLLD.h"
#include <xccompat.h>
#include "usb_host_global.h"
// Defines for standard descriptor requests


typedef enum
{
  BREQ_GET_STATUS = 0x00,
  BREQ_CLEAR_FEATURE = 0x01,
  BREQ_SET_FEATURE = 0x03,
  BREQ_SET_ADDRESS = 0x05,
  BREQ_GET_DESCRIPTOR = 0x06,
  BREQ_SET_DESCRIPTOR = 0x07,
  BREQ_GET_CONFIGURATION = 0x08,
  BREQ_SET_CONFIGURATION = 0x09,
  BREQ_GET_INTERFACE = 0x0A,
  BREQ_SET_INTERFACE = 0x0B,
  BREQ_SYNCH_FRAME = 0x0C
} BREQ;

typedef enum
{
  EPTransferType_Control = 0,
  EPTransferType_Iso = 1,
  EPTransferType_Bulk = 2,
  EPTransferType_Interrupt = 3,
} e_usb_epTransferType;

typedef enum
{
  EPSyncType_None = 0,
  EPSyncType_Async = 1,
  EPSyncType_Adapt = 2,
  EPSyncType_Sync = 3,
} e_usb_epSyncType;

typedef enum
{
  EPUsageType_Data = 0,
  EPUsageType_Feedback = 1,
  EPUsageType_ImpFeed = 2,
  EPUsageType_Rsv = 3,
} e_usb_epUsageType;

typedef enum
{
  EPIFClass_Audio = 0x01,
  EPIFClass_Comms = 0x02,
  EPIFClass_HID = 0x03,
  EPIFClass_Physical = 0x05,
  EPIFClass_Image = 0x06,
  EPIFClass_Printer = 0x07,
  EPIFClass_MassStorage = 0x08,
  EPIFClass_CDC = 0x0A,
  EPIFClass_SmartCard = 0x0B,
  EPIFClass_ContentSecurity = 0x0D,
  EPIFClass_Video = 0x0E,
  EPIFClass_PersonalHealthcare = 0X0F,
  EPIFClass_Diagnostic = 0xDC,
  EPIFClass_Wireless = 0xE0,
  EPIFClass_Misc = 0xEF,
  EPIFClass_AppSpecific = 0xFE,
  EPIFClass_VendorSpecific = 0xFF,
} e_usb_epIFClass;

typedef enum
{
  EPIFSubClass_AudioControl = 0x101,
  EPIFSubClass_AudioStreaming = 0x102,
  EPIFSubClass_MIDIStreaming = 0x103,
  EPIFSubClass_MS_RBC = 0x201,
  EPIFSubClass_MS_SFF8020 = 0x202,
  EPIFSubClass_MS_QIC = 0x203,
  EPIFSubClass_MS_UFI = 0x204,
  EPIFSubClass_MS_SFF8070 = 0x205,
  EPIFSubClass_MS_SCSI = 0x206,
  EPIFSubClass_VendorSpecific = 0xFF,
} e_usb_epIFSubClass;

typedef enum
{
  EPIFProtocol_MS_BulkOnly = 0x150,
} e_usb_epIFProtocol;

#ifndef __XC__
typedef struct
{
  e_usb_epTransferType transferType;
  e_usb_epSyncType     syncType;
  e_usb_epUsageType    usageType;
  int dir; // 1 = IN, 0 = OUT
  int EPnum;
  int maxPacketSize;
  int interval;
  int nextInterruptPacketTime;
  int interfaceNumber;
  int epfound;
  int (*rx)(chanend, char[], int, int, int);
} s_usb_endpoint;

typedef struct
{
  int interfaceNumber;
  int interfaceFound;
  int alternateSetting;
  int numEndpoints;
  int interfaceClass;
  int interfaceSubClass;
  int interfaceProtocol;
  int interfaceStringIndex;
  int (*parseDescriptor)(char[], int);
  int (*ctrlRequest)(streaming_chanend, int, int);
  int (*timeout) (streaming_chanend, int);
  int (*flush) (streaming_chanend);
  void (*init) (void);
} s_usb_interface;
#endif

typedef enum
{
  DESC_TYPE_DEVICE = 0x01,
  DESC_TYPE_CONFIGURATION = 0x02,
  DESC_TYPE_STRING = 0x03,
  DESC_TYPE_INTERFACE = 0x04,
  DESC_TYPE_ENDPOINT = 0x05,
  DESC_TYPE_DEVICE_QUALIFIER = 0x06,
  DESC_TYPE_OTHER_SPEED_CFG = 0x07,
  DESC_TYPE_INTERFACE_POWER = 0x08,
} e_usb_descType;

#ifdef __STDC__

#pragma pack(1)
typedef struct deviceDescriptor 
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned short bcdUSB;
  unsigned char bDeviceClass;
  unsigned char bDeviceSubClass;
  unsigned char bDeviceProtocol;
  unsigned char bMaxPacketSize;
  unsigned short idVendor;
  unsigned short idProduct;
  unsigned short bcdDevice;
  unsigned char iManufacturer;
  unsigned char iProduct;
  unsigned char iSerialNumber;
  unsigned char bNumConfigurations;
} DeviceDescriptor;

// See USB spec 9.6.3
#pragma pack(1)
typedef struct configDescriptor 
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned short wTotalLength;
  unsigned char bNumInterfaces;
  unsigned char bConfigurationValue;
  unsigned char iConfiguration;
  unsigned char bmAttributes;
  unsigned char bMaxPower;
} ConfigDescriptor;

// See USB spec 9.6.5
#pragma pack(1)
typedef struct interfaceDescriptor 
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned char bInterfaceNumber;
  unsigned char bAlternateSetting;
  unsigned char bNumEndpoints;
  unsigned char bInterfaceClass;
  unsigned char bInterfaceSubClass;
  unsigned char bInterfaceProtocol;
  unsigned char iInterface;
} InterfaceDescriptor;

// See USB spec 9.6.6
#pragma pack(1)
typedef struct endpointDescriptor
{
  unsigned char bLength;
  unsigned char bDescriptorType;
  unsigned char bEndpointAddress;
  unsigned char bmAttributes;
  unsigned short wMaxPacketSize;
  unsigned char bInterval;
} EndpointDescriptor;


#endif

#endif
