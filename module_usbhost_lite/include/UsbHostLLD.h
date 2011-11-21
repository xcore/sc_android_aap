#ifndef __USBHOSTLLD_H__
#define __USBHOSTLLD_H__

#include <xs1.h>

#if (defined HW_1V3) || (defined BOARD_1V3) || (defined BOARD_XR_IPOD_DOCK_V1_1V3) || (defined BOARD_XCVR_TEST)
/* 1V3 Hardware Port Layout */
#ifdef __XC__
typedef struct
{
  in buffered port:4 pRCV;
  in buffered port:4 pVp;
  in buffered port:4 pVm;
  out buffered port:32 pVo;
  out buffered port:32 pFSE0;
  out buffered port:32 pOEn;
  out port pVBUS;
  clock bTx;
  clock bRx;
  in buffered port:16 pSofTimer; /* 16 bit dummy port */
} struct_usbports;
#else
typedef struct
{
  unsigned pRCV;
  unsigned pVp;
  unsigned pVm;
  unsigned pVo;
  unsigned pFSE0;
  unsigned pOEn;
  unsigned pVBUS;
  unsigned bTx;
  unsigned bRx;
  unsigned pSofTimer;
} struct_usbports;
#endif
#endif/* HW_1V3 */

#if (defined HW_2V0) || (defined BOARD_2V0) || (defined BOARD_XR_IPOD_DOCK_V2)
/* 2V0 Hardware Port Layout */
#ifdef __XC__
typedef struct
{
  in buffered port:4 pRCV;
  in buffered port:4 pSE0; /* 4 bit port */
  out buffered port:32 pVo;
  out buffered port:32 pFSE0;
  out buffered port:32 pOEn;
  out port pVBUS;
  clock bTx;
  clock bRx;
  in buffered port:16 pSofTimer; /* 16 bit dummy port */
} struct_usbports;
#else
typedef struct
{
  unsigned pRCV;
  unsigned pSE0;
  unsigned pVo;
  unsigned pFSE0;
  unsigned pOEn;
  unsigned pVBUS;
  unsigned bTx;
  unsigned bRx;
  unsigned pSofTimer;
} struct_usbports;
#endif /* __XC__ */
#endif /* HW_2V0 */

// LLD transfer types.
// Each endpoint type (control, bulk, interrupt and isochronous)
// has a function for each direction.
int USBLLD_ControlTransferIn(
    int addr,
    int bmRequestType_type, int bmRequestType_recipient,
    int bRequest, int wValue, int wIndex, int wLength,
    int maxPacketSize
    );

// This is a special case version of an IN control transfer when
// data size is 0 so no ACK is needed.
int USBLLD_ControlTransferInDir(
    int addr,
    int bmRequestType_type, int bmRequestType_recipient,
    int bRequest, int wValue, int wIndex, int wLength, int direction,
    int maxPacketSize
    );

int USBLLD_ControlTransferOut(
    int addr,
    int bmRequestType_type, int bmRequestType_recipient,
    int bRequest, int wValue, int wIndex, int wLength, char data[]
    );

int USBLLD_InterruptTransfer_In(
    int addr, int ep,
    int maxdataLen);

int USBLLD_InterruptTransfer_Out(
    int addr, int ep,
    int dataLen, char data[]);

int USBLLD_IsoTransfer_In(
    int addr, int ep,
    int maxdataLen);

int USBLLD_IsoTransfer_Out(
    int addr, int ep,
    int dataLen, char data[]);

int USBLLD_BulkTransfer_In(
    int addr, int ep,
    int maxdataLen);

int USBLLD_BulkTransfer_Out(
    int addr, int ep,
    int dataLen, char data[]);

// Subsections of setup packet
typedef enum
{
  BRT_DIRECTION_TO_DEV = 0,
  BRT_DIRECTION_FROM_DEV = 1,
} e_usb_brt_direction;

typedef enum
{
  BRT_RECIPIENT_DEVICE = 0,
  BRT_RECIPIENT_INTERFACE = 1,
  BRT_RECIPIENT_ENDPOINT = 2,
  BRT_RECIPIENT_OTHER = 3,
} e_usb_brt_recipient;

// Class
typedef enum
{
  BRT_TYPE_STANDARD = 0,
  BRT_TYPE_CLASS = 1,
  BRT_TYPE_VENDOR = 2,
} e_usb_brt_type;

// Struct for setup packet. Each has 8 bytes.
typedef struct bmRequestType
{
  e_usb_brt_recipient Recipient;       // [4..0]   Request directed to:
                                 //          0b00000: Device
                                 //          0b00001: Specific interface
                                 //          0b00010: Specific endpoint
                                 //          0b00011: Other element in device
  e_usb_brt_type Type;            // [6..5]   0b00: Standard request
                                 //          0b01: Class specific request
                                 //          0b10: Request by vendor specific driver
  e_usb_brt_direction Direction;       // [7]      0 (Host->Dev)
                                 //          1 (Dev->Host)
} BmRequestType;


