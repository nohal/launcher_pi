#Launcher Plugin for OpenCPN

The plugin lets the user configure several commands which then can be launched from a simple dialog accessible from the main toolbar.

##Variable substitution

You can use several variables in your commands which will be replaced wit the actual values at the moment of command invocation

The variables implemented so far include:

- %BOAT_LAT% - Boat Latitude
- %BOAT_LON% - Boat longitude
- %BOAT_SOG% - Boat SOG
- %BOAT_COG% - Boat COG
- %BOAT_VAR% - Magnetic variation
- %BOAT_FIXTIME% - The timestamp of the past fix (seconds since 01/01/1970 UTC)
- %BOAT_NSATS - Number of satelites "visible" during the last fix

###Examples
Open bing satellite imagery at the boat location:

**firefox http://www.bing.com/maps/?v=2&cp=%BOAT_LAT%~%BOAT_LON%&lvl=16&dir=0&sty=h&form=LMLTCC**

Turn of a Linux computer (You must add something like "username    ALL=(ALL) NOPASSWD: /sbin/shutdown" into /etc/sudoers to be able to perform the shutdown without needing a password):

**sudo shutdown -h now &**

##Building
This assumes you have already seen the building instructions at http://opencpn.org/ocpn/compiling_source_linux or http://opencpn.org/ocpn/compiling_source_windows and can build OpenCPN from source.

Linux:
cd YOUROPENCPNSOURCETREE/plugins
git clone git clone https://github.com/nohal/launcher_pi.git
cd ../build
cmake ..
make

Windows:
cd C:\YOUROPENCPNSOURCETREE\plugins
git clone git clone https://github.com/nohal/launcher_pi.git
cd ..\build
cmake ..
cmake --build . --config release
