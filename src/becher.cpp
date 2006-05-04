
#include "StdAfx.h"
#include "bechapp.h"
#include "becher.h"
#include "console.h"
#include "game.h"
#include "infos.h"

//////////////////////////////////////////////
BechApp * g_app = NULL;
HoeGame::Lang * g_lang = NULL;
IHoe3DEngine * g_engine = NULL;
HoeGame::ResourceMgr * g_resmgr = NULL;
HoeGame::LuaScript * g_lua = NULL;
BGConsole * g_con = NULL;
BecherGame * g_becher = NULL;

class BecherResources : public HoeGame::ResourceMgr
{
public:
	virtual IHoeResource * MissingResource(int id)
	{
		const char * is = FindIDString(id);
		if (is)
			GetCon()->Printf("Resource id:%s requied..",is);
		else
			GetCon()->Printf("Resource id:%d requied..",id);
		return NULL;
	}
};

bool InitBecher(BechApp * application)
{
	g_app = application;
	g_engine = GetApp()->GetEngine();

	HoeGetInfo(GetEngine())->LoadFont("resource/font.ttf",12); 
	
	g_lang = new HoeGame::Lang();
	g_resmgr = new BecherResources();

	// init lua
	g_lua = new HoeGame::LuaScript(GetApp()->GetConsole());
	if (!GetLua()->Init())
		return false;

	if (!GetLua()->Connect(GetEngine()))
		return false;
	if (!GetLua()->Connect(GetResMgr()))
		return false;
	if (!GetLua()->Connect(GetLang()))
		return false;
	if (!GetLua()->Connect(application->GetFS()))
		return false;
	GetLua()->AddFunc("AddButton",ControlPanel::l_AddButton);
	GetLua()->AddFunc("ClearButtons",ControlPanel::l_ClearButtons);
	GetLua()->AddFunc("info",InfoPanel::l_info);
	GetLua()->AddFunc("AddTroll",BecherGame::l_AddTroll);
	GetLua()->AddFunc("SetBuilding",BecherGame::l_SetBuilding);
	GetLua()->AddFunc("PlaySound",BecherGame::l_PlaySound);
	GetLua()->AddFunc("AddCash",BecherGame::l_AddCash);
	GetLua()->AddFunc("GetCash",BecherGame::l_GetCash);
	GetLua()->AddFunc("SetVar", CVar::l_setvar);
	GetLua()->AddFunc("GetVar", CVar::l_getvar);


	g_becher = new BecherGame();

	// load skript
	if (!GetLua()->Load("scripts/init.lua",g_luaconst))
		return false;

	if (!g_con->Load(GetEngine()))
		return false;
	g_con->RegisterCommands(GetEngine());

	//if (!LoadInfos())
	//	return false;
	GetEngine()->RegisterCmd("pvar", CVar::c_printvar, NULL); 
	GetEngine()->RegisterCmd("set", CVar::c_setvar, NULL); 
	GetEngine()->RegisterCmd("map", BecherGame::c_map, NULL); 

	return true;
}

