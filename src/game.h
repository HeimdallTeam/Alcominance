
#ifndef _BECHER_GAME_H_
#define _BECHER_GAME_H_

#include "level.h"

class BecherResources : public HoeGame::ResourceMgr
{
public:
	virtual IHoeResource * MissingResource(int id);
};

class BecherGame
{
	HoeGame::HoeMusic_Module m_music;
	BecherLevel m_level;
	HoeGame::Lang m_lang;
	//IHoe3DEngine * g_engine = NULL;
	BecherResources m_resmgr;
	HoeGame::LuaScript m_lua;
public:
	BecherGame();
	bool Init();
	void Destroy();
	// key
	bool LoadLevel(const char * fpath);
	void CloseLevel();
	BecherLevel * GetLevel() { return &m_level; }
	HoeGame::Lang * GetLang() { return &m_lang; }
	//IHoe3DEngine * g_engine = NULL;
	BecherResources * GetResMgr() { return &m_resmgr; }
	HoeGame::LuaScript * GetLua() { return &m_lua; }

	static int l_PlaySound(lua_State * L);
	static int c_map(int argc, const char * argv[], void * param);
};

#endif // _BECHER_GAME_H_

