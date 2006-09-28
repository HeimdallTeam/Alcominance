
#ifndef _BECHER_TERRAIN_H_
#define _BECHER_TERRAIN_H_

// co definovat u kazdeho povrchu?
// index (uklada se do mapy)
// jmeno textury
// sirka gridu
// vyska gridu
// jestli muze byt 
struct TTerrainSurface
{
	int index;
	const char * tex_name;
	int width;
	int height;
#ifdef BECHER_EDITOR
	bool * k;
	char ** icon;
	const char * tt;
	void GetKeyIndex(bool a, bool b, bool c, bool d, byte &x, byte &y);
	void GetKey(bool &a, bool &b, bool &c, bool &d, byte x, byte y);
#endif
};

extern TTerrainSurface g_ter_surfaces[];

// u modelu terenu
// model id
// model name
// vrchni textura

struct TTerrainModel
{

};

// funkce pro teren
void SetTerrainData(IHoeEnv::GridSurface * grid);
void SetTerrainTexture(BecherMap * map, int x, int y, int index);

#endif // _BECHER_TERRAIN_H_

