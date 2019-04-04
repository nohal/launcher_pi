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

#include "LauncherSettingsDialog.h"

///////////////////////////////////////////////////////////////////////////

LauncherSettingsDialog::LauncherSettingsDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                                               const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizerSettings;
    bSizerSettings = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizerSettingsData;
    bSizerSettingsData = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizerSettings_inner;
    bSizerSettings_inner = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* bSizerList;
    bSizerList = new wxBoxSizer(wxVERTICAL);

    m_listData = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    bSizerList->Add(m_listData, 1, wxALL | wxEXPAND, 5);

    m_cbCloseOnButton = new wxCheckBox(this, wxID_ANY, _("Close window on action"), wxDefaultPosition, wxDefaultSize, 0);
    bSizerList->Add(m_cbCloseOnButton, 0, wxALL, 5);

    m_listData->InsertColumn(0, _("Label"), wxLIST_FORMAT_LEFT, 100);
    m_listData->InsertColumn(1, _("Command"), wxLIST_FORMAT_LEFT, 200);

    bSizerSettings_inner->Add(bSizerList, 1, wxEXPAND, 5);

    wxBoxSizer* bSizerButtons;
    bSizerButtons = new wxBoxSizer(wxVERTICAL);

    m_bAdd = new wxButton(this, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0);
    bSizerButtons->Add(m_bAdd, 0, wxALL, 5);

    m_bEdit = new wxButton(this, wxID_ANY, _("Edit"), wxDefaultPosition, wxDefaultSize, 0);
    bSizerButtons->Add(m_bEdit, 0, wxALL, 5);

    m_bRemove = new wxButton(this, wxID_ANY, _("Remove"), wxDefaultPosition, wxDefaultSize, 0);
    bSizerButtons->Add(m_bRemove, 0, wxALL, 5);

    bSizerSettings_inner->Add(bSizerButtons, 0, 0, 5);

    bSizerSettingsData->Add(bSizerSettings_inner, 1, wxEXPAND, 5);

    bSizerSettings->Add(bSizerSettingsData, 1, wxEXPAND, 5);

    m_sdbSizer = new wxStdDialogButtonSizer();
    m_sdbSizerOK = new wxButton(this, wxID_OK);
    m_sdbSizer->AddButton(m_sdbSizerOK);
    m_sdbSizerCancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
    m_sdbSizer->AddButton(m_sdbSizerCancel);
    m_sdbSizer->Realize();

    bSizerSettings->Add(m_sdbSizer, 0, wxALL | wxEXPAND, 5);

    this->SetSizer(bSizerSettings);
    this->Layout();

    this->Centre(wxBOTH);

    // Connect Events
    m_listData->Connect(wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler(LauncherSettingsDialog::OnItemDeselected), NULL,
                        this);
    m_listData->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(LauncherSettingsDialog::OnItemSelected), NULL, this);
    m_bAdd->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LauncherSettingsDialog::OnAddClick), NULL, this);
    m_bEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LauncherSettingsDialog::OnEditClick), NULL, this);
    m_bRemove->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LauncherSettingsDialog::OnRemoveClick), NULL, this);

    m_itemdlg = new LauncherItemDialog(this);

    m_bEdit->Enable(false);
    m_bRemove->Enable(false);
}

LauncherSettingsDialog::~LauncherSettingsDialog() {
    // Disconnect Events
    m_listData->Disconnect(wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler(LauncherSettingsDialog::OnItemDeselected), NULL,
                           this);
    m_listData->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(LauncherSettingsDialog::OnItemSelected), NULL,
                           this);
    m_bAdd->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LauncherSettingsDialog::OnAddClick), NULL, this);
    m_bEdit->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LauncherSettingsDialog::OnEditClick), NULL, this);
    m_bRemove->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LauncherSettingsDialog::OnRemoveClick), NULL, this);

    delete m_itemdlg;
    m_itemdlg = NULL;
}

void LauncherSettingsDialog::OnItemDeselected(wxListEvent& event) {
    m_bEdit->Enable(false);
    m_bRemove->Enable(false);
    event.Skip();
}

void LauncherSettingsDialog::OnItemSelected(wxListEvent& event) {
    m_bEdit->Enable();
    m_bRemove->Enable();
    event.Skip();
}

void LauncherSettingsDialog::OnAddClick(wxCommandEvent& event) {
    m_itemdlg->SetValues(wxEmptyString, wxEmptyString);
    if (m_itemdlg->ShowModal() == wxID_OK) {
        long itemIndex = m_listData->InsertItem(m_listData->GetItemCount(), m_itemdlg->GetLabel());
        m_listData->SetItem(itemIndex, 1, m_itemdlg->GetCommand());
        m_bEdit->Enable(false);
        m_bRemove->Enable(false);
    }
    event.Skip();
}

