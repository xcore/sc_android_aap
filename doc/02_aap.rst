Android Accessory Protocol
==========================

[1] http://developer.android.com/guide/topics/usb/adk.html

Android Accessory Protocol defines how an accessory detects and sets up communication with an Android-powered device.

External USB devices can communicate with Android-powered device (Version 2.3.4 and above - check!) in a special 'accessory' mode. Using this, the Android-powered device can act as a 'USB Device'. For simplicity, let's call the Android-powered device as 'APhone' and the connected USB accessory device as 'ADock'.

When APhone is in 'accessory' mode:

* ADock acts as USB host, powers the bus and enumerates devices
* APhone acts as USB Device

Please note that not all Android-powered devices support the 'accessory' mode and this must be checked while enumeration.

Implementation
--------------

[2] http://developer.android.com/guide/topics/usb/adk.html#accessory-protocol

ADock must follow these steps before communicating with APhone:

* Wait for and detect connected devices
* Determine APhone mode support
* Attmept to start APhone in special accessory mode
* Establish communication if APhone supports mode

Determine APhone mode support and start in accessory mode
---------------------------------------------------------
If the Vendor ID is 0x18D1 and Product ID is (0x2D00 | 0x2D01), then APhone supports and is in accessory mode

If VID and PID do not satisfy matches then attempt to start in accessory mode by:

* Send a 51 control request ('Get Protocol') on EP0. A non-zero return value indicates mode supported.
* Send ADock identification strings (Control request 52 on EP0) for appropriate application selection on APhone.
* Request the device to start in accessory mode by sending control request 53 on EP0.

After sending the last control request, ADock must restart the enumeration process for this device. If at any point these steps fail, indicates that APhone does not support accessory mode and ADock must start looking for newly connected devices.

Establish communication with device
-----------------------------------
Once APhone is in accessory mode, set it's configuration (0x09) to 1 (one). ADock must also find the first bulk input and output endpoints in APhone's interface descriptor. These bulk endpoints can now be used for communication with the APhone.

