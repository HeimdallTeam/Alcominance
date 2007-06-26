
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
HoeCore::StringPool g_pool;

static CVar v_debug_msg("debug_msg", false, 0);

IHoeResource * BecherResources::MissingResource(int id)
{
	const char * is = FindIDString(id);
	if (is)
		GetCon()->Printf("Resource id:%s requied..",is);
	else
		GetCon()->Printf("Resource id:%d requied..",id);
	return NULL;
}


BecherGame::BecherGame() : m_mem(200), m_condlg(*GetCon())
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

	if (!m_condlg.Load(GetEngine()))
		return false;
	m_condlg.RegisterCommands(GetEngine());

	//if (!LoadInfos())
	//	return false;
	GetEngine()->RegisterCmd("pvar", CVar::c_printvar, NULL); 
	GetEngine()->RegisterCmd("printvars", CVar::c_printallvars, NULL); 
	GetEngine()->RegisterCmd("set", CVar::c_setvar, NULL); 
	GetEngine()->RegisterCmd("map", BecherGame::c_map, NULL); 
	GetEngine()->RegisterCmd("memstat", BecherGame::c_memstat, NULL); 

	m_music.Init(GetCon());

	m_music.Load((void*)"sound/music/trackBG2.XM");
	//m_music.Load("sound/music/stavbapomniku.XM");
	//m_music.setOffsets(off);
	m_music.Play();
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
	if (!GetLua()->Load("scripts/troll.lua",g_luaconst))
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
			lp.Error("GetMem: first param must be handle."); 
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
        if (lp.IsPointer(-1))
        {
            BecherObject * bo = reinterpret_cast<BecherObject*>(lp.GetPointer(-1));
            i->Set(bo->GetID());
        }
        else if (lp.IsNum(-1))
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

int BecherGame::c_memstat(int argc, const char * argv[], void * param)
{
	GetCon()->Printf("----- Mem stat -----");
	GetCon()->Printf("- Pool -");
    for (uint u=0;u < g_pool.GetKeys().Count();u++)
    {
        GetCon()->Printf("%s : %d", g_pool.GetKeys()[u].str, g_pool.GetKeys()[u].ref);
    }
    lua_State* L = ::GetLua()->GetLua();
	GetCon()->Printf("- Lua -");
    GetCon()->Printf("Size of stack %d", lua_gettop(L));
    GetCon()->Printf("Stack: %dKb rem %d", lua_gc (L,LUA_GCCOUNT,0), lua_gc (L,LUA_GCCOUNTB,0));
	GetCon()->Printf("--------------------");
	return 0;
}

// messages
static int level = 0;


void DbgMsg(int level, int msg, unsigned long id, int npar2)
{
    if (v_debug_msg.GetBool() == false)
        return;
    switch (msg)
    {
        case BMSG_CursorActive:
        case BMSG_CursorInactive:
        case BMSG_SelectPlace:
            return;
    default:
        break;
    };
    
    switch (id)
    {
    case 0:
        GetCon()->Printf("L%d:Message %s to system nump=%d", level, FindIDString(msg), npar2);
        break;
    case IDMSG_ALL:
        GetCon()->Printf("L%d:Message %s to all nump=%d", level, FindIDString(msg), npar2);
        break;
    default:
        GetCon()->Printf("L%d:Message %s to obj %d nump=%d", level, FindIDString(msg), id, npar2);
        break;
    };
}



