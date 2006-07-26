
#include "../StdAfx.h"
#include "map_properties.h"
#include "editor_map.h"

MapSettingsDialog::MapSettingsDialog(wxWindow* win, bool new_map)
{
	m_edit = !new_map;
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    Create(win, -1, _("Map Settings"), wxDefaultPosition, wxSize(400,400));
    CreateButtons(wxOK|wxCANCEL);
	
	// page terrain
	wxBookCtrlBase* notebook = GetBookCtrl();
	wxPanel* panel_g = new wxPanel(notebook, wxID_ANY);
	m_width = new wxSpinCtrl(panel_g, -1,"64", wxPoint(20,20),wxSize(50,20));
	m_width->SetRange(8,256);
	m_width->SetValue(64);
	if (m_edit)
		m_width->Disable();
	m_width->SetToolTip(_("Map grid width."));
	m_height = new wxSpinCtrl(panel_g, -1,"64", wxPoint(80,20),wxSize(50,20));
	m_height->SetRange(8,256);
	m_height->SetValue(64);
	if (m_edit)
		m_height->Disable();
	m_height->SetToolTip(_("Map grid height."));

	// page script
	wxPanel* panel_s = new wxPanel(notebook, wxID_ANY);

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


