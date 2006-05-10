
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "bechapp.h"
#include "buildings.h"
#include "obj_sugar.h"
#include "troll.h"
#include "../../HoeGame/include/hoe_benchmark.h"

//HoeGame::OffsetLoop off[] = { MAKELONG(25,0), MAKELONG(26,0),MAKELONG(0,0), MAKELONG(1,0), 0xffffffff, 0xffffffff };

void Test()
{
	HoeGame::LuaFunc f(GetLua(), "i_sugar");
	f.PushTable();
	f.SetTableInteger("cane_avail", 0);
	f.SetTableInteger("cane", 10);
	f.SetTableInteger("sugar", 10);
	f.Run(1);
	f.Pop(1);
}

void LuaBenchmark()
{
	const int num = 10000;
	HoeGame::Bench b;
	b.Start("Lua Idiot");
	for (int i=0;i < num;i++)
		Test();
	b.End(num);
}

BecherGame::BecherGame()
{
}

bool BecherGame::Init()
{
	m_mselect = NULL;
	m_info.Init(10.f,550.f, 50.f);
	m_controls.Init();
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

#define MOVE_STEP 40.f

void BecherGame::Update(float time)
{
	if (m_level.IsPaused())
	{
		GetCon()->Update(time);
		return;
	}
	else
	{
		m_level.Update(time);
	}

	if (!GetCon()->IsActive())
	{
		if (this->IsKeyDown(HK_UP))
			GetView()->Move(time * MOVE_STEP,0);
		if (this->IsKeyDown(HK_DOWN))
			GetView()->Move(-time * MOVE_STEP,0);
		if (this->IsKeyDown(HK_RIGHT))
			GetView()->Move(0,time * MOVE_STEP);
		if (this->IsKeyDown(HK_LEFT))
			GetView()->Move(0,-time * MOVE_STEP);

		if (IsKeyDown(HK_MINUS))
			GetView()->Zoom(20 * time);
		if (IsKeyDown(HK_EQUALS))
			GetView()->Zoom(-20 * time);
			
		if (IsKeyDown(HK_COMMA))
			GetView()->Rotate(-time);
		if (IsKeyDown(HK_PERIOD))
			GetView()->Rotate(time);
	}
	else
	{
		GetCon()->Update(time);
	}
}

bool BecherGame::LoadLevel(const char * fpath)
{
	CloseLevel();

	if (!GetLua()->Load("scripts/main.lua",g_luaconst))
		return false;

	// benchamrk
	// LuaBenchmark();

	if (!m_level.LoadGame(fpath))
		return false;
		// set scene
	//if (m_scene)
	//	delete m_scene;
	//m_level->Create(GetEngine()->CreateScene(HOETS_GRAPH));

	GetApp()->SetScene(this);
	m_level.Start();
	return true;
}

void BecherGame::CloseLevel()
{
}

void BecherGame::OnSet()
{
	//if (m_level)
	{
		HoeGetRef(GetEngine())->SetBackgroundColor(0x00000000);
		HoeGetInput(GetEngine())->RegisterKeyboard(this);
		//if (HoeGetRef(GetEngine())->IsFullscreen())
		//	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Cursored, this);
		//else
			HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Background, this);

		m_level.GetScene()->Set2DCallback(this);
		GetEngine()->SetActiveScene(m_level.GetScene());	
	}
}

void BecherGame::OnPaint()
{
	m_controls.Draw(Get2D());
	m_info.Draw(Get2D());
	if (GetCon()->IsActive())
        GetCon()->Draw(Get2D());

	GetCash()->Paint(Get2D());
}

void BecherGame::OnKeyDown(int key)
{
	if (key == HK_GRAVE)
		GetCon()->Open();
	else if (key == HK_F9)
	{
			if (m_level.SaveGame("a.sav"))
				m_info.Add("Game saved.");
			else
				assert(!"game save failed");
			
			
	}
	/*if (GetCon()->IsActive())
		GetCon()->*/
	//m_info.Addf("key down %d",key);
}

