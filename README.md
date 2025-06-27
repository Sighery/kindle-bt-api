# Kindle Bluetooth

An attempt at trying to expose Bluetooth on Kindles to allow for more useful
functionality.

Bluetooth functionality is available since the Kindle Paperwhite 10th generation
(PW5). As far as I can tell, this includes both Classic and BLE.

The Kindles have some limited UI to allow for very basic Classic connection and
audio playback.

A CLI utility, `ace_bt_cli`, can be used to expose and connect to BLE devices.
Probably can also be used for connecting and using non-audio Classic devices
(such as HID). This I have not tested.

This project links against the existing `libace_bt` shared object, which
contains all the Bluetooth functionality. The CLI utility also makes use of this
library.
