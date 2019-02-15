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

#include "LauncherUIDialog.h"
#include <map>
///////////////////////////////////////////////////////////////////////////

LauncherUIDialog::LauncherUIDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
                                   long style)
    : wxDialog(parent, id, title, pos, size, style) {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* main_sizer;
    main_sizer = new wxBoxSizer(wxVERTICAL);

    m_scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
    m_scrolledWindow->SetScrollRate(5, 5);

    button_sizer = new wxBoxSizer(wxVERTICAL);

    m_scrolledWindow->SetSizer(button_sizer);
    m_scrolledWindow->Layout();
    m_hide_on_btn = false;
    button_sizer->Fit(m_scrolledWindow);
    main_sizer->Add(m_scrolledWindow, 1, wxALL | wxEXPAND, 5);

    this->SetSizer(main_sizer);
    this->Layout();

    this->Centre(wxBOTH);
}

void LauncherUIDialog::CreateButtons(const wxArrayString& labels, const wxArrayString& commands) {
    for (size_t i = 0; i < labels.Count(); i++) {
        AddButton(labels[i], commands[i]);
    }
    this->m_scrolledWindow->Layout();
    this->button_sizer->Fit(m_scrolledWindow);
    this->Layout();
}

void LauncherUIDialog::AddButton(const wxString& label, const wxString& command) {
    LauncherButton* m_bAction = new LauncherButton(m_scrolledWindow, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
    m_bAction->SetCommand(command);
    button_sizer->Add(m_bAction, 1, wxALL | wxEXPAND, 5);

    m_bAction->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LauncherUIDialog::OnBtnClick), NULL, this);
}

LauncherUIDialog::~LauncherUIDialog() {}

long TranslateKey(const wxString key) {
    std::map<wxString, wxKeyCode> keys{{"F1", WXK_F1}, {"F2", WXK_F2},   {"F3", WXK_F3},   {"F4", WXK_F4},
                                       {"F5", WXK_F5}, {"F6", WXK_F6},   {"F7", WXK_F7},   {"F8", WXK_F8},
                                       {"F9", WXK_F9}, {"F10", WXK_F10}, {"F11", WXK_F11}, {"F12", WXK_F12}};
    std::map<wxString, wxKeyCode>::iterator i = keys.find(key);
    if (i != keys.end()) {
        return i->second;
    }
    return WXK_ESCAPE;
}

void LauncherUIDialog::SendKbdEvents(const wxString cmd) {
    wxKeyEvent e;
    wxArrayString keys = wxSplit(cmd.AfterFirst(':'), ',', '\\');
    for (size_t i = 0; i < keys.Count(); i++) {
        wxString key = keys[i];
        if (key.Length() > 1 && key[0] == '!') {
            e.SetEventType(wxEVT_KEY_DOWN);
            key = key.AfterFirst('!');
        } else {
            e.SetEventType(wxEVT_KEY_UP);
        }
        if (key.Length() == 1) {
            e.m_keyCode = key[0];
        } else {
            e.m_keyCode = TranslateKey(key);
        }
        m_parent->ProcessWindowEvent(e);
    }
}

void LauncherUIDialog::OnBtnClick(wxCommandEvent& event) {
    LauncherButton* button = (LauncherButton*)event.GetEventObject();
    if (m_hide_on_btn) this->Hide();
    wxString cmd = button->GetCommand();
    if (cmd.StartsWith(_T("KBD:"))) {
        SendKbdEvents(cmd);
    } else {
        cmd.Replace(_T( "%BOAT_LAT%" ), wxString::Format(_T( "%f" ), m_Lat));
        cmd.Replace(_T( "%BOAT_LON%" ), wxString::Format(_T( "%f" ), m_Lon));
        cmd.Replace(_T( "%BOAT_SOG%" ), wxString::Format(_T( "%f" ), m_Sog));
        cmd.Replace(_T( "%BOAT_COG%" ), wxString::Format(_T( "%f" ), m_Cog));
        cmd.Replace(_T( "%BOAT_VAR%" ), wxString::Format(_T( "%f" ), m_Var));
        cmd.Replace(_T( "%BOAT_FIXTIME%" ), wxString::Format(_T( "%d" ), m_FixTime));
        cmd.Replace(_T( "%BOAT_NSATS%" ), wxString::Format(_T( "%d" ), m_nSats));
        wxExecute(cmd, wxEXEC_ASYNC);
    }
    event.Skip();
}

void LauncherUIDialog::SetPositionFix(double Lat, double Lon, double Cog, double Sog, double Var, time_t FixTime, int nSats) {
    m_Lat = Lat;
    m_Lon = Lon;
    m_Cog = Cog;
    m_Sog = Sog;
    m_Var = Var;
    ;
    m_FixTime = FixTime;
    m_nSats = nSats;
}