void BecherGame::OnMouseMove(float X, float Y)
{
	bool select = false;

	// select 2d
	select = this->m_controls.MouseMove(X,Y);
	if (select)
        m_level.MouseLeave();
	else
		m_level.MouseUpdate(X,Y);

}

void BecherGame::OnWheel(long p)
{
	if (IsShiftKeyDown())
		GetView()->Zoom(p * 0.1f);
	else
		GetView()->Rotate(p * 0.001f);
}

void BecherGame::OnLeftButtonUp()
{
	int i = m_controls.GetButton(GetMouseX(), GetMouseY());
	if (i != -1)
	{
		// zrusit select
		m_controls.GetButton(i)->Click();
		return;
	}

	this->m_controls.ShowReset();
	m_level.MouseLeftDown(GetMouseX(), GetMouseY());
}

void BecherGame::OnLeftButtonDown()
{
	//m_info.Addf("button up %d",p);

}

void BecherGame::OnRightButtonUp()
{
	/*if (GetSelectionCount() > 0 && GetSelectedObject(0)->GetType() ==  EBO_Troll)
	{
		float x,y;
		if (GetView()->GetPick(GetMouseX(), GetMouseY(), &x, &y))
            reinterpret_cast<Troll*>(GetSelectedObject(0))->Go(x,y);
	}*/
}

void BecherGame::OnRightButtonDown()
{
	//m_info.Addf("right button up");
}

void BecherGame::OnSelectObject(EObjType type, BecherObject* obj)
{
	if (obj)
		obj->Select();
}

void BecherGame::AddTroll(float x, float y)
{
	/*Troll * t = (Troll*)m_level.CreateObject(EBO_Troll);
	t->SetPosition(-400, -400);

	m_level.AddObject(t);
	t->Go(x,y);
	t->jobs.Reset();
	t->jobs.Go(x,y);
	t->jobs.Msg("Prisel novej tupoun");*/
}

void BecherGame::AddBuildObject(unsigned long id, int gold, int wood, int stone)
{
	if (GetCash()->GetLimitCash() < gold)
	{
		GetInfoPanel()->Add(1);
		return;
	}

	BecherObject * bo = m_level.CreateObject(id);
	m_level.SetBuildObject( bo, gold, wood, stone);
}

int BecherGame::l_AddTroll(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1, "b", "AddTroll"))
	{
		if (lp.GetBool(-1))
		{
			if (GetBecher()->m_level.GetSelectedObject())
			{
				Troll * t = (Troll*)GetBecher()->m_level.CreateObject(EBO_Troll);
				t->SetPosition(-400, -400);
				GetBecher()->m_level.AddObject(t);
				BecherObject * b = GetBecher()->m_level.GetSelectedObject();
				dynamic_cast<BecherBuilding*>(b)->Idiot(t);
			}
			else
				lp.Error("No object selected");
		}
		else
			GetBecher()->AddTroll(0,0);
	}
	return 0;
}

int BecherGame::l_SetBuilding(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(4,"nnnn", "SetBuilding"))
	{
		GetBecher()->AddBuildObject(lp.GetNum(-4), lp.GetNum(-3), lp.GetNum(-2), lp.GetNum(-1));
	}
	return 0;
}

int BecherGame::l_PlaySound(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1,"n", "PlaySound"))
	{
		GetResMgr()->Get<IHoeSound>(lp.GetNum(-1))->Play();
		return 0;
	}

	return 0;
}

int BecherGame::l_AddCash(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1, "n", "AddCash"))
	{
		GetBecher()->m_level.GetCash()->Add(lp.GetNum(-1));
	}
	return 0;
}

int BecherGame::l_GetCash(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	lp.CheckPar(0,0, "GetCash");
	lp.PushNum(GetBecher()->GetCash()->GetLimitCash());
	return 1;
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















