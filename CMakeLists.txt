##---------------------------------------------------------------------------
## Author:      Pavel Kalian
## Copyright:   
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(launcher_pi)

SET(PACKAGE_NAME launcher_pi)
SET(VERBOSE_NAME Launcher)
SET(TITLE_NAME Launcher)
SET(CPACK_PACKAGE_CONTACT "Pavel Kalian")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "3")

SET(NAME_SUFFIX "ov50")

#SET(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_MACOSX_RPATH "ON")

set (CMAKE_CXX_STANDARD 11)

OPTION(LAUNCHER_USE_SVG "Use SVG graphics" ON)

INCLUDE("cmake/PluginConfigure.cmake")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

IF(LAUNCHER_USE_SVG)
  ADD_DEFINITIONS(-DLAUNCHER_USE_SVG)
ENDIF(LAUNCHER_USE_SVG)

SET(SRC_LAUNCHER
            src/launcher_pi.h
            src/launcher_pi.cpp
            src/icons.h
            src/icons.cpp
            src/LauncherUIDialog.h
            src/LauncherUIDialog.cpp
            src/LauncherSettingsDialog.h
            src/LauncherSettingsDialog.cpp
)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_LAUNCHER})

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
