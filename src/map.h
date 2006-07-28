
#ifndef _BECHER_MAP_H_
#define _BECHER_MAP_H_

class BecherObject;
class BecherSystemObject;

#include "id.h"

class BecherMap : public HoeGame::Strategy::Map
{
protected:
	// seznam objektu
	BecherObject * m_obj[1000]; // todo
	int m_numobj;
	BecherSystemObject * m_sysobj[100]; ///< @todo zmenit styl
	int m_numsysobj;
	unsigned long m_lastid;

	// systemove objekty
	

	int FindObjIndex(BecherObject * bo);

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
	static EObjType GetObjectClass(EObjType type);
	void AddObject(BecherObject * obj);
	void AddSystemObject(BecherSystemObject * obj);
	void DeleteObject(BecherObject * obj);
	void DeleteObject(int index);
	int GetNumObj() { return m_numobj; }
	BecherObject * GetObj(int index) { assert(index >= 0 && index < m_numobj); return m_obj[index];}
	BecherObject * GetObject(const int x, const int y);

	unsigned long GenObjectID() { return ++m_lastid; }
	void ComputeLastID();
	IHoeEnv::GridSurface * GetTerrain() { return m_terrain; };
public:
	// najde objekt podle id
	BecherObject * GetObjFromID(unsigned long id);
};

#endif // _BECHER_MAP_H_

