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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

// ===========================================================================
// wxJoin and wxSplit
// ===========================================================================

#if wxCHECK_VERSION(2, 9, 0)
#else
#include "wx/tokenzr.h"

wxString wxJoin(const wxArrayString& arr, const wxChar sep, const wxChar escape)
{
    size_t max = arr.GetCount();
    if (max == 0)
        return wxEmptyString;

    wxString str;

    // the "medium" lenght of the strings in the given array: how we compute it
    // is very arbitrary but this is better than nothing
    size_t mlen = (arr.Item(0).Len() + arr.Item(max - 1).Len()) / 2;
    str.Alloc(max * mlen);

    if (escape == wxT('\0')) {
        // escaping is disabled:

        for (size_t i = 0; i < max; i++)
            str << arr.Item(i) << sep;
    } else {
        for (size_t i = 0; i < max; i++) {
            for (const wxChar* p = arr.Item(i); *p != wxT('\0'); p++) {
                if (*p == sep)
                    str << escape; // escape this separator
                str << *p;
            }

            str << sep;
        }
    }

    // remove the last separator
    str.RemoveLast();
    return str;
}

wxArrayString wxSplit(
    const wxString& str, const wxChar sep, const wxChar escape)
{
    if (escape == wxT('\0'))
        // we don't need to honour the escape character
        return wxStringTokenize(str, sep,
            wxTOKEN_RET_EMPTY_ALL /* return all tokens, event empty ones */);

    wxArrayString ret;
    wxString curr;
    wxChar prev = wxT('\0');

    for (const wxChar* p = str.c_str(); *p != wxT('\0'); p++) {
        if (*p == sep) {
            if (prev == escape) {
                // remove the escape character and don't consider this
                // occurrence of 'sep' as a real separator
                curr.Last() = sep;
            } else {
                // add only non-empty tokens
                ret.Add(curr);
                curr.Empty();
            }
        } else
            curr << *p;

        prev = *p;
    }

    // add the last token
    ret.Add(curr);

    return ret;
}
#endif

#include "launcher_pi.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void* ppimgr)
{
    return new launcher_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) { delete p; }

//---------------------------------------------------------------------------------------------------------
//
//    Launcher PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

launcher_pi::launcher_pi(void* ppimgr)
    : opencpn_plugin_113(ppimgr)
{
    m_hide_on_btn = true;
    // Create the PlugIn icons
    m_window_width = 0;
    m_window_height = 0;
    m_window_pos_x = 0;
    m_window_pos_y = 0;
    m_logo = GetBitmapFromSVGFile(GetDataDir() + "launcher_pi.svg", 32, 32);
}

launcher_pi::~launcher_pi() { }

int launcher_pi::Init()
{
    AddLocaleCatalog(_T ( "opencpn-launcher_pi" ));

    ::wxDisplaySize(&m_display_width, &m_display_height);

    //    Get a pointer to the opencpn configuration object
    m_pconfig = GetOCPNConfigObject();

    //    And load the configuration items
    LoadConfig();

    // Get a pointer to the opencpn display canvas, to use as a parent for the
    // LAUNCHER dialog
    m_parent_window = GetOCPNCanvasWindow();

    wxString _svg_launcher = GetDataDir() + "launcher_pi.svg";
    wxString _svg_launcher_rollover = GetDataDir() + "launcher_pi_rollover.svg";
    wxString _svg_launcher_toggled = GetDataDir() + "launcher_pi_toggled.svg";
    m_leftclick_tool_id = InsertPlugInToolSVG(_T( "Launcher" ), _svg_launcher,
        _svg_launcher_rollover, _svg_launcher_toggled, wxITEM_CHECK,
        _("Launcher"), _T( "" ), nullptr, LAUNCHER_TOOL_POSITION, 0, this);

    wxPoint pos = wxDefaultPosition;
    wxSize size(400, 450);
    if (m_window_width != 0 || m_window_height != 0) {
        size.SetWidth(
            wxMax(100, wxMin(m_window_width, ::wxGetDisplaySize().x)));
        size.SetHeight(
            wxMax(100, wxMin(m_window_height, ::wxGetDisplaySize().y)));
    }
    if (m_window_pos_x != 0 || m_window_pos_y != 0) {
        pos.x = wxMax(
            0, wxMin(m_window_pos_x, ::wxGetDisplaySize().x - m_window_width));
        pos.y = wxMax(
            0, wxMin(m_window_pos_y, ::wxGetDisplaySize().y - m_window_height));
    }

    m_pLauncherDialog = new LauncherUIDialog(
        this, m_parent_window, wxID_ANY, _("Launcher"), pos, size);
    m_pLauncherDialog->SetHideOnBtn(m_hide_on_btn);
    m_pLauncherDialog->CreateButtons(m_alauncher_labels, m_alauncher_commands);
    m_pLauncherSettingsDialog = new LauncherSettingsDialog(m_parent_window);
    m_pLauncherSettingsDialog->SetItems(
        m_alauncher_labels, m_alauncher_commands, m_hide_on_btn);

    return (WANTS_CURSOR_LATLON | WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL
        | WANTS_CONFIG | WANTS_NMEA_EVENTS | WANTS_PREFERENCES);
}