/** Typedef for SetupPacket structure
  *
  **/
typedef struct setupPacket
{

  BmRequestType bmRequestType;   // (1 byte) Specifies direction of dataflow, type of rquest and recipient
  unsigned char bRequest;        // Specifies the request
  unsigned short wValue;         // (2 bytes) Host can use this to pass info to the device in its own way
  unsigned short wIndex;         // (2 bytes) Typically used to pass index/offset such as interface or EP no
  unsigned short wLength;        // (2 Bytes) Number of data bytes in the data stage (for Host -> Device this                                   // this is exact count, for Dev->Host is a max.
} SetupPacket;

// Return values to epManager from LLD
typedef enum
{
  USB_TRANS_ACKSTATE_ACK = 0x0,
  USB_TRANS_ACKSTATE_NAK = 0x1,
  USB_TRANS_ACKSTATE_STALL = 0x2,
  USB_TRANS_ACKSTATE_IGNORE = 0x3,
  USB_TRANS_ACKSTATE_HOSTDISCON = 0x4
} e_usb_trans_ackstate;

// Endpoint/transfer types
typedef enum
{
  USB_TRANSTYPE_BULK = 0x02,
  USB_TRANSTYPE_INTERRUPT = 0x03,
  USB_TRANSTYPE_ISO = 0x01,
  USB_TRANSTYPE_CONTROL = 0x00,
  USB_TRANSTYPE_SOF = 0x04,
} e_usb_transfertype;


// usbTransfer is the atomic element that is transferred between epManager and the LLD
// This defines all aspects of a usb transaction and the response is sent back in the
// same format.
// A Setup transaction can still be defined through this interface even though it
// consists of a setup packet, followed by IN/OUT transactions.
#define MAX_PKT_SIZE_WORDS 256
// ensure the above matches with the one in phy_rxdata.S
typedef struct usbTransfer
{
  e_usb_brt_direction direction;
  e_usb_transfertype transferType;
  int address; // USB Address
  int endpoint; // USB Endpoint number
  int dataLen; // Length of following payload data
  int transferComplete; // 0 = Incomplete, 1 = Complete
  e_usb_trans_ackstate ackState;
  unsigned data[MAX_PKT_SIZE_WORDS]; // Payload
  SetupPacket setupPacket; // (Optional) Setup packet for control transfers
  int received_dataLen; // Length of data received in case of partial packets split over SOF intervals
  int retry_count; // number of attempts to send packet so we can detect disconnect events
  int maxPacketSize;
  int timeout_ts;
} UsbTransfer;

/* structure for handling timed transactions (Isochronous / Interrupt) */
typedef struct S_TIMED_TRANSACTION 
{
    int dev_addr;                       // USB device address
    int EP_addr;                        // USB End Point address
    e_usb_transfertype trans_type;      // transaction type - interrupt (control)  / Isoc
    e_usb_brt_direction direction;      // direction of request - IN / OUT
    char bInterval;                     // interval value in mS
    char count;                         // interval counter
    int active;
    int maxPktLen;                      // maximum packet length
} t_timed_transaction;


#if defined __XC__
void usbHostLLD (struct_usbports &s_usbports, streaming chanend ?cVBus, chanend cCtrl, streaming chanend cSOF, unsigned se0_detect_shift);
#elif defined __STDC__
void usbHostLLD (struct_usbports *s_usbports, unsigned cVBus, unsigned cCtrl, unsigned cSOF, unsigned se0_detect_shift);
#endif

typedef enum
{
  USB_CMD_POWER_CYCLE = 1,
  USB_CMD_SOFT_RESET = 2,
  USB_CMD_DEVICE_RESET = 3,
  USB_CMD_DRAIN_BUFFER = 4,
  USB_CMD_WAIT_FOR_DEVICE = 5,
  USB_CMD_INIT_TIMED_TRANS = 6,
} e_usb_ctrl_cmd;

void install_trap_handler();
void reset_on_trap();

#ifdef __STDC__

int USBLLD_removesof(char *ackstate);
int USBLLD_getreadhandle(UsbTransfer **handle);

#endif

void USBLLD_readfinished();

/**
 * Get a handle for a USB Transfer object for reading out of the buffer (application/epMan layer)
 */
#ifdef __STDC__
int epManTimed_getReadHandle( UsbTransfer **handle );
#endif

/**
 * Application layer finished with buffer entry
 */
void epManTimed_readFinished( );

#endif

#ifndef __XC__
int WaitForResponse(UsbTransfer **usbTrans);
#endif
