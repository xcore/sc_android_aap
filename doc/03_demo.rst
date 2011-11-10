Demo
====

The Demo required following items:
* A Samsung Google Nexus S Phone (with Android Version 2.3.6)
* An iPod Dock V2 board with XTAG and the firmware loaded
* IR Remote (the one by Apple is used here)

In this demo, the ADock waits and identifies an APhone and waits for an IR remote input.

The processes for APhone is carried out in USBHostStackWrapper() thread. The application thread waits for IR remote input. Once a IR remote command is received it is processed and is sent to the APhone using USB Bulk transfer functions.

In this demo the IR remote buttons are configured to toggle button objects in APhone application. This is the match:
* >|| = B1
* +   = B2
* -   = B3
* >>| = Gyro Position
* others = not configured to do anything

Every time a configured button pressed on IR remote it will toggle corresponding object in APhone application.

Note:
* If the phone enters sleep mode while the demo is in progress then it would break the connection and the setup must be reconnected.