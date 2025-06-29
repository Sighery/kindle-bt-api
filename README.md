# Kindle Bluetooth

An attempt at trying to expose Bluetooth on Kindles to allow for more useful
functionality.

The Bluetooth functionality this project targets is available since the Kindle
Paperwhite 11th generation (PW5). As far as I can tell, this includes both
Classic and BLE.

The Kindles have some limited UI to allow for very basic Classic connection and
audio playback.

A CLI utility, `ace_bt_cli`, can be used to expose and connect to BLE devices.
Probably can also be used for connecting and using non-audio Classic devices
(such as HID). This I have not tested.

This project links against the existing `libace_bt` shared object, which
contains all the Bluetooth functionality. The CLI utility also makes use of this
library.

## Howto

1. Install `koxtoolchain` using target `kindlehf`.
   [Follow the Kindle Modding Wiki][Kindle Modding Wiki prerequisites].
2. Install `kindle-sdk` from [my fork][kindle-sdk fork], using target `scribe1`.
   You may also
   [follow the Kindle Modding Wiki][Kindle Modding Wiki prerequisites]. With the
   caveat of using my fork's URL instead of theirs. I aim to merge the changes
   back into upstream in the future.
3. Set up meson for cross-compilation:

```sh
# Set up build dir
make .build_scribe1

# Configure cross compilation
meson setup --wipe -Dkindle_root_dir=/home/user/x-tools/arm-kindlehf-linux-gnueabihf/arm-kindlehf-linux-gnueabihf/sysroot/ --cross-file ~/x-tools/arm-kindlehf-linux-gnueabihf/meson-crosscompile.txt .build_scribe1/

# Compile
meson compile -C .build_scribe1
```

## Useful Kindle BT notes

The Kindle comes with `ace_bt_cli`, a CLI utility wrapping around the `ace_bt`
library. This tool is a bit particular, but I've compiled a
[recipe on a basic BLE read/write interaction][ace_bt_cli recipe]. A list of the
options with some extra info [is also available][ace_bt_cli help].

Bluetooth operations seem to require using the `bluetooth` user and group. The
`ace_bt_cli` automatically drops privileges.

`ace_bt` logs can be found under `/var/log/messages`. This file has pretty
aggressive rotation. Use something like `tail -F /var/log/messages | grep -i
ace`.

HCI logging can be enabled and found under `/mnt/us/bt_snoop_log`. This is a
binary file format that requires Wireshark to inspect.

Sometimes the Bluetooth manager is finicky. Restart with
`initctl restart btmanagerd`. Give it some seconds to fully initialise back.

## Notes

* `5.16.3` seems to have old API for `aceBT_bleRegisterGattClient` and
`aceBt_bleRegisterGattClient`? Using `5.17.2` works with the headers.



[Kindle Modding Wiki prerequisites]: https://kindlemodding.org/kindle-dev/gtk-tutorial/prerequisites.html
[kindle-sdk fork]: https://github.com/Sighery/kindle-sdk
[ace_bt_cli recipe]: https://github.com/Sighery/kindle-notes/blob/master/bluetooth/kindle_pico_led_recipe.md
[ace_bt_cli help]: https://github.com/Sighery/kindle-notes/blob/master/bluetooth/ace_bt_cli_help.txt
