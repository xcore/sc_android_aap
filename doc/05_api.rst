AAP API
=======

Files
-----

**module_android_aap\src\usb_host_app_conf.h**

This file must provide generic USB host defines for your device. It has preprocessor defines that the USB Host will use.

**module_android_aap\src\AAP_if.h**

This file contains hook functions from USB host in order to follow specified enumeration process and attempt to start the Android device in 'accessory' mode.

**module_android_aap\src\AndroidDescriptor.c**

This file contains USB decriptors for the Android device.

Configuration Defines and Constants
-----------------------------------

**NUM_CTRL_CHANENDS**

This is the number of control channels in your application or the number of interfaces in the descriptor. As we need to work with only one, this is made 1.

**START_CONFIG_NUM**

Index of the starting configuration we want to work with. Required interface is present in configuration number 0.

**TIMED_EP_COUNT**

Maximum number of time endpoints for isochronous and interrupt endpoints. As we are working with bulk endpoints, this is made 0.

**INITIAL_GET_DEV_DESC_LEN**

Length of the device descriptor. Currently set as 18. Extra descriptor bytes (if any) will be read automatically after reading the first 18 bytes.

**DEFAULT_EP0_PIPE_WIDTH**

For Full Speed USB this is set as 64.

**INITIAL_GET_CONF_DESC_LEN**

Length of configuration descriptor. Currently set as 255. Any extra bytes (if any) will be read automatically after reading the initial bytes.

**RECONNECT_PAUSE**

The amount of time (in mS) between connection / re-connection. Android accessory might need to re-introduce itself after putting the Android device in 'special' accessory mode. This will require a re-connection.

**USBHOST_VID_PID_HOOK_ON**

This should be defined for AAP as we need a hook from device descriptor mismatch. This hook will contain the code to start the Android device in accessory mode.

**ACC_GET_PROTOCOL**

Control transfer command to get Android device's protocol support. Currently set as 51 (0x33).

**ACC_SEND_STRING**

Control transfer command to send strings (introduce accessory) to the Android device. Currently set as 52 (0x34).

**ACC_MODE_START**

Control transfer command to set Android device in accessory mode. Currently set as 53 (0x35).

**EP_DESC_COUNT**

Number of endpoint descriptors to expect from the Android device. Nexus S Android phone has 2.

**IF_DESC_COUNT**

Number of interface descriptors to expect from the Android device. Nexus S Android phone has 1.

**vendorID**

Vendor ID of the Android device. Nexus S VID is 0x18D1.

**productID**

Product ID of the Android device in accessory mode. Nexus S PID should be either 0x2D00 or 0x2D01. The PID of this phone (when not in accessory mode) is 0x4E22.

**vendorIDMask**

Mask to match VID

**productIDMask**

Mask to match PID

Typedefs
--------

**s_usb_endpoint**

The USB Endpoint descriptor. Each endpoint descriptor typedef will also contain function pointer for that specific endpoint type. For example, the IN endpoint has a function pointer to android_rx() to handle data response for an IN transfer.

**s_usb_interface**

The USB Interface descriptor. Each interface has handlers. The android_ctrl() function pointer is used as an interface between the application thread and the USB thread. 

Prototypes
----------

USBHost_user_vid_pid_fail
+++++++++++++++++++++++++

Implementation file: module_android_aap\src\USBHostUser.c

Return type: int

Param:

* DeviceDescriptor *devicedesc - the device descriptor got from the USB device
* int vendorIDMask - VID mask
* int vendorID - VID. For Google Nexus S, this should be 18D1
* int productIDMask - PID mask
* int productID - PID. For Google Nexus S, this should be 4E21 when not in accessory mode and 2D00 or 2D01 when in accessory mode.

Description:

When the Device descriptor do not match, it might be possible that the Android device is not in accessory mode. This hook is useful to attempt to start the Android device in accessory mode by checking the protocol, introducing accessory, setting the accessory mode (if any) and restarting the enumeration process.

Once the above process is done, the Android device should start in accessory mode satisfying the VID/PID match.

android_ctrl
++++++++++++

Implementation file: module_android_aap\src\AAP_if.xc

Return type: int

Param:

* streaming chanend c - channel between application and USB thread. All data from the application comes via this channel.
* int reqVal - Request value from the application.
* int timeNow - current time.

Description:

Every time the application thread wants to send data over the USB, it pushes data onto this channel (c). The data is received by this function from the application thread and it is passed to the USB as a Bulk OUT transfer.

android_rx
++++++++++

Implementation file: module_android_aap\src\AAP_if.xc

Return type: int

Param:

* streaming chanend c
* unsigned char data[]
* int datalen
* int ackstate
* int timedTrans

Description:

Handle any data that comes in response to an IN, send to application via a channel.

application_thread
++++++++++++++++++

Implementation files: app_android_aap_ir\src\main.xc

Return type: int

Param:

* streaming chanend cCtrl[NUM_CTRL_CHANENDS] - all the control channels.
* streaming chanend cSOFGen - USB SOF channel.
* streaming chanend cVBus - USB VBus channel.
* out port pGPIO - Port to setup VBus.

Description:

This thread sets up the USB host by requesting disconnection notification, setup VBus and ask USB host to use internal SOF timing. It also interprets IR commands and prepares appropriate packet to send to the USB device. It send this packet over the control channel. The data on this channel is received by the android_ctrl() function.
