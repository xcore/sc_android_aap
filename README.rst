Android Accessory Protocol
..........................

:Stable release:  unreleased

:Status:  idea

:Maintainer:  vinithmundhra

:Description:  Implementation of Android Accessory Protocol


Key Features
============

* Recognizes Android (Samsung Google Nexus S) phone
* Puts (and checks) the phone into accessory mode
* Processes IR remote command and sends it to Android phone to toggle phone application button objects

Firmware Overview
=================

Initial IR Remote implementation is based on an iPod Dock V2 board. The Android device is first recognized (checked) and put into 'accessory' mode. Once this is successful, the XMOS device can communicate with the Android device using its bulk endpoints. Commands from IR remote is processed and sent to the application in the Android phone to toggle button objects.

Phone application is available from the Android developer website and can be downloaded on to the phone using Android SDK.

The IR folder contains code for processing remote commands.

Required Repositories
================

* xcommon git\@github.com:xcore/xcommon.git

Documentation
=============

See http://github.xcore.com/sc_android_aap

Installation
============

** Installing and running the demo (Command Line) **

From an environment that can access the XMOS Desktop Development tools, unzip the zipfile and from the app_android_aap_ir directory run:

   * xmake all
   * xflash bin/app_android_aap_ir/aap.xe
   * power cycle the hardware

To rapidly try out modifications to the firmware, from the app_android_aap_ir directory run:

   * xmake all
   * xrun bin/app_android_aap_ir/aap.xe

Firmware run in this way will not persist following a power cycle of the hardware.

** Installing and running the demo (XDE) **

To install, open the XDE Eclipse IDE and perform the following steps:

   * Select the "Import" item from the "File" menu
   * Select "General" -> "Existing Projects into Workspace" and click "Next"
   * Click on "Browse" next to "Select archive file" and select the button server zipfile 
   * Make sure the project you want to import is ticked in the "Projects:" list
   * Click on "Finish"

To build, select the project in the Project Explorer window and click on build icon.

To run, click on the run icon. The initial run attempt will prompt for confirmation of whether to write the firmware to the iPod Dock flash, or run the firmware.

   * if the firmware is written to flash, the hardware must then be power cycled
   * if the firmware is run directly from XDE it will not persist following a power cycle of the hardware

To configure the project to run on a particular device or in the simulator click on the drop down arrow next to the run icon and select "Run Configurations"

Support
=======

This release of firmware is supported on following XMOS Boards:
XR-IPOD-DOCK-V2 1V1

Please contact: vinithmundhra for firmware issues. Users wishing to develop this work further and who are not MFI licensed should contact XMOS to enquire about obtaining a suitable development board.

Known Issues
============

To Do
=====

