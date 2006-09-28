
#ifndef _BECHER_MAP_H_
#define _BECHER_MAP_H_

class BecherObject;
class BecherSystemObject;
class BecherGameLoad;
class BecherGameSave;

#include "id.h"

class BecherMap : public HoeGame::Strategy::Map
{
protected:
	// seznam objektu
	HoeGame::PtrSet<BecherObject*> m_obj; // todo
	HoeGame::PtrSet<BecherSystemObject*> m_sysobj; ///< @todo zmenit styl
	unsigned long m_lastid;

	// systemove objekty
	
	// teren
	IHoeEnv::GridSurface * m_terrain;

	// nahravani a ukladani
	bool LoadObject(BecherGameLoad & r);
	bool SaveAllObjects(BecherGameSave & w);
public:
	/*!!! dat mimo public */
	uint m_numX;
	uint m_numY;
	float m_sizeX;
	float m_sizeY;
	float m_distX;
	float m_distY;

	BecherMap();
	bool Load(BecherGameLoad & r, bool savegame);
	IHoeScene * CreateScene();
	IHoeScene * GetScene() { return m_scene; }

	BecherObject * CreateObject(EObjType type);
	BecherSystemObject * CreateSystemObject(EObjType type);
	static EObjType GetObjectClass(EObjType type);
	void AddObject(BecherObject * obj);
	void AddSystemObject(BecherSystemObject * obj);
	void DeleteObject(BecherObject * obj);
	int GetNumObj() { return m_obj.Count(); }
	BecherObject * GetObj(int index) { return m_obj[index];}
	int GetNumSysObj() { return m_sysobj.Count(); }
	BecherSystemObject * GetSysObj(int index) { return m_sysobj[index];}
	BecherObject * GetObject(const int x, const int y);

	unsigned long GenObjectID() { return ++m_lastid; }
	void ComputeLastID();
	IHoeEnv::GridSurface * GetTerrain() { return m_terrain; };
	bool SetTerrainData();
public:
	// najde objekt podle id
	BecherObject * GetObjFromID(unsigned long id);
};

#endif // _BECHER_MAP_H_

