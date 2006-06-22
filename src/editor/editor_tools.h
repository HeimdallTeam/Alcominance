
#ifndef _BECHER_EDITOR_TOOLS_H_
#define _BECHER_EDITOR_TOOLS_H_

class ToolObjects : public wxChoicebook
{
public:
	ToolObjects(wxWindow * parent);
	void OnClick(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

class TerrainObject : public wxPanel
{
public:
	TerrainObject(wxWindow * parent);
	void OnClick(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

//////////////////////////////////////////
// provizorne
class TexturesConfig
{
public:
	bool Load(wxString path);
	virtual void AddTexture(wxString name, wxString texture, uint w, uint h) = 0;
	virtual void AddTextureField(wxString name, wxString texture, uint w, uint h) = 0;
};

class TexturesDialog : public wxDialog
{
protected:
	wxListCtrl * m_list;
public:
	TexturesDialog(wxWindow * parent);
};

#endif // _BECHER_EDITOR_TOOLS_H_

