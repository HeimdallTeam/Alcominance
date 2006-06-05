
#include "../StdAfx.h"
#include "map_properties.h"

MapSettingsDialog::MapSettingsDialog(wxWindow* win)
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    Create(win, -1, _("Map Settings"), wxDefaultPosition, wxSize(400,400));
    CreateButtons(wxOK|wxCANCEL);

   
}


