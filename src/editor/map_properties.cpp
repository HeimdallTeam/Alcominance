
#include "../StdAfx.h"
#include "../becher.h"
#include "map_properties.h"
#include "editor_map.h"

enum
{
	ID_LIST_SCRIPT = 1,
	IDB_ADD,
	IDB_REMOVE,
	IDB_PREV,
	IDB_NEXT
};

BEGIN_EVENT_TABLE(MapSettingsDialog, wxPropertySheetDialog)
	EVT_LISTBOX(ID_LIST_SCRIPT, MapSettingsDialog::OnUpdateButtons)
	EVT_BUTTON(IDB_ADD, MapSettingsDialog::OnAdd)
	EVT_BUTTON(IDB_REMOVE, MapSettingsDialog::OnRemove)
	EVT_BUTTON(IDB_PREV, MapSettingsDialog::OnMoveScript)
	EVT_BUTTON(IDB_NEXT, MapSettingsDialog::OnMoveScript)
END_EVENT_TABLE()


MapSettingsDialog::MapSettingsDialog(wxWindow* win, EditorMap * map)
{
	m_edit = map != NULL;
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    Create(win, -1, _("Map Settings"), wxDefaultPosition, wxSize(400,400));
    CreateButtons(wxOK|wxCANCEL);
	
	// page terrain
	wxBookCtrlBase* notebook = GetBookCtrl();
	wxPanel* panel_g = new wxPanel(notebook, wxID_ANY);
	m_width = new wxSpinCtrl(panel_g, -1,"64", wxPoint(20,20),wxSize(50,20));
	m_width->SetRange(4,500);
	m_width->SetValue(map ? map->m_numX: 64);
	if (m_edit)
		m_width->Disable();
	m_width->SetToolTip(_("Map grid width."));
	m_height = new wxSpinCtrl(panel_g, -1,"64", wxPoint(80,20),wxSize(50,20));
	m_height->SetRange(4,500);
	m_height->SetValue(map ? map->m_numY: 64);
	if (m_edit)
		m_height->Disable();
	m_height->SetToolTip(_("Map grid height."));

	// page script
	wxPanel* panel_s = new wxPanel(notebook, wxID_ANY);
	// create ovladani
	m_scripts = new wxListBox(panel_s,ID_LIST_SCRIPT,  wxPoint(10,10),wxSize(100,100),0);
	// add to scripts
	m_saddtext = new wxTextCtrl(panel_s, wxID_ANY, "",wxPoint(10, 110), wxSize(100,20));  
	m_sadd = new wxButton(panel_s, IDB_ADD, _("Add..."),wxPoint(110, 110), wxSize(50,20));
	m_sremove = new wxButton(panel_s, IDB_REMOVE, _("Remove"),wxPoint(150, 110), wxSize(50,20));
	m_sprev = new wxButton(panel_s, IDB_PREV, _("Move prev"),wxPoint(110, 10), wxSize(50,20));
	m_snext = new wxButton(panel_s, IDB_NEXT, _("Move next"), wxPoint(170, 30), wxSize(50,20));
	UpdateButtons();

    notebook->AddPage(panel_g, _("Global"));
    notebook->AddPage(panel_s, _("Script"));
 
	if (m_edit)
	{
		wxPanel* panel_r = new wxPanel(notebook, wxID_ANY);
		m_sba = new wxSpinCtrl(panel_r, -1,"0", wxPoint(80,20),wxSize(50,20));
		m_sba->SetRange(-256,256);
		m_sba->SetValue(0);
		m_sba->SetToolTip(_("Map grid top."));
		m_sbb = new wxSpinCtrl(panel_r, -1,"0", wxPoint(80,160),wxSize(50,20));
		m_sbb->SetRange(-256,256);
		m_sbb->SetValue(0);
		m_sbb->SetToolTip(_("Map grid bottom."));
		m_sbc = new wxSpinCtrl(panel_r, -1,"0", wxPoint(20,80),wxSize(50,20));
		m_sbc->SetRange(-256,256);
		m_sbc->SetValue(0);
		m_sbc->SetToolTip(_("Map grid left."));
		m_sbd = new wxSpinCtrl(panel_r, -1,"0", wxPoint(160,80),wxSize(50,20));
		m_sbd->SetRange(-256,256);
		m_sbd->SetValue(0);
		m_sbd->SetToolTip(_("Map grid right."));
	    notebook->AddPage(panel_r, _("Terrain resize"));
	}
}

void MapSettingsDialog::ApplyChanges(EditorMap *map)
{
	// resize
	// nova velikost
	if (m_sba->GetValue() || m_sbb->GetValue() || m_sbc->GetValue() || m_sbd->GetValue())
		map->Resize(m_sba->GetValue(),m_sbb->GetValue(),m_sbc->GetValue(),m_sbd->GetValue());

}

void MapSettingsDialog::AddScriptFile(wxString file)
{
	m_scripts->Append(file);
}

void MapSettingsDialog::OnRemove(wxCommandEvent& event)
{
	int i = 0;
	while (i < m_scripts->GetCount())
	{
		if (m_scripts->IsSelected(i))
			m_scripts->Delete(i);
		else
			i++;
	}
	
	UpdateButtons();
}

void MapSettingsDialog::OnUpdateButtons(wxCommandEvent& event)
{
	UpdateButtons();
}

void MapSettingsDialog::UpdateButtons()
{
	for (int i=0;i < m_scripts->GetCount();i++)
	{
		if (m_scripts->IsSelected(i))
		{
			m_sremove->Enable(true);
			m_sprev->Enable(true);
			m_snext->Enable(true);
			// nahoru dolu
			return;
		}
	}
	m_sremove->Enable(false);
	m_sprev->Enable(false);
	m_snext->Enable(false);
}

void MapSettingsDialog::OnAdd(wxCommandEvent& event)
{
	/*wxString files = _("Hoe script Files(*.lua)|*.lua|All files(*.*)|*.*");
	wxFileDialog dialog(this,_("Add file..."),_T(""), _T(""), files, wxOPEN | wxHIDE_READONLY | wxFILE_MUST_EXIST | wxMULTIPLE);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxArrayString paths;
		dialog.GetPaths(paths);
		for (unsigned int i=0;i < paths.Count();i++)
		{
			//AddFile(HoeUtils::relativepath(m_becherdir->GetValue().c_str(),paths[i].c_str()).c_str());
			AddScriptFile(paths[i].c_str());
		}*/

	if (m_saddtext->GetValue() != "")
	{
		AddScriptFile(m_saddtext->GetValue());
		m_saddtext->SetValue("");
		UpdateButtons();
	}
}

void MapSettingsDialog::OnMoveScript(wxCommandEvent& event)
{
	
}

/////////////////////////////////////////
VarsPanel::VarsPanel(wxWindow * win)
: wxPanel(win,wxID_ANY,wxDefaultPosition, wxSize(400,300))
{
	// vytvorit tree a property
}



