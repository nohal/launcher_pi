/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Launcher Plugin
 * Author:   Pavel Kalian
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Pavel Kalian                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#ifndef _LAUNCHERPI_H_
#define _LAUNCHERPI_H_

#include <cstdint>
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <wx/arrstr.h>
#include <wx/fileconf.h>

#include "config.h"

#define MY_API_VERSION_MAJOR 1
#define MY_API_VERSION_MINOR 13

#include "ocpn_plugin.h"

#include "LauncherSettingsDialog.h"
#include "LauncherUIDialog.h"

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define LAUNCHER_TOOL_POSITION \
  -1  // Request default positioning of toolbar tool

class launcher_pi : public opencpn_plugin_113 {
  friend class LauncherUIDialog;

public:
  launcher_pi(void* ppimgr);
  ~launcher_pi();

  //    The required PlugIn Methods
  int Init();
  bool DeInit();

  int GetAPIVersionMajor();
  int GetAPIVersionMinor();
  int GetPlugInVersionMajor();
  int GetPlugInVersionMinor();
  wxBitmap* GetPlugInBitmap();
  wxString GetCommonName();
  wxString GetShortDescription();
  wxString GetLongDescription();

  //    The override PlugIn Methods
  void SetCursorLatLon(double lat, double lon);
  int GetToolbarToolCount();
  void ShowPreferencesDialog(wxWindow* parent);
  void OnToolbarToolCallback(int id);
  void SetPositionFix(PlugIn_Position_Fix& pfix);

  // Other public methods
  void SetColorScheme(PI_ColorScheme cs);
  wxString GetDataDir() {
    return GetPluginDataDir("Launcher_pi") + wxFileName::GetPathSeparator() +
           "data" + wxFileName::GetPathSeparator();
  }

private:
  bool LoadConfig();
  bool SaveConfig();

  wxFileConfig* m_pconfig;
  wxWindow* m_parent_window;

  LauncherUIDialog* m_pLauncherDialog;
  LauncherSettingsDialog* m_pLauncherSettingsDialog;

  int m_display_width, m_display_height;
  int m_leftclick_tool_id;

  wxString m_launcher_labels, m_launcher_commands;
  wxArrayString m_alauncher_labels, m_alauncher_commands;
  bool m_hide_on_btn;
  wxBitmap m_logo;

protected:
  int m_window_width;
  int m_window_height;
  int m_window_pos_x;
  int m_window_pos_y;
};
#endif
