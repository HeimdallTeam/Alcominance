
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "bechapp.h"
#include "buildings.h"
#include "obj_sugar.h"
#include "troll.h"
#include "../../HoeGame/include/hoe_benchmark.h"

//HoeGame::OffsetLoop off[] = { MAKELONG(25,0), MAKELONG(26,0),MAKELONG(0,0), MAKELONG(1,0), 0xffffffff, 0xffffffff };

HoeGame::Lang g_lang;
BecherResources g_resmgr;

IHoeResource * BecherResources::MissingResource(int id)
{
	const char * is = FindIDString(id);
	if (is)
		GetCon()->Printf("Resource id:%s requied..",is);
	else
		GetCon()->Printf("Resource id:%d requied..",id);
	return NULL;
}


BecherGame::BecherGame()
{
}

bool BecherGame::Init()
{
	if (!GetLua()->Init())
		return false;

	if (!GetLua()->Connect(GetEngine()))
		return false;
	if (!GetLua()->Connect(GetResMgr()))
		return false;
	if (!GetLua()->Connect(GetLang()))
		return false;
	if (!GetLua()->Connect(GetApp()->GetFS()))
		return false;
	GetLua()->AddFunc("AddButton",HUD::l_AddButton);
	GetLua()->AddFunc("ClearButtons",HUD::l_ClearButtons);
	GetLua()->AddFunc("info",HUD::l_info);
	GetLua()->AddFunc("AddTroll",BecherLevel::l_AddTroll);
	GetLua()->AddFunc("SetBuilding",BecherLevel::l_SetBuilding);
	GetLua()->AddFunc("PlaySound",BecherGame::l_PlaySound);
	GetLua()->AddFunc("AddCash",BecherLevel::l_AddCash);
	GetLua()->AddFunc("GetCash",BecherLevel::l_GetCash);
	GetLua()->AddFunc("Camera",BecherLevel::l_Camera);
	GetLua()->AddFunc("SetVar", CVar::l_setvar);
	GetLua()->AddFunc("GetVar", CVar::l_getvar);


	// load skript
	if (!GetLua()->Load("scripts/load.lua",g_luaconst))
		return false;

	if (!GetCon()->Load(GetEngine()))
		return false;
	GetCon()->RegisterCommands(GetEngine());

	//if (!LoadInfos())
	//	return false;
	GetEngine()->RegisterCmd("pvar", CVar::c_printvar, NULL); 
	GetEngine()->RegisterCmd("printvars", CVar::c_printallvars, NULL); 
	GetEngine()->RegisterCmd("set", CVar::c_setvar, NULL); 
	GetEngine()->RegisterCmd("map", BecherGame::c_map, NULL); 

	m_music.Init(GetCon());

	//m_music.Load("sound/music/rozdelane(bg4).XM");
	//m_music.Load("sound/music/stavbapomniku.XM");
	//m_music.setOffsets(off);
	//m_music.Play();
	//m_music.setFactor(0);

	return true;
}

void BecherGame::Destroy()
{
	m_music.Destroy();
}

bool BecherGame::LoadLevel(const char * fpath)
{
	CloseLevel();

	// vymazat luu

	if (!GetLua()->Load("scripts/main.lua",g_luaconst))
		return false;
	if (!GetLua()->Load("scripts/sugar.lua",g_luaconst))
		return false;
	if (!GetLua()->Load("scripts/alco.lua",g_luaconst))
		return false;
	if (!GetLua()->Load("scripts/store.lua",g_luaconst))
		return false;
	if (!GetLua()->Load("scripts/factory.lua",g_luaconst))
		return false;
	if (!GetLua()->Load("scripts/mine.lua",g_luaconst))
		return false;

	// benchamrk
	// LuaBenchmark();

	if (!m_level.LoadGame(fpath))
		return false;
		// set scene
	//if (m_scene)
	//	delete m_scene;
	//m_level->Create(GetEngine()->CreateScene(HOETS_GRAPH));

	GetApp()->SetScene(&m_level);
	m_level.Start();
	return true;
}

void BecherGame::CloseLevel()
{
}

int BecherGame::l_PlaySound(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1,"n", "PlaySound"))
	{
		IHoeSound* s = ::GetResMgr()->Get<IHoeSound>(lp.GetNum(-1));
		if (s)
			s->Play(false);
		else
			lp.Error("sound ID: %d not loaded.",lp.GetNum(-1)); 
		return 0;
	}

	return 0;
}

int BecherGame::c_map(int argc, const char * argv[], void * param)
{
	if (argc<2)
	{
		GetCon()->Printf("Bad params in command map.");
		return 1;
	}
	GetCon()->Printf("Load map %s", argv[1]);
	GetBecher()->LoadLevel(argv[1]);
	return 0;
}















