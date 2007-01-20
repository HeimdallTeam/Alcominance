
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


BecherGame::BecherGame() : m_mem(200)
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
	GetLua()->AddFunc("SetBuilding",BecherLevel::l_SetBuilding);
	GetLua()->AddFunc("PlaySound",BecherGame::l_PlaySound);
	GetLua()->AddFunc("AddCash",BecherLevel::l_AddCash);
	GetLua()->AddFunc("GetCash",BecherLevel::l_GetCash);
	GetLua()->AddFunc("Camera",BecherLevel::l_Camera);
	GetLua()->AddFunc("SetVar", CVar::l_setvar);
	GetLua()->AddFunc("GetVar", CVar::l_getvar);
	GetLua()->AddFunc("GetInfo", l_GetInfo);
	GetLua()->AddFunc("SendMsg", l_SendMsg);
	GetLua()->AddFunc("GetMem", BecherGame::l_GetMem);
	GetLua()->AddFunc("SetMem", BecherGame::l_SetMem);


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
	if (!GetLua()->Load("scripts/construct.lua",g_luaconst))
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

int BecherGame::l_GetMem(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(3,"*s*", "GetMem"))
	{
		int id = 0;
		if (lp.IsPointer(-3))
			id = reinterpret_cast<BecherObject*>(lp.GetPointer(-3))->GetID();
		else if (lp.IsNum(-3))
			id = lp.GetNum(-3);
		else if (!lp.IsNil(-3))
			lp.Error("SetMem: first param must be handle."); 
		HoeGame::Mem::Item * i = GetBecher()->m_mem.Get(id, lp.GetString(-2), false);
		if (i)
		{
			if (i->type == HoeGame::Mem::EInt)
			{
				lp.PushNum(i->ivalue);
				return 1;
			}
		}
		lp.PushNum(lp.GetNum(-1));
		return 1;
	}

	return 0;
}

