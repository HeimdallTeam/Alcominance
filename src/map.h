
#ifndef _BECHER_MAP_H_
#define _BECHER_MAP_H_

class BecherObject;

#include "id.h"

#define ID_BECHERFILE ('b' | 'm' << 8 | 'a' << 16 | 'p' << 24)
#define ID_BECHERVER 3

class BecherMap : public HoeGame::Strategy::Map
{
protected:
	struct FileHeader
	{
		unsigned long idfile;
		unsigned long ver;
		unsigned long numobjects;
	};

	// seznam objektu
	BecherObject * m_obj[1000]; // todo
	int m_numobj;
	unsigned long m_lastid;

	int FindObjIndex(BecherObject * bo);
	bool LoadObjects(HoeFileReader & r, int numobj, int ver);
	bool SaveObjects(HoeFileWriter & w);
public:
	BecherMap();
	bool Load(HoeFileReader & r, bool loadobj);
	BecherObject * CreateObject(unsigned long type);
	static EObjType GetObjectClass(EObjType type);
	void AddObject(BecherObject * obj);
	void DeleteObject(BecherObject * obj);
	void DeleteObject(int index);
	int GetNumObj() { return m_numobj; }
	BecherObject * GetObj(int index) { assert(index >= 0 && index < m_numobj); return m_obj[index];}
	BecherObject * GetObject(const int x, const int y);
	IHoeScene * GetScene() { return m_scene; }
	unsigned long GenObjectID() { return ++m_lastid; }
	void ComputeLastID();
public:
	// najde objekt podle id
	BecherObject * GetObjFromID(unsigned long id);
};

#endif // _BECHER_MAP_H_

