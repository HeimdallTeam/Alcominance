
#ifndef _BECHER_MAP_H_
#define _BECHER_MAP_H_

class BecherObject;

#include "id.h"

#define ID_CHUNK(a,b,c,d) ((a) | (b) << 8 | (c) << 16 | (d) << 24)
#define ID_BECHERFILE ID_CHUNK('b','m','a','p')
#define ID_BECHERVER 4

struct MapChunk
{
	unsigned long chunk;
	unsigned long ver;
	unsigned long reserved;
};

class BecherMapLoader : public HoeFileReader
{
	MapChunk chunk;
public:
	BecherMapLoader(XHoeFile * f) : HoeFileReader(f, 0)
	{
		chunk.chunk = 0;
	}
	const MapChunk & Chunk() { return chunk; }
	bool ReadNext() 
	{
		if (!this->Read(&chunk, sizeof(chunk)))
			chunk.chunk = 0;
		return chunk.chunk != 0;
	}
};

class BecherMap : public HoeGame::Strategy::Map
{
protected:
	// seznam objektu
	BecherObject * m_obj[1000]; // todo
	int m_numobj;
	unsigned long m_lastid;

	int FindObjIndex(BecherObject * bo);
	bool LoadObjects(BecherMapLoader & r);
	bool SaveObjects(HoeFileWriter & w);

	// teren
	IHoeEnv::GridSurface * m_terrain;
public:
	/*!!! dat mimo public */
	uint m_numX;
	uint m_numY;
	float m_sizeX;
	float m_sizeY;
	float m_distX;
	float m_distY;

	BecherMap();
	bool Load(BecherMapLoader & r, bool loadobj);
	IHoeScene * CreateScene();
	IHoeScene * GetScene() { return m_scene; }

	BecherObject * CreateObject(unsigned long type);
	static EObjType GetObjectClass(EObjType type);
	void AddObject(BecherObject * obj);
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

