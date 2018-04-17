# Launcher Plugin for OpenCPN

- The plugin lets the user configure several commands which then can be launched from a simple dialog accessible from the main toolbar.
- The plugin allows sending sequences of keystrokes to the main OpenCPN application. This allows to invoke virtually any OpenCPN function normally available using the keyboard on a system without it, eg. a tablet.

## Keyboard events

- Start the command definition with string `KBD:` followed by coma separated list of key events to send to the main application. A component starting with `!` (explanation mark) represents a key down even, a component without the beginning `!` represents akey up event. Example: `KBD:!S,S` represents a short press of the `S` key, resulting in turning on/of the soundings.

### Special keys

- Special keys may be used if implemented by the plugin. Example: `KBD:!F5,F5` represents a short press of the F5 key, resulting in switching the color scheme.
- Special keys implemented: F1-F12

## Variable substitution

You can use several variables in your commands which will be replaced wit the actual values at the moment of command invocation

The variables implemented so far include:

| Variable | Meaning |
| -------- | ------- |
| %BOAT_LAT% | Boat Latitude |
| %BOAT_LON% | Boat longitude |
| %BOAT_SOG% | Boat SOG |
| %BOAT_COG% | Boat COG |
| %BOAT_VAR% | Magnetic variation |
| %BOAT_FIXTIME% | The timestamp of the past fix (seconds since 01/01/1970 UTC) |
| %BOAT_NSATS% | Number of satelites "visible" during the last fix |

### Examples
Open bing satellite imagery at the boat location:

```firefox http://www.bing.com/maps/?v=2&cp=%BOAT_LAT%~%BOAT_LON%&lvl=16&dir=0&sty=h&form=LMLTCC```

Turn of a Linux computer (You must add something like "username    ALL=(ALL) NOPASSWD: /sbin/shutdown" into /etc/sudoers to be able to perform the shutdown without needing a password):

```sudo shutdown -h now &```

## Building
This assumes you have already seen the building instructions at http://opencpn.org/ocpn/compiling_source_linux or http://opencpn.org/ocpn/compiling_source_windows and can build OpenCPN from source.

### Linux / macOS:

```
cd YOUROPENCPNSOURCETREE/plugins
git clone git clone https://github.com/nohal/launcher_pi.git
cd ../build
cmake ..
make
```

### Windows:

```
cd C:\YOUROPENCPNSOURCETREE\plugins
git clone git clone https://github.com/nohal/launcher_pi.git
cd ..\build
cmake ..
cmake --build . --config release
```
