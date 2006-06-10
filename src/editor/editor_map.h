
#ifndef _BECHER_EDITOR_MAP_H_
#define _BECHER_EDITOR_MAP_H_

#include "../map.h"

class BecherObject;

class EditorMap : public BecherMap
{
	static EditorMap * s_actmap;
protected:
	BecherObject * m_createobject;
	wxString m_mapfilepath;
	IHoeEnv::HeightMapSurface * m_terrain;
public:
	BecherObject * m_lockobject;
	EditorMap()
	{
		assert(s_actmap == NULL);
		s_actmap = this;
		m_createobject = NULL;
		m_lockobject = NULL;
		m_terrain = NULL;
	}
	~EditorMap()
	{
		assert(s_actmap != NULL);
		s_actmap = NULL;
	}
	void CreateNew();
	void AddNewObject(unsigned long type);
	BecherObject * GetCreatedObject() { return m_createobject; }
	static inline EditorMap * Get() { return s_actmap; }
	bool IsBuildMode() { return m_createobject != NULL; }
	void LeaveObject();
	void BuildObject();
	bool SaveMap();
	bool SaveMap(const wxString &path);
	void SelectObject(const int x, const int y);
	void SetFilePath(const wxString &path);
	wxString GetFilePath();
	wxString GetTitle();
	IHoeEnv::HeightMapSurface * GetTerrain() { return m_terrain; };
};

#endif // _BECHER_EDITOR_MAP_H_

