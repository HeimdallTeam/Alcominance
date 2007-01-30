
#include "StdAfx.h"
#include "map.h"
#include "terrain.h"

#ifndef BECHER_EDITOR
#define ADD_SUR(index, name, width, height, keys, icon, tt) index, name, width, height, 
#else
#define ADD_SUR(index, name, width, height, keys, icon, tt) index, name, width, height, keys, icon, tt,
#include "../resource/maleikony/trava.xpm"
#include "../resource/maleikony/ter_trava.xpm"
#include "../resource/maleikony/ter_trava-tmava.xpm"
#include "../resource/maleikony/ter_hlina.xpm"
#include "../resource/maleikony/ter_pisek.xpm"
#include "../resource/maleikony/City_Dirt.xpm"
#include "../resource/maleikony/city_squaretiles.xpm"
#include "../resource/maleikony/cliff1.xpm"
#include "../resource/maleikony/concrete.xpm"
#include "../resource/maleikony/metalplatecliff.xpm"
#include "../resource/maleikony/metalplatetiles.xpm"

	//b = new wxBitmapButton(sb,IDB_TEX1+1,wxBitmap(City_Dirt_xpm),BT_P(2,0),BT_SIZE/*,BS_FLAT*/);
	//b = new wxBitmapButton(sb,IDB_TEX1+2,wxBitmap(City_SquareTiles_xpm),BT_P(3,0),BT_SIZE/*,BS_FLAT*/);
	//b = new wxBitmapButton(sb,IDB_TEX1+3,wxBitmap(Cliff1_xpm),BT_P(0,1),BT_SIZE/*,BS_FLAT*/);
	//b = new wxBitmapButton(sb,IDB_TEX1+4,wxBitmap(Concrete_xpm),BT_P(1,1),BT_SIZE/*,BS_FLAT*/);
	//b = new wxBitmapButton(sb,IDB_TEX1+5,wxBitmap(MetalPlateCliff_xpm),BT_P(2,1),BT_SIZE/*,BS_FLAT*/);
	//b = new wxBitmapButton(sb,IDB_TEX1+6,wxBitmap(MetalPlateTiles_xpm),BT_P(3,1),BT_SIZE/*,BS_FLAT*/);

static bool g_sur_8x4[] = {
	true, true, true, true,
	false, true, false, false,
	true, false, false, false,
	true, true, false, false,
	// 2
	false, false, false, true,
	false, true, false, true,
	true, false, false, true,
	true, true, false, true,
	// 3
	false, false, true, false,
	false, true, true, false,
	true, false, true, false,
	true, true, true, false,
	// 4
	false, false, true, true,
	false, true, true, true,
	true, false, true, true,
	true, true, true, true,
	// zadni pulka
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,

};

static bool g_sur_4x4[] = {
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
	true, true, true, true,
};

#endif

// pridavat pouze dalsi indexy!
TTerrainSurface g_ter_surfaces[] = {
	ADD_SUR(0, "ter_trava", 4, 4, g_sur_4x4, trava_xpm, "Zakladni trava")
	ADD_SUR(1, "ter_war3", 8, 4, g_sur_8x4,trava_xpm, "Trava z warcrafta")
	ADD_SUR(2, "ter_trava1", 8, 4, g_sur_8x4,ter_trava_xpm, "Trava 1")
	ADD_SUR(3, "ter_trava2", 8, 4, g_sur_8x4,trava_tmava_xpm, "Trava 2")
	ADD_SUR(4, "ter_trava3", 8, 4, g_sur_8x4,trava_tmava_xpm, "Trava 3")
	ADD_SUR(5, "ter_hlina", 8, 4, g_sur_8x4,hlina_xpm, "Hlina")
	ADD_SUR(6, "ter_pisek", 8, 4, g_sur_8x4,pisek_xpm, "Pisek")
	ADD_SUR(10, "utes_tex", 1, 1, 0, 0, 0)
	ADD_SUR(0, 0, 0, 0, 0, 0, 0)
};

void SetTerrainData(IHoeEnv::GridSurface * grid)
{
	for (int i=0; g_ter_surfaces[i].tex_name;i++)
	{
		grid->SetTexture(g_ter_surfaces[i].index, g_ter_surfaces[i].tex_name,
			g_ter_surfaces[i].width, g_ter_surfaces[i].height);
	}
}

