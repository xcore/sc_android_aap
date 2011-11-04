Android Accessory Protocol
..........................

:Stable release:  unreleased

:Status:  idea

:Maintainer:  vinithmundhra

:Description:  Implementation of Android Accessory Protocol


Key Features
============

* Recognizes Android (Samsung Google Nexus) phone
* Puts (and checks) the phone into accessory mode
* Processes IR remote command and sends it to Android phone to toggle phone application button objects

To Do
=====


Firmware Overview
=================

Initial IR Remote implementation is based on an iPod Dock V2 board. The Android device is first recognized (checked) and put into 'accessory' mode. Once this is successful, the XMOS device can communicate with the Android device using its bulk endpoints. Commands from IR remote is processed and sent to the application in the Android phone to toggle button objects.

Phone application is available from the Android developer website and can be downloaded on to the phone using Android SDK.

The IR folder contains code for processing remote commands.

Please note that there might be some references to iPod - kindly ignore them.

Known Issues
============


Required Repositories
================

* sc_usb_host
* xcommon git\@github.com:xcore/xcommon.git

Support
=======

Please contact: vinithmundhra