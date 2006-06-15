
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

#endif // _BECHER_EDITOR_TOOLS_H_

