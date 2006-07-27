
/**
   @file    map_properties.h
   @date    May 2006
   @version 1.0
   @brief   Dialog pro vlastnosti mapy.
*/

#ifndef _MAP_PROPERTIES_H_
#define _MAP_PROPERTIES_H_

class EditorMap;

class MapSettingsDialog : public wxPropertySheetDialog
{
	/*wxTextCtrl* m_becherdir;
    wxButton* m_browse;
    wxButton* m_add;
    wxButton* m_remove;
    wxButton* m_addres;
    wxButton* m_removeres;
    wxListCtrl* m_listresources;*/
	wxSpinCtrl * m_sba;
	wxSpinCtrl * m_sbb;
	wxSpinCtrl * m_sbc;
	wxSpinCtrl * m_sbd;

	// scripts
    wxListBox* m_scripts;
	wxTextCtrl* m_saddtext;
	wxButton * m_sadd;
	wxButton * m_sremove;
	wxButton * m_snext;
	wxButton * m_sprev;
	bool m_edit;
public:
	wxSpinCtrl * m_width;
	wxSpinCtrl * m_height;

	MapSettingsDialog(wxWindow* win, EditorMap * map);
	void OnAdd(wxCommandEvent& event);
	void OnRemove(wxCommandEvent& event);
	void AddScriptFile(wxString file);
	void OnMoveScript(wxCommandEvent& event);
	void UpdateButtons();
	void OnUpdateButtons(wxCommandEvent& event);

	void ApplyChanges(EditorMap * map);
	
	DECLARE_EVENT_TABLE()
};

#endif // _MAP_PROPERTIES_H_