#ifdef BECHER_EDITOR

void TTerrainSurface::GetKeyIndex(bool a, bool b, bool c, bool d, byte &x, byte &y)
{
	// najit key
	for (int i=x;i < width;i++)
	for (int j=y+1;j < height;j++)
	{
		bool * p = k + ((i*height+j)*4);
		if (p[0]==a && p[1]==b && p[2]==c && p[3]==d)
		{
			x = i;
			y = j;
			return;
		}
	}
	for (int i=0;i < width;i++)
	for (int j=0;j < height;j++)
	{
		bool * p = k + ((i*height+j)*4);
		if (p[0]==a && p[1]==b && p[2]==c && p[3]==d)
		{
			x = i;
			y = j;
			return;
		}
	}
	x = 0;
	y = 0;
}

void TTerrainSurface::GetKey(bool &a, bool &b, bool &c, bool &d, byte x, byte y)
{
	// najit key
	bool * p = k + ((x*height+y)*4);
	a = p[0];
	b = p[1];
	c = p[2];
	d = p[3];
}

void SetTerrainGrid(BecherMap * map,int x, int y, bool a, bool b, bool c, bool d, int index)
{
	IHoeEnv::GridSurface * grid = map->GetTerrain();
	IHoeEnv::GridSurface::TGridDesc desc;
	if (x < 0 || y < 0 || x >= map->m_numX || y >= map->m_numY)
		return;
	bool aa=true,bb=true,cc=true,dd=true;
	grid->GetGridDesc(x,y,&desc);
	if (desc.tex1 != g_ter_surfaces[index].index)
	{
		// pro levej
		if (desc.tex2 == 0xff || desc.tex2 != g_ter_surfaces[index].index)
		{
			// nastavit priznak
			desc.tex2 = g_ter_surfaces[index].index;
		}
		else // nejaka textura tam uz je 
		{
			g_ter_surfaces[index].GetKey(aa, bb, cc, dd, desc.x2, desc.y2);
			a = a || aa;b = b || bb;c = c || cc;d = d || dd;
		}
		g_ter_surfaces[index].GetKeyIndex(a, b, c, d, desc.x2, desc.y2);
		g_ter_surfaces[index].GetKey(aa, bb, cc, dd, desc.x2, desc.y2);
		if (aa && bb && cc && dd)
		{
			desc.x1 = desc.x2;
			desc.y1 = desc.y2;
			desc.tex1 = desc.tex2;
			desc.tex2 = 0xff;
		}
	}
	else
	{
		g_ter_surfaces[index].GetKeyIndex(true, true, true, true, desc.x1, desc.y1);
	}
	// pokud vsechny 4 tak propadnout na dvojku
	// zjistit zpetnym dotazem

	grid->SetGridDesc(x,y,&desc);
}

void SetTerrainTexture(BecherMap * map, int x, int y, int index)
{
	// zkontrolovat zda neni mimo

	// nastavit prizrak
	// udelat pro vsechny 4 a nastavit hlavni texture, texturu
	SetTerrainGrid(map, x-1,y-1,false, true, false, false, index);
	SetTerrainGrid(map, x,y-1,true, false, false, false, index);
	SetTerrainGrid(map, x-1,y,false, false, false, true, index);
	SetTerrainGrid(map, x,y,false, false, true, false, index);
	map->GetTerrain()->Load();
}

	/*m_terrain->SetTexture(0, "trava", 4, 4);
	m_terrain->SetTexture(1, "ter_war3", 8, 4);
	m_terrain->SetTexture(2, "City_Dirt", 8, 4);
	m_terrain->SetTexture(3, "City_SquareTiles", 4, 4);
	m_terrain->SetTexture(4, "Cliff1", 4, 4);
	m_terrain->SetTexture(5, "Concrete", 4, 4);
	m_terrain->SetTexture(6, "MetalPlateCliff", 4, 4);
	m_terrain->SetTexture(7, "MetalPlateTiles", 4, 4);

	m_terrain->SetTexture(10, "utes_tex", 1, 1);*/

#endif

