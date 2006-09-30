
#ifndef _PHYS_PROVISORIUM_H_
#define _PHYS_PROVISORIUM_H_

/*

Provizorni fyzika

seznam oblasti, kudy se muze chodit
seznam cvercu a kruhu, kde se nemue stavet a kde se nemuze chodit


*/

class Path;
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

struct Point
{
	float x,y;
	Point() {}
	Point(float X, float Y) { x=X;y=Y; }
	Point(const Point &p) { x=p.x;y=p.y; }
};

struct TPolygon
{
	Point max;
	Point min;
	HoeGame::PtrList<Point> points;
	void End();
	bool IsSkrz(Point from, Point to);
	bool ToLeft(Point &from, Point &to, Point &res);
	bool ToRight(Point &from, Point &to, Point &res);
};

struct PointInt
{
	int x,y;
	PointInt() {}
	PointInt(int X, int Y) { x=X;y=Y; }
	PointInt(const PointInt &p) { x=p.x;y=p.y; }
	bool operator == (const PointInt &p) { return (x==p.x)&&(y==p.y); }
};

struct OkrajLine
{
	PointInt a;
	PointInt b;
	bool IsLine(PointInt &pnt)
	{
		if(a == pnt)
		{
			pnt = b;
			return true;
		}
		if(b == pnt)
		{
			pnt = a;
			return true;
		}
		return false;
	}
};


// 
struct TPathPart
{
	Point to;
	TPathPart * next;
	TPathPart * child;
	float distance;
	TPathPart * Find(Point from);
	void Copy(Path * path);
};

struct TPathBridge
{
	byte a;
	Point pa;
	byte b;
	Point pb;
};

class PhysGroup
{
	byte id;
	HoeGame::PtrSet<TPolygon*> m_poly;
	HoeGame::PtrSet<Obdelnik*> m_obd;
	HoeGame::PtrSet<Kruh*> m_kruh;
	TPolygon * CreatePolygon(HoeGame::PtrSet<OkrajLine> & lines);
	TPolygon *m_obal;

public:
	PhysGroup(byte id);
	HoeGame::PtrSet<TPolygon*> & GetPolygons() { return m_poly; }
	bool IsOk(const Obdelnik * o)
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
	}
	void Parse(BecherMap * map, byte * grids);
};

class Phys
{
protected:
	HoeGame::PtrSet<TPathBridge> m_bridges;
	static Phys m_instance;
	byte * m_gridmap;
	PhysGroup * m_groups[20];
public:
	static Phys * Get() { return &m_instance; }
	void ParseLevel(BecherMap * map, TerrainMiniMap * minimap);
	void ClearAll();
	TPathPart * Find(Point from, Point to);
	void Pisek(IHoeEnv::GridSurface * grid, int x, int y);
	void Pisek2(byte * grid, int x, int y, byte obl, byte vypln);
	void AddLines(HoeGame::PtrSet<OkrajLine> & lines, byte grp);
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







