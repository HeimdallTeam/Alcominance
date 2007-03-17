
#ifndef _BECHER_MAP_H_
#define _BECHER_MAP_H_

class BecherObject;
class BecherSystemObject;
class BecherGameLoad;
class BecherGameSave;
class Addon;

#include "id.h"

class BecherMap : public HoeGame::Strategy::Map
{
protected:
	// seznam objektu
	HoeCore::Set<BecherObject*> m_obj; // todo	
    HoeCore::Set<BecherSystemObject*> m_sysobj; ///< @todo zmenit styl
    HoeCore::Set<Addon*> m_addon; // doplnky

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
	// nahraje chunk mapy
	virtual bool LoadMapChunk(BecherGameLoad & r);
	IHoeScene * CreateScene();
	IHoeScene * GetScene() { return m_scene; }

	BecherObject * CreateObject(EObjType type);
    Addon * CreateAddOnObject(EObjType type);
	BecherSystemObject * CreateSystemObject(EObjType type);
	static EObjType GetObjectClass(EObjType type);
	void AddObject(BecherObject * obj);
	void AddSystemObject(BecherSystemObject * obj);
    void AddAddonObject(Addon * obj);
	void DeleteObject(BecherObject * obj);
    void DeleteAddonObject(Addon * obj);
	void DeleteSystemObject(BecherSystemObject * obj);
	int GetNumObj() { return m_obj.Count(); }
	BecherObject * GetObj(int index) { return m_obj[index];}
	int GetNumSysObj() { return m_sysobj.Count(); }
	BecherSystemObject * GetSysObj(int index) { return m_sysobj[index];}
	int GetNumAddonObj() { return m_addon.Count(); }
	Addon * GetAddonObj(int index) { return m_addon[index];}
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

