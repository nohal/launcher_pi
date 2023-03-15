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

#ifndef __LAUNCHERSETTINGSDIALOG_H__
#define __LAUNCHERSETTINGSDIALOG_H__

#include <wx/arrstr.h>
#include <wx/artprov.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/colour.h>
#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/intl.h>
#include <wx/listctrl.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/string.h>

///////////////////////////////////////////////////////////////////////////

class LauncherItemDialog;

///////////////////////////////////////////////////////////////////////////////
/// Class LauncherSettingsDialog
///////////////////////////////////////////////////////////////////////////////
class LauncherSettingsDialog : public wxDialog {
private:
    LauncherItemDialog* m_itemdlg;

protected:
    wxListCtrl* m_listData;
    wxCheckBox* m_cbCloseOnButton;
    wxButton* m_bAdd;
    wxButton* m_bEdit;
    wxButton* m_bRemove;
    wxStdDialogButtonSizer* m_sdbSizer;
    wxButton* m_sdbSizerOK;
    wxButton* m_sdbSizerCancel;

    // Event handlers, overide them in your derived class
    void OnItemDeselected(wxListEvent& event);
    void OnItemSelected(wxListEvent& event);
    void OnAddClick(wxCommandEvent& event);
    void OnEditClick(wxCommandEvent& event);
    void OnRemoveClick(wxCommandEvent& event);

public:
    LauncherSettingsDialog(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Launcher Settings"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(400, 450),
        long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    ~LauncherSettingsDialog();
    void SetItems(const wxArrayString& labels, const wxArrayString& commands,
        const bool hide);
    wxArrayString GetLabels();
    wxArrayString GetCommands();
    bool GetHideOnBtn() { return m_cbCloseOnButton->GetValue(); }
};

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class LauncherItemDialog
///////////////////////////////////////////////////////////////////////////////
class LauncherItemDialog : public wxDialog {
private:
protected:
    wxStaticText* m_stLabel;
    wxTextCtrl* m_tLabel;
    wxStaticText* m_stCommand;
    wxTextCtrl* m_tCommand;
    wxStdDialogButtonSizer* m_sdbSizer;
    wxButton* m_sdbSizerOK;
    wxButton* m_sdbSizerCancel;

public:
    LauncherItemDialog(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Launcher Item"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(400, 250),
        long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    ~LauncherItemDialog();
    void SetValues(const wxString& label, const wxString& command);
    wxString GetLabel();
    wxString GetCommand();
};

#endif //__LAUNCHERSETTINGSDIALOG_H__