void LauncherSettingsDialog::OnEditClick(wxCommandEvent& event) {
    wxListItem row_info;
    wxString lbl;
    wxString cmd;

    row_info.m_itemId = m_listData->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    row_info.m_mask = wxLIST_MASK_TEXT;
    row_info.m_col = 0;
    m_listData->GetItem(row_info);
    lbl = row_info.m_text;

    row_info.m_col = 1;
    m_listData->GetItem(row_info);
    cmd = row_info.m_text;

    m_itemdlg->SetValues(lbl, cmd);
    if (m_itemdlg->ShowModal() == wxID_OK) {
        m_listData->SetItem(row_info.m_itemId, 0, m_itemdlg->GetLabel());
        m_listData->SetItem(row_info.m_itemId, 1, m_itemdlg->GetCommand());
    }
    event.Skip();
}

void LauncherSettingsDialog::OnRemoveClick(wxCommandEvent& event) {
    if (wxMessageBox(_("Do you really want to remove this launcher action?"), _("Really remove?"), wxYES_NO) == wxYES) {
        long itemIndex = -1;
        for (;;) {
            itemIndex = m_listData->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            if (itemIndex == -1) break;
            m_listData->DeleteItem(itemIndex);
            m_bEdit->Enable(false);
            m_bRemove->Enable(false);
        }
    }
}

void LauncherSettingsDialog::SetItems(const wxArrayString& labels, const wxArrayString& commands, const bool hide) {
    m_cbCloseOnButton->SetValue(hide);
    m_listData->DeleteAllItems();
    for (size_t i = 0; i < labels.Count(); i++) {
        m_listData->InsertItem(i, labels[i]);
        m_listData->SetItem(i, 1, commands[i]);
    }
}

wxArrayString LauncherSettingsDialog::GetLabels() {
    wxListItem row_info;
    wxArrayString lbl;

    for (int i = 0; i < m_listData->GetItemCount(); i++) {
        row_info.m_itemId = m_listData->GetNextItem(i - 1, wxLIST_NEXT_ALL);
        row_info.m_mask = wxLIST_MASK_TEXT;
        row_info.m_col = 0;
        m_listData->GetItem(row_info);
        lbl.Add(row_info.m_text);
    }
    return lbl;
}

wxArrayString LauncherSettingsDialog::GetCommands() {
    wxListItem row_info;
    wxArrayString cmd;

    for (int i = 0; i < m_listData->GetItemCount(); i++) {
        row_info.m_itemId = m_listData->GetNextItem(i - 1, wxLIST_NEXT_ALL);
        row_info.m_mask = wxLIST_MASK_TEXT;
        row_info.m_col = 1;
        m_listData->GetItem(row_info);
        cmd.Add(row_info.m_text);
    }
    return cmd;
}

///////////////////////////////////////////////////////////////////////////

LauncherItemDialog::LauncherItemDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                                       const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizerItem;
    bSizerItem = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizerData;
    bSizerData = new wxBoxSizer(wxVERTICAL);

    m_stLabel = new wxStaticText(this, wxID_ANY, _("Label"), wxDefaultPosition, wxDefaultSize, 0);
    m_stLabel->Wrap(-1);
    bSizerData->Add(m_stLabel, 0, wxALL, 5);

    m_tLabel = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    bSizerData->Add(m_tLabel, 0, wxALL | wxEXPAND, 5);

    m_stCommand = new wxStaticText(this, wxID_ANY, _("Command"), wxDefaultPosition, wxDefaultSize, 0);
    m_stCommand->Wrap(-1);
    bSizerData->Add(m_stCommand, 0, wxALL, 5);

    m_tCommand = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    bSizerData->Add(m_tCommand, 0, wxALL | wxEXPAND, 5);

    bSizerItem->Add(bSizerData, 1, wxEXPAND, 5);

    m_sdbSizer = new wxStdDialogButtonSizer();
    m_sdbSizerOK = new wxButton(this, wxID_OK);
    m_sdbSizer->AddButton(m_sdbSizerOK);
    m_sdbSizerCancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
    m_sdbSizer->AddButton(m_sdbSizerCancel);
    m_sdbSizer->Realize();

    bSizerItem->Add(m_sdbSizer, 0, wxALL | wxEXPAND, 5);

    this->SetSizer(bSizerItem);
    this->Layout();

    this->Centre(wxBOTH);
}

LauncherItemDialog::~LauncherItemDialog() {}

void LauncherItemDialog::SetValues(const wxString& label, const wxString& command) {
    m_tLabel->SetValue(label);
    m_tCommand->SetValue(command);
}

wxString LauncherItemDialog::GetLabel() { return m_tLabel->GetValue(); }

wxString LauncherItemDialog::GetCommand() { return m_tCommand->GetValue(); }