/*bool Becher::Init()
{
	if (!m_view.Init(GetEngine()->GetActiveScene()))
		return false;
	m_view.SetTargetPosition(0,0);
	m_view.SetDistance(110.f);
	m_view.SetAngle(0);

	tup = new tTupon(0,0);

	return true;
}

bool t[4] = {0};

void Becher::KeyDown(int key)
{
	switch (key)
	{
	case HK_LEFT:
		t[0] = true;
		break;
	case HK_RIGHT:
		t[1] = true;
		break;
	case HK_UP:
		t[2] = true;
		break;
	case HK_DOWN:
		t[3] = true;
		break;
	case HK_GRAVE:
		g_bcon->Open();
		break;
	case HK_ESCAPE:
		GetEngine()->exec("quit");
		break;
	};
}

void Becher::KeyUp(int key)
{
	switch (key)
	{
	case HK_LEFT:
		t[0] = false;
		break;
	case HK_RIGHT:
		t[1] = false;
		break;
	case HK_UP:
		t[2] = false;
		break;
	case HK_DOWN:
		t[3] = false;
		break;

	};
}

void Becher::Update(float dtime)
{
	if (t[0])
		m_view.Rotate(dtime * 2);
	if (t[1])
		m_view.Rotate(-dtime * 2);
	if (t[3])
	{
		float d = m_view.GetDistance();
		d += dtime * 35;
		if (d < 400)
			m_view.SetDistance(d);
	}

	if (t[2])
	{
		float d = m_view.GetDistance();
		d -= dtime * 35;
		if (d > 30)
			m_view.SetDistance(d);
	}


	if (g_bcon->IsActive())
	{
		g_bcon->Update(dtime);
	}
}

void Becher::OnPaint()
{
	if (g_bcon->IsActive())
		g_bcon->Draw(Get2D());
}

void Becher::OnSet()
{
	//m_view.Activate();
	HoeGetInput(GetEngine())->RegisterKeyboard(this); 
	GetEngine()->GetActiveScene()->Set2DCallback(this);
}



/*

bool Becher::Init(iHoe3DEngine * hoe)
{
	g_becher = this;
	m_state = sInit;
	m_hoe3D = hoe;
	m_phys = (iHoePhysics *)hoe->Create("physics");
	assert(m_phys);
	m_build = NULL;
	m_select = NULL;

	m_hoe3D->Set2DCallback(Becher::Draw);

	m_mloader = NULL;

	LuaInit();
	LoadScript("init.lua");

	// init sound

	if (!InitInput())
		return false;

	if (!m_view.Init())
		return false;

	return true;
}

bool Becher::LoadLevel(const char * mapname)
{
	assert(m_state != sRun);

	// inicializace nahravani
	this->m_state = sLoad;

	m_mloader = new MapLoader;

	m_map = m_mloader->LoadMap(mapname);

	if (!m_ctlpanel.Init())
		return false;

	if (!m_info.Init())
		return false;

	delete m_mloader;
	m_mloader = NULL;

	this->m_state = sRun;

	return true;
}

bool Becher::GoMenu()
{
	assert(m_state != sMenu);

	if (m_state == sRun)
	{
		// odnahrat
	}

	// zinicializovat menu
	m_state = sMenu;

	return true;
}

void Becher::Update(const float time)
{
	m_timeJump = time;
	// move cursor
	static int lx = -1;
	static int ly = -1;

	if (lx != m_view.curx || ly != m_view.cury)
	{
		m_ctlpanel.Move(m_view.curx,m_view.cury);
		lx = m_view.curx;
		ly = m_view.cury;
	}

	switch (m_state)
	{
	case sRun:
		{
			m_view.Update(time);
			
			m_obj.UpdateAll();

			// mel by spis zjistit kurzor a nastavit novy souradnice
			if (m_build)
				m_build->Update();

		}
		break;
	};

}

void Becher::UnloadMap()
{

}

void Becher::BuildOK()
{
	if (m_build) // and build ok
	{
		if (m_build->IsPlaceOK())
		{
			m_build->Contruct(0,0);
			m_obj.AddStatic(m_build);

			// add job
			if (m_select && m_select->GetTypeID() == ID_TUPON)
			{
				tTupon* t = reinterpret_cast<tTupon*>(m_select);
				float max;
				float *p;
				t->jobs.Reset();
				p = m_build->GetPos(max);
				t->jobs.Go(p[0],p[2]);
				t->jobs.Msg("Jsem tu sefe..");
				t->jobs.Build(reinterpret_cast<tBuilding*>(m_build));
				t->jobs.Msg("Hotovo!");
				t->jobs.Idle();
			}
			m_build = NULL;

		}
		// jinak by mel zahrat varovnej zvuk..
	}
}

void Becher::BuildCancel()
{
	if (m_build)
		m_build->Delete();
	m_build = NULL;
}


void Becher::KeyDown(int key)
{
	float x,y;
	switch (key)
	{
	case HK_RETURN:
		this->BuildOK();
		break;
	case HK_C:
		this->BuildCancel();
		break;
	case HK_W:
		Becher::GetView()->GetPick(0,x,y);
		new tStrom(x,y);
		break;
	case HK_B:
		Becher::GetView()->GetPick(0,x,y);
		this->GetObjects()->AddDynamic( new tTupon(x,y));
		break;
	};
}

///////////////////////////////////////////

int Becher::Draw(iHoe2D * hoe2d)
{
	Becher * b = Becher::GetBecher();

	switch (b->m_state)
	{
	case sRun:
		b->m_ctlpanel.Draw(hoe2d);
		b->m_info.Draw(hoe2d);
		if (b->m_select)
			b->m_select->Draw(hoe2d);
		break;
	case sTitule:
		b->DrawTitule(hoe2d);
		break;
	case sLoad:
		b->m_mloader->LoadingProgress(hoe2d);
		break;
	}

	return 0;
}

void Becher::LClick(float x, float y)
{
	// klik v becheru
	  // zkusit klik na panely
	if (this->m_ctlpanel.Click(x,y))
	{
		return;
	}

	if (m_build)
	{
		Becher::GetBecher()->BuildOK();
		return;
	}

	if (m_select = m_view.GetObject(x,y))
	{
		if (!lua_select(m_select->GetID()))
			m_select = NULL;
	}

}

void Becher::RClick(float x, float y)
{
	if (m_select && m_select->GetTypeID() == ID_TUPON)
	{
		float x,y;
		Becher::GetView()->GetPick(0,x,y);
		tTupon* t = reinterpret_cast<tTupon*>(m_select);
		t->jobs.Reset();
		t->jobs.Go(x,y);
		t->jobs.Msg("Jsem tu sefe..");
		t->jobs.Idle();
	}

	Becher::GetBecher()->BuildCancel();
}

void Becher::CreateBuilding(int idtype)
{
	if (m_build)
	{
		if (m_build->GetTypeID() == idtype)
			return;
		m_build->Delete();
	}
	m_build = ::CreateBuilding(idtype);
}

  */
