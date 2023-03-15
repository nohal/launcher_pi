# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# Copyright (c) 2022 Pavel Kalian
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
  "nohal/opencpn-plugins"
  CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
  "nohal/launcher_pi-beta"
  CACHE STRING
  "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
  "nohal/launcher_pi-stable"
  CACHE STRING
  "Default repository for tagged builds not matching 'beta'"
)

#
#
# -------  Plugin setup --------
#
set(PKG_NAME Launcher_pi)
set(PKG_VERSION "1.3.0")
set(PKG_PRERELEASE "beta")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME Launcher)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME launcher_pi) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Launcher plugin for OpenCPN")
set(PKG_DESCRIPTION [=[
  Launcher plugin for OpenCPN
]=])

set(PKG_AUTHOR "Pavel Kalian")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/nohal/launcher_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/launcher.html)

option(SANITIZE "What sanitizers to use" "")

if(NOT "${SANITIZE}" STREQUAL "OFF" AND NOT "${SANITIZE}" STREQUAL "")
  add_compile_options(-fsanitize=${SANITIZE} -fno-omit-frame-pointer)
  add_link_options(-fsanitize=${SANITIZE} -fno-omit-frame-pointer)
endif()

add_definitions(-DocpnUSE_GL)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(HDR_N
  ${CMAKE_SOURCE_DIR}/include/launcher_pi.h
  ${CMAKE_SOURCE_DIR}/include/LauncherSettingsDialog.h
  ${CMAKE_SOURCE_DIR}/include/LauncherUIDialog.h
)
set(SRC_N
  ${CMAKE_SOURCE_DIR}/src/launcher_pi.cpp
  ${CMAKE_SOURCE_DIR}/src/LauncherSettingsDialog.cpp
  ${CMAKE_SOURCE_DIR}/src/LauncherUIDialog.cpp
)

set(SRC
  ${HDR_N}
  ${SRC_N}
)

set(PKG_API_LIB api-18)  #  A dir in opencpn-libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.

endmacro()

macro(add_plugin_libraries)
endmacro()
