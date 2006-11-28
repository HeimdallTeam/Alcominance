
#include "StdAfx.h"
#include "phys_prov.h"
#include "minimap.h"
#include "map.h"
#include "troll.h"

#define INDEX(x,y) ((y)*m_numX+(x))

Phys Phys::m_instance;

static byte * g_cache = NULL;
static uint m_numX;
static uint m_numY;
static float m_sizeX;
static float m_sizeY;
static float m_distX;
static float m_distY;

void Phys::ClearAll()
{
	// grupy kde jsou odstranit
	memset(m_groups, 0, sizeof(m_groups));
}

// skala = 1
// voda = 2
// ok = 3

void Phys::ParseLevel(BecherMap *map, TerrainMiniMap * minimap)
{
	// 
	/*m_numX = map->m_numX;
	m_numY = map->m_numY;
	m_sizeX = map->m_sizeX;
	m_sizeY = map->m_sizeY;
	m_distX = map->m_distX;
	m_distY = map->m_distY;
	// vytvorit mapu gridu
	m_gridmap = new byte[m_numX*m_numY];
	g_cache = new byte[m_numX*m_numY]; // o jedno vetsi

	memset(m_gridmap,0,sizeof(byte)*m_numX*m_numY);
	IHoeEnv::GridSurface * grid = map->GetTerrain();
	Pisek(grid, 0,0);
	// nejak vypiskovat
	for (int y=0;y < m_numY;y++)
	{
		for (int x=0;x < m_numX;x++)
		{
			if (m_gridmap[INDEX(x,y)]==0)
				m_gridmap[INDEX(x,y)]=1;
			if (m_gridmap[INDEX(x,y)]==2)
			{
				float min = 0.f;
				grid->GetAvgHeight(x,y,&min,NULL);
				if (min >= -15.f)
					m_gridmap[INDEX(x,y)] = 3;
			}
		}
	}

	// ted to rozdelit na ty oblasti!
	// 
	byte oblast = 0x10;
	for (int y=0;y < m_numY;y++)
	{
		for (int x=0;x < m_numX;x++)
		{
			if (m_gridmap[INDEX(x,y)]==3)
			{
				// pisek
				Pisek2(m_gridmap,x,y,oblast,3);
				m_groups[oblast>>4] = new PhysGroup(oblast);
				m_groups[oblast>>4]->Parse(map, m_gridmap);
				oblast+=0x10;
			}
		}
	}

	if (g_cache)
		delete g_cache;
#ifndef BECHER_EDITOR
	if (minimap)
		minimap->Load(map, m_gridmap);
#endif
		*/
}

TPathPart * Phys::Find(HoeMath::Vector2 from, HoeMath::Vector2 to)
{
	//
	TPathPart * main = new TPathPart;
	main->to = to;
	main->child = NULL;
	main->next = NULL;
	if (main = main->Find(from))
		return main;
	delete main;
	return NULL;
}

byte Phys::GetGroup(float x, float y)
{
	// prepocitat na souradnice
	const uint sx = (uint)((x+(m_sizeX*0.5f))/(m_distX));
	const uint sy = (uint)((y+(m_sizeY*0.5f))/(m_distY));
	if (sx < 0 || sx >= m_numX || sy < 0 || sy >= m_numY)
		return 0;
	return m_gridmap[sy*m_numY+sx] & 0xf0;
}

TPathBridge * Phys::FindBridge(byte a, byte b)
{
	// najit bridge
	for (uint i=0;i < m_bridges.Count();i++)
	{
		if ((m_bridges[i].a == a && m_bridges[i].b == b)
			|| (m_bridges[i].a == b && m_bridges[i].b == a))
			return &(m_bridges.Get(i));
	}
	return NULL;
}

bool Phys::IsWater(int x, int y) 
{ 
	return false;
	return m_gridmap[y*m_numX+x]==2; 
}


////////////////////////////////////////////
PhysGroup::PhysGroup(byte grp)
{
	id = grp;
}

