
#include "../StdAfx.h"
#include "map_properties.h"

MapSettingsDialog::MapSettingsDialog(wxWindow* win, bool new_map)
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    Create(win, -1, _("Map Settings"), wxDefaultPosition, wxSize(400,400));
    CreateButtons(wxOK|wxCANCEL);
	
	// page terrain
	wxBookCtrlBase* notebook = GetBookCtrl();
	wxPanel* panel_g = new wxPanel(notebook, wxID_ANY);
	wxSpinCtrl * sbx = new wxSpinCtrl(panel_g, -1,"64", wxPoint(20,20),wxSize(50,20));
	sbx->SetRange(8,256);
	sbx->SetValue(64);
	sbx->SetToolTip(_("Map grid width."));
	wxSpinCtrl * sby = new wxSpinCtrl(panel_g, -1,"64", wxPoint(80,20),wxSize(50,20));
	sby->SetRange(8,256);
	sby->SetValue(64);
	sby->SetToolTip(_("Map grid height."));

	// page script
	wxPanel* panel_s = new wxPanel(notebook, wxID_ANY);

    notebook->AddPage(panel_g, _("Global"));
    notebook->AddPage(panel_s, _("Script"));
 

}


