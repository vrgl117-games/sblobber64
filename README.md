# sblobber64 for Nintendo64

Is it a slime? Is it a blob? who knows?!

N64 game developed for the N64brew Jam #1

Support 1 player and Rumble Pak.

![title](./resources/screenshots/title.png?raw=true)
![elems1](./resources/screenshots/level1.png?raw=true)
![elems2](./resources/screenshots/level2.png?raw=true)

## Note

This ROM file has been tested to work on real Nintendo 64 hardware using the
[EverDrive-64 by krikzz](http://krikzz.com/). It should also work with
[64drive by retroactive](http://64drive.retroactive.be/).

This ROM file is only known to work on low-level, high-accuracy Nintendo 64
emulators such as [CEN64](https://cen64.com/) or [MAME](http://mamedev.org/)
due to the use of [libdragon](https://dragonminded.com/n64dev/libdragon/)
instead of the proprietary SDK used by official licensed Nintendo software.

## Building

### Using Docker

All development can be done using Docker. It's the easiest way to build the ROM on Windows and MacOS.

 * Install [Docker](https://docker.com)
 * Run `make` to produce the `sblobber64.z64` ROM file.

 ### On linux

* Install [libdragon development toolchain](https://github.com/DragonMinded/libdragon)
* Run `make sblobber.z64` to produce the `sblobber.z64` ROM file.

## Testing

* Download / compile / install [CEN64](https://cen64.com/).
* Set the `CEN64_DIR` env var to the cen64 folder.
* Run `make cen64`.

## Thanks

* Thanks to [Kenney](https://www.kenney.nl) for the art used in the game.
* Thanks to [m0d](https://modarchive.org/index.php?request=view_artist_modules&query=69141&page=2#mods) for the music used in the game.
* Thanks to [Jennifer Taylor](https://github.com/DragonMinded) for libdragon.
* Thanks to [conker64](https://github.com/conker64) and the [#n64dev](irc://irc.efnet.org/#n64dev) IRC community for their help.
* Thanks to [Christopher Bonhage](https://github.com/meeq) for his work on [FlappyBird-N64](https://github.com/meeq/FlappyBird-N64).
