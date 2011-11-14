#ifndef __USB_HOST_APP_CONF_H__
#define __USB_HOST_APP_CONF_H__

/* Number of CTRL chanends used. This is defined by the number of interfaces that are in your descriptor */
#define NUM_CTRL_CHANENDS 1

/* set the configuration to start with - useful if we know which configuration we want */
#define START_CONFIG_NUM 0

/* maximum number of timed EP's we have (e.g. Isoc / Interrupt EPs) */
#define TIMED_EP_COUNT 0

/* Initial get descriptor length to try, will retry if actual length is longer */
#define INITIAL_GET_DEV_DESC_LEN         18

/* EP0 Pipe width - for FS this should only ever be 64 bytes */
#define DEFAULT_EP0_PIPE_WIDTH         64

/* Initial get configuration descriptor length to try, will retry if length is longer */
#define INITIAL_GET_CONF_DESC_LEN    255

/* Define the amount of time between connections in mS (for 0ms comment this define out) */
#define RECONNECT_PAUSE     10

/* enable hook for VID/PID match failure */
#define USBHOST_VID_PID_HOOK_ON

#ifdef USBHOST_VID_PID_HOOK_ON
#ifndef __XC__
int USBHost_user_vid_pid_fail( DeviceDescriptor *devicedesc, int vendorIDMask, int vendorID, int productIDMask, int productID );
#endif
#endif

#endif /* __USB_HOST_APP_CONF_H__ */