int BecherGame::l_SetMem(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	// handle
	if (lp.CheckPar(3,"*s*", "SetMem"))
	{
		int id = 0;
		if (lp.IsPointer(-3))
			id = reinterpret_cast<BecherObject*>(lp.GetPointer(-3))->GetID();
		else if (lp.IsNum(-3))
			id = lp.GetNum(-3);
		else if (!lp.IsNil(-3))
			lp.Error("SetMem: first param must be handle."); 
		HoeGame::Mem::Item * i = GetBecher()->m_mem.Get(id, lp.GetString(-2), true);
		// TODO vice hodnot
		if (lp.IsNum(-1))
			i->Set(lp.GetNum(-1));
		else
			i->Set(lp.GetString(-1));
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

// messages
static int level = 0;

int SendGameMsg(BecherObject * o, int msg, int par1, void * par2, uint npar2)
{
	if (o == NULL)
        return SendGameMsgId(0, msg, par1, par2, npar2);
	int ret=0;
	level++;
	GetCon()->Printf("L%d:Message %s to obj %d nump=%d", level, FindIDString(msg), o->GetID(), npar2);
	ret = o->GameMsg(msg, par1, par2, npar2);
	level--;
	return ret;
}

int SendGameMsgId(unsigned long id, int msg, int par1, void * par2, uint npar2)
{
	// find id
	if (id > 0 && id < IDMSG_ALL)
		return SendGameMsg(GetLevel()->GetObjFromID(id),msg,par1,par2,npar2);
	int ret=0;
	level++;
	// log message
	if (id==IDMSG_ALL)
	{
		GetCon()->Printf("L%d:Message %s to all nump=%d", level, FindIDString(msg), npar2);
		for (int i=0;i<GetLevel()->GetNumObj();i++)
		{
			GetLevel()->GetObj(i)->GameMsg(msg, par1, par2, npar2);
		}
	}
	else
	{
		GetCon()->Printf("L%d:Message %s to system nump=%d", level, FindIDString(msg), npar2);
		switch (msg)
		{
		case BMSG_Info:
			hoe_assert(par2 && npar2 == 1);
			GetLevel()->GetPanel()->GetInfo()->Add((const char *)par2);
			break;
		};
	}
	level--;
	return ret;
	
}

int l_GetInfo(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.GetNumParam() < 2)
	{
		lp.Error("GetInfo: function requie min 2 parameters.");
		return 0;
	}
	int par = lp.GetNumParam();
	BecherObject * bo = NULL;
	if (lp.IsPointer(-par))
		bo = (BecherObject*)(lp.GetPointer(-par));
	else if (lp.IsNum(-par))
		bo = GetLevel()->GetObjFromID(lp.GetNum(-par));
	else 
	{
		lp.Error("Bad handle parameter");
		return 0;
	}
	par--;
	for (int i=0;i < par;i++)
	{
		int info = 0;
		if (lp.IsNum(-par))
			info = bo->GetInfo(lp.GetNum(-par), NULL, 0);
		else if (lp.IsString(-par))
		{
			char buff[1024] = {0};
			strncpy(buff, lp.GetString(-par), sizeof(buff)-1);
			info = bo->GetInfo(BINFO_Custom, buff, sizeof(buff)-1);
		}
		else
			lp.Error("type info must be id or string");
		lp.PushNum(info);
	}
	return par;
}

const char * GetMsgParam(int msg)
{
	switch (msg)
	{
	case BMSG_Chief:
		// prvni parametr do par1 a zbytek jako stringy do pole
		return "1:s|*:[s*]"; 
	default:
		return NULL;
	};
};

static union MsgParam
{
	int d;
	const char * s;
	void * p;
} g_pars[20];;

int l_SendMsg(lua_State * L)
{
	// zjistit jak se ktery parametr ma ukladat, a ten prekonvertit z luy do pole
	// nebo do toho formatu, ktery vyzaduje zprava
	// podle stringu?
	// "0bsdd"
	HoeGame::LuaParam lp(L);
	if (lp.GetNumParam() < 2)
	{
		lp.Error("Function SendMsg requied min 2 parameters.");
		return 0;
	}
	unsigned long id=0;
	BecherObject * bo = NULL;
	int msg = 0;
	int par1 = 0;
	void * par2 = NULL;
	uint npar2 = 0;
	int par = -lp.GetNumParam();
	if (lp.IsPointer(par))
		bo = (BecherObject*)lp.GetPointer(par);
    else if (lp.IsNum(par))
		id = lp.GetNum(par);
	else if (!lp.IsNil(par))
	{
		lp.Error("SendMsg: First parameter must be handle.");
		return 0;
	}
	par++;
	msg = lp.GetNum(par++);

	const char * pp = GetMsgParam(msg);
	if (!pp)
	{
		lp.Error("Message %s cannot call from lua.", FindIDString(msg));
		return 0;
	}

	// najit pocet parametru
	while (1)
	{
		if (pp[0] == '*')
		{
			pp ++; break;
		}

		if (*pp >= '0' && *pp < '9')
		{
			int np = 0;
			while (*pp >= '0' && *pp < '9')
			{
				np = np * 10 + *pp - '0';pp++;
			}
			if (np == -par)
				break;

			while (*pp && *pp != '|') pp++;
			if (*pp == '|')
			{
				pp++; continue;
			}
		}
		lp.Error("Failed params in msg %s.", FindIDString(msg));
		return 0;
	}
	hoe_assert(*pp == ':' && "Bad format for lua SendMsg");
	pp++;
	// rozparsovat parametry podle stringu
	// vytvorit pole atd..
	// parse parameters
	if (*pp == '[')
	{
		pp++;
		par2 = g_pars;
		if (par < -10)
			par = -10;
		for (;par < 0;par++)
		{
			if (!*pp)
				break;
			switch (*pp)
			{
			case 's':
				g_pars[npar2++].s = lp.GetString(par);
				break;
			case 'd':
				g_pars[npar2++].d = lp.GetNum(par);
				break;
			case 'p':
				g_pars[npar2++].p = lp.GetPointer(par);
				break;
			case '_':
				par1 = lp.GetNum(par);
				break;
			default:
				hoe_assert(!"Bad format for lua SendMsg");
			};
			if (pp[1] != '*')
				pp++;
		}
	}
	else
	{
		for (;par < 0;par++)
		{
			if (!*pp)
				break;
			switch (*pp)
			{
			case 's':
				par2 = (void*)lp.GetString(par);npar2 = 1;
				break;
			case 'p':
				par2 = lp.GetPointer(par);npar2 = 1;
				break;
			case '_':
				par1 = lp.GetNum(par);
				break;
			default:
				hoe_assert(!"Bad format for lua SendMsg");
			};
			pp++;
		}
	}

	if (bo)
		SendGameMsg(bo, msg, par1, par2, npar2);
	else
		SendGameMsgId(id, msg, par1, par2, npar2);

	return 0;
}




















