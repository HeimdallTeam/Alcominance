
#ifndef _BECHER_EDITOR_MAP_H_
#define _BECHER_EDITOR_MAP_H_

#include "../map.h"

class EditorMap : public BecherMap
{
protected:
	wxString m_mapfilepath;
public:
	EditorMap()
	{
	}
	~EditorMap()
	{
	}
	void CreateNew(uint sizeX, uint sizeY);
	BecherSystemObject * CreateSystemObject(EObjType type);
	bool LoadMap(const wxString &path);
	bool SaveMap();
	bool SaveMap(const wxString &path);
	void SelectObject(const int x, const int y);
	void SetFilePath(const wxString &path);
	wxString GetFilePath();
	wxString GetTitle();
	void Resize(int top, int bottom, int left, int right);
};

#endif // _BECHER_EDITOR_MAP_H_

