
#ifndef _BECHER_EDITOR_MAP_H_
#define _BECHER_EDITOR_MAP_H_

#include "../map.h"

/*
class BecherObject; 

struct LevelEditObject
{
	int item;
	BecherObject * object;
};

class BecherEditorMap : public HoeLevel::BaseLevel, public BecherLevel
{
	int m_levelitem;
	std::string m_name;
	std::string m_path;
	IHoeScene * m_scene;
	std::vector<LevelEditObject> m_objs;
	HoeGame::Strategy::SelectObjContainer soc;
	BecherObject * m_mselect;

	// heightmap
	wxBitmap m_height;
	wxString m_heightpath;
	float m_mapRealX;
	float m_mapRealY;
	int m_mapGridX;
	int m_mapGridY;
	float m_mapScale;
public:
	BecherEditorMap();
	void OnSelectItem(int item, void * data);
	void SelectRoot();
	void OnChangeProperty(HoeExtern::IPropertyItem * item);
	bool Save(HoeExtern::IProjectSave *);
	bool Load(HoeExtern::IProjectLoad *);
	bool Open(const char * name, const char * path);
	virtual int Msg(HoePlugin::EPluginMsg msg, int par1, void * par2);
	void OnKeyDown(int key);
	bool CreateNew(const char * path);
	void NewSelectObject(unsigned long type);
	virtual void OnAddObject(HoeGame::Strategy::StgObject * obj);
	void AddToEditor(int item);
	LevelEditObject * FindItemObject(int item);
	LevelEditObject * FindItemObject(BecherObject * object);
	virtual void OnSelectObject(EObjType type, BecherObject* obj);
	void OnChangeObjProperty(HoeExtern::IPropertyItem * item, BecherObject * obj);
	static const char * GetTypeName(EObjType type);
	void RemoveItemObject(BecherObject * object);
	void RemoveSelections();
	const char * GetMapName() { return m_name.c_str(); }
	void Close();
	void LoadHeightMap(wxString & filename);
};

*/

class BecherObject;

class EditorMap : public BecherMap
{
	static EditorMap * s_actmap;
protected:
	BecherObject * m_createobject;
	wxString m_mapfilepath;
public:
	BecherObject * m_lockobject;
	EditorMap()
	{
		assert(s_actmap == NULL);
		s_actmap = this;
		m_createobject = NULL;
		m_lockobject = NULL;
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
};

#endif // _BECHER_EDITOR_MAP_H_