void PhysGroup::Parse(BecherMap * map, byte * grids)
{/*
	int x,y;
	// najit vsechny gridy a vytvorit si polygon //
	memset(g_cache, 0, map->m_numX*map->m_numY);
	for (int i=0;i < (map->m_numX*map->m_numY);i++)
	{
		if (this->id == grids[i] & 0xf)
			g_cache[i] = 1;
	}
	byte pisk = 2;
	HoeCore::Set<OkrajLine> lines;
	OkrajLine line;
	uint m_numX = map->m_numX;
	uint m_numY = map->m_numY;
	for (y = 0,x=0;x < m_numX;x++) 
	{ 
		if (g_cache[INDEX(x,y)]==1)
		{
			line.a = PointInt(x,y);
			line.b = PointInt(x+1,y);
			lines.Add(line);
		}
	}
	for (x = 0,y=0;y < m_numY;y++)
	{ 
		if (g_cache[INDEX(x,y)]==1)
		{
			line.a = PointInt(x,y);
			line.b = PointInt(x,y+1);
			lines.Add(line);
		}
	}
	for (x=0,y = m_numY-1;x < m_numX;x++)
	{ 
		if (g_cache[INDEX(x,y)]==1)
		{
			line.a = PointInt(x,y+1);
			line.b = PointInt(x+1,y+1);
			lines.Add(line);
		}
	}
	for (x = m_numX-1,y=0;y < m_numY;y++)
	{ 
		if (g_cache[INDEX(x,y)]==1)
		{
			line.a = PointInt(x+1,y);
			line.b = PointInt(x+1,y+1);
			lines.Add(line);
		}
	}
	// scan oblasti
	for (y = 0,x=0;x < m_numX;x++) 
	{ 
		if (g_cache[INDEX(x,y)]==0)
		{
			Phys::Get()->Pisek2(g_cache, x, y, pisk, 0);
			Phys::Get()->AddLines(lines, pisk++);
		}
	}
	for (x = 0,y=0;y < m_numY;y++)
	{ 
		if (g_cache[INDEX(x,y)]==0)
		{
			Phys::Get()->Pisek2(g_cache, x, y, pisk, 0);
			Phys::Get()->AddLines(lines, pisk++);
		}
	}
	for (x=0,y = m_numY-1;x < m_numX;x++)
	{ 
		if (g_cache[INDEX(x,y)]==0)
		{
			Phys::Get()->Pisek2(g_cache, x, y, pisk, 0);
			Phys::Get()->AddLines(lines, pisk++);
		}
	}
	for (x = m_numX-1,y=0;y < m_numY;y++)
	{ 
		if (g_cache[INDEX(x,y)]==0)
		{
			Phys::Get()->Pisek2(g_cache, x, y, pisk, 0);
			Phys::Get()->AddLines(lines, pisk++);
		}
	}
	// okraj hotov
	// okraj from lines
	//lines to polygon
	m_obal = CreatePolygon(lines);

	// ostatni vnitrni hodnoty
	for (x=0;x < m_numX;x++)
		for (y=0;y < m_numY;y++)
		{
			if (g_cache[INDEX(x,y)]==0)
			{
				lines.SetCount(0);
				Phys::Get()->Pisek2(g_cache, x, y, pisk, 0);
				Phys::Get()->AddLines(lines, pisk);
				TPolygon * p = CreatePolygon(lines);
				assert(p);
				m_poly.Add(p);
				pisk++;
			}
		}
		*/
}

/////////////////////////////////////////////

TPathPart * TPathPart::Find(HoeMath::Vector2 from)
{
	// nejdriv zcheckovat jestli jsou ve stejne grupe
	// pokud ne, tak najit most a rozdelit na casti vcetne mostu
	// pokud ano hledat dal
	// check v terenu
	/*byte gfrom = Phys::Get()->GetGroup(from.x, from.y);
	byte gto = Phys::Get()->GetGroup(to.x, to.y);
	if (gfrom != gto)
	{
		// najit prestup
		// cestu do prestupu
		// cestu z prestupu
		TPathBridge * b = Phys::Get()->FindBridge(gfrom,gto);
		if (!b)
			return false;
		// bridge

	}
	// hledani v gfrom
	PhysGroup * g = Phys::Get()->GetGroup(gfrom);
	if (!g)
		return false;
	// nejdriv najit v fdsfsf
	// najit v polygonech
	// vzit seznam
	TPathPart * part = NULL;
	HoeCore::Set<TPolygon*> &poly = g->GetPolygons();
	// najit vsechny polygony a zjistit zda stoji v ceste
	{
		TPolygon* pp[50];
		int nump=0;
		for (uint i=0;i < poly.Count();i++)
		{
			if (poly[i]->IsSkrz(from, to))
			{
				// ukazatel na vlozeni prvniho
				TPathPart ** up = &part;
				HoeMath::Vector2 f = from;
				HoeMath::Vector2 r = f;
				while (poly[i]->ToRight(f, to, r))
				//for (int j=0;j<poly[i]->points.Count();j++)
				{
					*up = new TPathPart;
					//(*up)->to = poly[i]->points[j];//r;
					(*up)->to = r;
					f = r;
					(*up)->next = NULL;
					up = &((*up)->next); 
				}
				// zaradit nakonec
				(*up) = this;
				// obejit prvni
				return part;
				pp[nump++] = poly[i];
			}
		}
		// vsechny ktere jdou skrz, najit body vlevo, a vytvorit cestu
		// najit body vpravo a vytvorit cestu
		
	

	}*/

	return this;
}



