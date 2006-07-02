
#ifndef _BECHER_EDITOR_MAP_H_
#define _BECHER_EDITOR_MAP_H_

#include "../map.h"

class EditorTerrain
{
protected:
	IHoeEnv::GridSurface * m_terrain;
public:
	void Set(IHoeEnv::GridSurface * ter) { m_terrain = ter; }
	IHoeEnv::GridSurface * Get() { return m_terrain; }
};

class EditorMap : public BecherMap
{
protected:
	wxString m_mapfilepath;
	EditorTerrain m_terrain;
public:
	EditorMap()
	{
		m_terrain.Set(NULL);
	}
	~EditorMap()
	{
	}
	void CreateNew();
	bool SaveMap();
	bool SaveMap(const wxString &path);
	void SelectObject(const int x, const int y);
	void SetFilePath(const wxString &path);
	wxString GetFilePath();
	wxString GetTitle();
	EditorTerrain * GetTerrain() { return &m_terrain; };
};

#endif // _BECHER_EDITOR_MAP_H_

