
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
    wxListBox* m_listfiles;
    wxListCtrl* m_listresources;*/
	wxSpinCtrl * m_sba;
	wxSpinCtrl * m_sbb;
	wxSpinCtrl * m_sbc;
	wxSpinCtrl * m_sbd;

	bool m_edit;
public:
	wxSpinCtrl * m_width;
	wxSpinCtrl * m_height;

	MapSettingsDialog(wxWindow* win, bool new_map);
	void ApplyChanges(EditorMap * map);
	//DECLARE_EVENT_TABLE()
};

#endif // _MAP_PROPERTIES_H_