bool launcher_pi::DeInit()
{
    if (m_pLauncherDialog) {
        m_pLauncherDialog->Close();
        delete m_pLauncherDialog;
        m_pLauncherDialog = NULL;
    }

    if (m_pLauncherSettingsDialog) {
        delete m_pLauncherSettingsDialog;
        m_pLauncherSettingsDialog = NULL;
    }
    SaveConfig();
    return true;
}

int launcher_pi::GetAPIVersionMajor() { return MY_API_VERSION_MAJOR; }

int launcher_pi::GetAPIVersionMinor() { return MY_API_VERSION_MINOR; }

int launcher_pi::GetPlugInVersionMajor() { return PLUGIN_VERSION_MAJOR; }

int launcher_pi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }

wxBitmap* launcher_pi::GetPlugInBitmap() { return &m_logo; }

wxString launcher_pi::GetCommonName() { return _T ( "Launcher" ); }

wxString launcher_pi::GetShortDescription()
{
    return _("Launcher PlugIn for OpenCPN");
}

wxString launcher_pi::GetLongDescription()
{
    return _("Launcher PlugIn for OpenCPN\n\
Provides a simple configurable dialog to launch external programs.");
}

int launcher_pi::GetToolbarToolCount() { return 1; }

void launcher_pi::ShowPreferencesDialog(wxWindow* parent)
{
    if (m_pLauncherSettingsDialog->ShowModal() == wxID_OK) {
        m_alauncher_labels = m_pLauncherSettingsDialog->GetLabels();
        m_alauncher_commands = m_pLauncherSettingsDialog->GetCommands();
        m_hide_on_btn = m_pLauncherSettingsDialog->GetHideOnBtn();
        SaveConfig();
        wxPoint pos = m_pLauncherDialog->GetPosition();
        wxSize size = m_pLauncherDialog->GetSize();
        m_pLauncherDialog->Hide();
        delete m_pLauncherDialog;
        m_pLauncherDialog = new LauncherUIDialog(
            this, m_parent_window, wxID_ANY, _("Launcher"), pos, size);
        m_pLauncherDialog->SetHideOnBtn(m_hide_on_btn);
        m_pLauncherDialog->CreateButtons(
            m_alauncher_labels, m_alauncher_commands);
    }
}

void launcher_pi::OnToolbarToolCallback(int id)
{
    SetToolbarItemState(id, false);
    m_pLauncherDialog->Show();
}

void launcher_pi::SetCursorLatLon(double lat, double lon)
{
    // TODO: We will perhaps implement some kind of variable substitution in a
    // later version
}

bool launcher_pi::LoadConfig()
{
    wxFileConfig* pConf = (wxFileConfig*)m_pconfig;

    if (!pConf)
        return false;

    pConf->SetPath(_T ( "/PlugIns/LAUNCHER" ));

    m_launcher_labels = pConf->Read(_T ( "Labels" ), wxEmptyString);
    m_launcher_commands = pConf->Read(_T ( "Commands" ), wxEmptyString);
    m_hide_on_btn = pConf->Read(_T ( "HideOnBtn" ), true);
    m_window_width = pConf->Read(_T ( "Width" ), 0l);
    m_window_height = pConf->Read(_T ( "Height" ), 0l);
    m_window_pos_x = pConf->Read(_T ( "PosX" ), 0l);
    m_window_pos_y = pConf->Read(_T ( "PosY" ), 0l);

    if (m_launcher_labels != wxEmptyString
        || m_launcher_commands != wxEmptyString) {
        m_alauncher_labels = wxSplit(m_launcher_labels, ';', '\\');
        m_alauncher_commands = wxSplit(m_launcher_commands, ';', '\\');
    }

    for (size_t i = 1;
         i <= wxMax(m_alauncher_labels.Count(), m_alauncher_commands.Count());
         i++) {
        if (m_alauncher_labels.Count() < i) {
            m_alauncher_labels.Add(wxString::Format("Unknown %d", i));
        }
        if (m_alauncher_commands.Count() < i) {
            m_alauncher_commands.Add(wxString::Format("unknown%d", i));
        }
    }

    return true;
}

bool launcher_pi::SaveConfig()
{
    wxFileConfig* pConf = (wxFileConfig*)m_pconfig;

    if (!pConf)
        return false;

    pConf->SetPath(_T ( "/PlugIns/LAUNCHER" ));

    m_launcher_labels = wxJoin(m_alauncher_labels, ';', '\\');
    m_launcher_commands = wxJoin(m_alauncher_commands, ';', '\\');

    pConf->Write(_T ( "Labels" ), m_launcher_labels);
    pConf->Write(_T ( "Commands" ), m_launcher_commands);
    pConf->Write(_T ( "HideOnBtn" ), m_hide_on_btn);
    pConf->Write(_T ( "Width" ), m_window_width);
    pConf->Write(_T ( "Height" ), m_window_height);
    pConf->Write(_T ( "PosX" ), m_window_pos_x);
    pConf->Write(_T ( "PosY" ), m_window_pos_y);

    return true;
}

void launcher_pi::SetColorScheme(PI_ColorScheme cs)
{
    DimeWindow(m_pLauncherDialog);
}

void launcher_pi::SetPositionFix(PlugIn_Position_Fix& pfix)
{
    m_pLauncherDialog->SetPositionFix(pfix.Lat, pfix.Lon, pfix.Cog, pfix.Sog,
        pfix.Var, pfix.FixTime, pfix.nSats);
}