int SendGameMsg(BecherObject * o, int msg, int par1, void * par2, uint npar2)
{
	if (o == NULL)
        return SendGameMsgId(0, msg, par1, par2, npar2);
	int ret=0;
	level++;
	DbgMsg(level, msg, o->GetID(), npar2);
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
	DbgMsg(level, msg, id, npar2);
	// log message
	if (id==IDMSG_ALL)
	{
		for (int i=0;i<GetLevel()->GetNumObj();i++)
		{
			GetLevel()->GetObj(i)->GameMsg(msg, par1, par2, npar2);
		}
	}
	else
	{
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

BecherObject * GetBecherHandle(HoeGame::LuaParam &lp, int par)
{
    BecherObject * bo = NULL;
	if (lp.IsPointer(par))
		bo = (BecherObject*)(lp.GetPointer(par));
	else if (lp.IsNum(par))
		bo = GetLevel()->GetObjFromID(lp.GetNum(par));
	else 
	{
		lp.Error("Bad handle parameter");
		return 0;
	}
    return bo;
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
		{
			info = bo->GetInfo(lp.GetNum(-par), NULL, 0);
            // dbg
            int idinfo = lp.GetNum(-par);
            if (v_debug_msg.GetBool())
			    GetCon()->Printf("info: %d=%s(%d,%s)", info, FindIDString(idinfo&0xffff0000), bo->GetID(),FindIDString(idinfo&0xffff));
		}
		else if (lp.IsString(-par))
		{
			char buff[1024] = {0};
			strncpy(buff, lp.GetString(-par), sizeof(buff)-1);
			info = bo->GetInfo(BINFO_Custom, buff, sizeof(buff)-1);
			GetCon()->Printf("info: %d=%s(%d)", info, buff, bo->GetID());
		}
		else
			lp.Error("type info must be id or string");
		lp.PushNum(info);
	}
	return par;
}

static union MsgParam
{
	int d;
	const char * s;
	void * p;
} g_pars[20];;

int ConvertTable(HoeGame::LuaParam &lp, int pos)
{
	size_t s = lua_objlen (lp.GetLua(), pos);
	for (int i=0;i < s;i++)
	{
		lua_rawgeti(lp.GetLua(), pos, i+1);
		// on stack
		switch (lp.GetType(-1))
		{
		case LUA_TNIL:
			g_pars[i].p = NULL;break;
		case LUA_TNUMBER:
			g_pars[i].d = lp.GetNum(-1);
			break;
		case LUA_TSTRING:
			g_pars[i].s = lp.GetString(-1); break;
		case LUA_TUSERDATA:
		case LUA_TLIGHTUSERDATA:
			g_pars[i].p = lp.GetPointer(-1); break;
		default:
			lp.Error("SendMsg: Cannot convert %s to struct item.",lp.GetTypeString(-1));
			return 0;
		};
		lua_pop(lp.GetLua(), 1);

	}
	return s;
}

int l_SendMsg(lua_State * L)
{
	// zjistit jak se ktery parametr ma ukladat, a ten prekonvertit z luy do pole
	HoeGame::LuaParam lp(L);
	if (lp.GetNumParam() < 2)
	{
		lp.Error("Function SendMsg requied min 2 parameters.");
		return 0;
	}
	if (lp.GetNumParam() > 4)
	{
		lp.Error("Too match parameters for SendMsg(id,msg,par1,par2)");
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

	// zpracovani prvniho parametru, pointer se prevede na id
	if (par < 0)
	{
		if (lp.IsPointer(par))
			par1 = reinterpret_cast<BecherObject*>(lp.GetPointer(par))->GetID();
		else if (lp.IsNum(par))
			par1 = lp.GetNum(par);
		else
		{
			lp.Error("SendMsg: Cannot convert par1 from %s to number.",lp.GetTypeString(par));
			return 0;
		}
		par++;
	}
	if (par < 0)
	{
		// konvert from all
		// cislo na pointer
		// string na string
		// pointer
		// tabulka na pole nebo strukturu
		switch (lp.GetType(par))
		{
		case LUA_TNIL:
			par2 = NULL;npar2 = 1; break;
		case LUA_TNUMBER:
			par2 = GetLevel()->GetObjFromID(lp.GetNum(par));npar2 = 1;
			if (par2 == NULL)
			{
				lp.Error("SendMsg: Cannot convert par2 from number to pointer.");
				return 0;
			}
			break;
		case LUA_TSTRING:
			par2 = (void*)lp.GetString(par); npar2 = 1;break;
		case LUA_TUSERDATA:
		case LUA_TLIGHTUSERDATA:
			par2 = lp.GetPointer(par); npar2 = 1;break;
		case LUA_TTABLE:
			// scan table
			par2 = g_pars; npar2 = ConvertTable(lp,par);break;
		default:
			lp.Error("SendMsg: Cannot convert par2 from %s to pointer.",lp.GetTypeString(par));
			return 0;
		};
	}
	int ret;
	if (bo)
		ret = SendGameMsg(bo, msg, par1, par2, npar2);
	else
		ret = SendGameMsgId(id, msg, par1, par2, npar2);
	lp.PushNum(ret);
	return 1;
}




















