
#ifndef _PHYS_PROVISORIUM_H_
#define _PHYS_PROVISORIUM_H_

/*

Provizorni fyzika

seznam oblasti, kudy se muze chodit
seznam cvercu a kruhu, kde se nemue stavet a kde se nemuze chodit


*/

struct TrollPath;
class BecherMap;
class TerrainMiniMap;

class Obdelnik
{
public:
	float width;
	float height;
	float radius;
	float angle;
	float x,y; // centr
};

struct Kruh
{
	float radius;
	float x,y;
};

// 
struct TPathPart
{
	HoeMath::Vector2 to;
	TPathPart * next;
	TPathPart * child;
	float distance;
	TPathPart * Find(HoeMath::Vector2 from);
};

struct TPathBridge
{
	byte a;
	HoeMath::Vector2 pa;
	byte b;
	HoeMath::Vector2 pb;
};

class PhysGroup
{
	byte id;
	HoeCore::Set<HoeMath::Polygon2*> m_poly;
	HoeCore::Set<Obdelnik*> m_obd;
	HoeCore::Set<Kruh*> m_kruh;
	HoeMath::Polygon2 *m_obal;

public:
	PhysGroup(byte id);
	HoeCore::Set<HoeMath::Polygon2*> & GetPolygons() { return m_poly; }
	/*bool IsOk(const Obdelnik * o)
	{
		for (size_t i=0;i < m_kruh.Count();i++)
		{
			const float lx = m_kruh[i]->x - o->x;
			const float ly = m_kruh[i]->y - o->y;
			const float r = m_kruh[i]->radius+o->radius;
			if (((lx*lx)+(ly*ly)) > (r*r))
				continue;

			// hodne blizko
		}
	}*/
	void Parse(BecherMap * map, byte * grids);
};

class Phys
{
protected:
	HoeCore::Set<TPathBridge> m_bridges;
	static Phys m_instance;
	byte * m_gridmap;
	PhysGroup * m_groups[20];
public:
	static Phys * Get() { return &m_instance; }
	void ParseLevel(BecherMap * map, TerrainMiniMap * minimap);
	void ClearAll();
	TPathPart * Find(HoeMath::Vector2 from, HoeMath::Vector2 to);
	bool IsWater(int x, int y);
	byte GetGroup(float x, float y);
	TPathBridge * FindBridge(byte a, byte b);
	PhysGroup * GetGroup(byte grp) { return m_groups[grp >> 4]; }
};

inline float Distance(float x, float y, float tx, float ty)
{
	x -= tx;
	y -= ty;
	return sqrt((x*x)+(y*y));
}

#endif // _PHYS_PROVISORIUM_H_







