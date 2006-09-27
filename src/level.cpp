
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "elements.h"
#include "level.h"
#include "game.h"
#include "dialogs.h"
#include "troll.h"
#include "obj_sugar.h"
#include "obj_destilate.h"
#include "obj_factory.h"
#include "obj_store.h"
#include "obj_waterhole.h"
#include "obj_farm.h"

static CVar v_camera("camera_speed", 0.9f, 0);


BecherLevel::BecherLevel()
{
	this->m_build = NULL;
	m_filename[0] = '\0';
	m_select = NULL;
	m_mselect = NULL;
	m_selhud = NULL;
	m_dlg = NULL;
	//m_info.Init(10.f,550.f, 50.f);
	//m_controls.Init();

}

void BecherLevel::_Paint(IHoe2D * h2d)
{
	// hud
	m_hud.Draw(h2d);
	if (m_selhud)
		m_selhud->Draw(h2d);
	//m_controls.Draw(h2d);
	//m_info.Draw(h2d);
	//GetCash()->Paint(h2d);
	// dlg
	if (m_dlg)
		m_dlg->Draw(h2d);

	if (GetCon()->IsActive())
        GetCon()->Draw(h2d);

}

void BecherLevel::OnSet()
{
	HoeGetRef(GetEngine())->SetBackgroundColor(0x00000000);
	HoeGetInput(GetEngine())->RegisterKeyboard(this);
		//if (HoeGetRef(GetEngine())->IsFullscreen())
		//	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Cursored, this);
		//else
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Background, this);

	GetScene()->Set2DCallback(this);
	GetEngine()->SetActiveScene(GetScene());	

}

#define MOVE_STEP 40.f

void BecherLevel::Update(float time)
{
	if (IsPaused())
	{
		GetCon()->Update(time);
		return;
	}
	else
	{
		m_timer.Update(time);
		GetView()->Update(time);
		// update objects
		for (int i=0;i < this->m_numobj;i++)
		{
			this->m_obj[i]->Update(time);
		}
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

void BecherLevel::MouseUpdate(float x, float y)
{
	if (m_build)
	{
		float X,Y;
		m_build->Show(true);
		if (GetView()->GetPick(x,y,&X,&Y))
			m_build->SetPosition(X,Y,0);
	}
	else // show select
	{
		// vybrat objekt pro zobrazeni
	}
}

void BecherLevel::MouseLeave() 
{
	if (m_build)
		m_build->Show(false);
}

void BecherLevel::SetBuildObject(BecherBuilding * bo, int gold, int wood, int stone)
{
	if (m_build)
		delete m_build;

	m_build = bo;
	m_buildgold = gold;
	m_buildwood = wood;
	m_buildstone = stone;
	//m_buildinit = m_select;
	bo->SetMode(EBM_Select);
	bo->Show(false);
}

void BecherLevel::Start()
{
	m_timer.Set(HoeGame::Timer::tmClock, 0);
	m_timer.Start();
	IHoeLight * light = m_scene->CreateLight(true);
}

void BecherLevel::MouseLeftDown(float x, float y)
{
	if (m_build)
	{
		//m_buildcandidate.wood,m_buildcandidate.stone);
		//assert((this->GetSelectionCount() == 1));
		//assert(this->GetSelectedObject(0)->GetType() == EBO_Troll);
		//reinterpret_cast<Troll*>(this->GetSelectedObject(0))->Build(reinterpret_cast<BecherBuilding*>(o));
		float X,Y;
		if (GetView()->GetPick(x,y,&X,&Y))
		{
			// build
			m_build->SetPosition(X,Y,0);
			assert(GetObjectClass(m_build->GetType()));
			if (dynamic_cast<BecherBuilding*>(m_build)->StartBuilding(m_buildgold, m_buildwood, m_buildstone))
			{
				this->AddObject(m_build);
				//if (m_buildinit && m_buildinit->GetType() == EBO_Troll)
				//	dynamic_cast<BecherBuilding*>(m_build)->Assign(dynamic_cast<Troll*>(m_buildinit));
				m_build = NULL;
			}
		}
		return;
	}

	HoeGame::Strategy::StgObject * o = GetView()->SelObject(x,y);
	if (o)
		SelectObject(dynamic_cast<BecherObject *>(o));
	else
		SelectObject(NULL);
}

void BecherLevel::SelectObject(BecherObject * so)
{
	if (m_select && m_select != so)
	{
		m_select->Unselect();
		SetObjectHud(NULL);
		m_hud.ShowReset();
	}
	if (so && so->Select())
	{
		GetView()->SetTrack(so->GetPosX(), so->GetPosY(), v_camera.GetFloat());
		m_select = so;
	}
	else
		m_select = NULL;
}

bool BecherLevel::SaveGame(const char * path)
{
	HoeGame::HoeFile file;
	if (!file.Open(path, hftWrite))
		return false;
	BecherGameSave w(&file);

	MapChunk head = {ID_BSAVE, ID_BECHERVER, GetNumObj()};
	w.Write(&head, sizeof(head));

	// save map info (ktera mapa)
	size_t sfn = strlen(m_filename);
	w.Write(&sfn, sizeof(sfn));
	w.Write(m_filename, sfn);

	// save timer
	m_cash.Save(w);
	m_timer.Save(w);

	// save view
	float view[4];
	GetView()->GetTargetPosition( &view[0], &view[1]);
	view[2] = GetView()->GetAngle();
	view[3] = GetView()->GetDistance();
	w.Write(view, sizeof(view));

	// save objects
	SaveAllObjects(w);

	return true;
}

bool BecherLevel::LoadGame(BecherGameLoad &r)
{
	int ver = r.Chunk().ver;
	size_t sfn;
	r.Read(&sfn, sizeof(sfn));
	r.Read(m_filename, sfn);
	m_filename[sfn] = '\0';

	HoeGame::HoeFile file;
	file.Open(m_filename);
	BecherGameLoad rr(&file);
	Load( rr, true);

	// load timer and casch
	m_cash.Load( ver, r);
	m_timer.Load( ver, r);

	float view[4];
	r.Read(view, sizeof(view));
	GetView()->SetTargetPosition( view[0], view[1]);
	GetView()->SetAngle(view[2]);
	GetView()->SetDistance(view[3]);

	r.ReadNext();
	//LoadObjects(r);
	assert(0);

	return true;
}

bool BecherLevel::LoadGame(const char *path)
{
	Create(CreateScene());

	m_hud.Load("scripts/hud.menu");
	this->m_cash.Link(dynamic_cast<HoeGame::Gui::DigiCounter *>(m_hud.ReqItem("cash", HoeGame::Gui::EDigiCounter)));
	Sugar::m_userhud.Load("scripts/sugar.menu");
	Destilate::m_userhud.Load("scripts/alco.menu");
	Factory::m_userhud.Load("scripts/factory.menu");
	Store::m_storepref.Load("scripts/store.menu");
	WaterHole::m_userhud.Load("scripts/waterhole.menu");
	Farm::m_userhud.Load("scripts/farm.menu");
	m_builddlg.Load("scripts/build.menu");
	m_dlg = &m_builddlg;

	SetTerrainData();

	HoeGame::HoeFile file;
	if (!file.Open(path))
	{
		GetCon()->Printf("Open file %s failed.", path);
		return false;
	}

	BecherGameLoad r(&file);
	if (!r.ReadHeader())
		return false;
	if (!r.IsSaveGame())
	{
		strncpy(m_filename, path, sizeof(m_filename));
		if (!Load( r, false))
			return false;		
	}
	else 
	{
		if (!LoadGame(r))
			return false;
	}

	return true;
}

////////////////////////////////////////////////
bool BecherTime::Save(HoeFileWriter &w)
{
	return true;
}

bool BecherTime::Load(int ver, HoeFileReader &r)
{
	return true;
}

BecherCash::BecherCash()
{
	Set(-1000,-100000);
}

void BecherCash::Set(int cash, int limit)
{
	m_limit = limit;
	m_cash = cash;
}

bool BecherCash::Add(int m)
{ 
	if (GetLimitCash() < -m) 
		return false; 
	m_cash += m; 
	return true; 
}

bool BecherCash::Save(HoeFileWriter &w)
{
	//w.Write<int>(m_cash);
	//w.Write<int>(m_limit);
	return true;
}

bool BecherCash::Load(int ver, HoeFileReader &r)
{
	//m_cash = r.Read<int>();
	//m_limit = r.Read<int>();
	Set(0,-100000);
	return true;
}

void BecherLevel::OnKeyDown(int key)
{
	if (key == HK_GRAVE)
		GetCon()->Open();
	else if (key == HK_F9)
	{
			if (SaveGame("a.sav"))
				;//m_info.Add("Game saved.");
			else
				assert(!"game save failed");
			
			
	}
	/*if (GetCon()->IsActive())
		GetCon()->*/
	//m_info.Addf("key down %d",key);
}

void BecherLevel::OnMouseMove(float X, float Y)
{
	bool select = false;

	// select 2d
	/*select = this->m_controls.MouseMove(X,Y);
	if (select)
        MouseLeave();
	else*/
	BecherObject * o = NULL;
	if (m_dlg && m_dlg->Move(X,Y))
	{

	}
	else if (!m_hud.Move(X,Y))
	{
		HoeGame::Strategy::StgObject * obj = GetView()->SelObject(X,Y);
		if (obj)
			o = dynamic_cast<BecherObject *>(obj);
	}
	if (m_mselect && o != m_mselect && m_mselect != m_select)
	{
		m_mselect->SetCurActive(false);
	}
	if (o && o != m_select)
	{
		o->SetCurActive(true);
	}
	m_mselect = o;

	MouseUpdate(X,Y);

}

void BecherLevel::OnWheel(long p)
{
	if (IsShiftKeyDown())
		GetView()->Zoom(p * 0.1f);
	else
		GetView()->Rotate(p * 0.001f);
}

void BecherLevel::OnLeftButtonUp()
{
	BecherButton * butt = m_hud.GetButton(GetMouseX(), GetMouseY());
	if (butt)
	{
		butt->OnClick();
		return;
	}
	else
	//this->m_controls.ShowReset();
	MouseLeftDown(GetMouseX(), GetMouseY());
}

void BecherLevel::OnLeftButtonDown()
{
	//m_info.Addf("button up %d",p);

}

void BecherLevel::OnRightButtonUp()
{
	/*if (GetSelectionCount() > 0 && GetSelectedObject(0)->GetType() ==  EBO_Troll)
	{
		float x,y;
		if (GetView()->GetPick(GetMouseX(), GetMouseY(), &x, &y))
            reinterpret_cast<Troll*>(GetSelectedObject(0))->Go(x,y);
	}*/
}

void BecherLevel::OnRightButtonDown()
{
	//m_info.Addf("right button up");
}

void BecherLevel::OnSelectObject(EObjType type, BecherObject* obj)
{
	if (obj)
		obj->Select();
}

void BecherLevel::AddTroll(float x, float y)
{
	/*Troll * t = (Troll*)CreateObject(EBO_Troll);
	t->SetPosition(-400, -400);

	AddObject(t);
	t->Go(x,y);
	t->jobs.Reset();
	t->jobs.Go(x,y);
	t->jobs.Msg("Prisel novej tupoun");*/
}

void BecherLevel::AddBuildObject(unsigned long id, int gold, int wood, int stone)
{
	if (GetCash()->GetLimitCash() < gold)
	{
		//GetPanel()->Add(1);
		// todo -> limit pro penize
		return;
	}

	assert_obj(id);
	BecherObject * bo = CreateObject((EObjType)id);
	BecherBuilding * bb = dynamic_cast<BecherBuilding*>(bo);
	if (bb)
		SetBuildObject( bb, gold, wood, stone);
}

int BecherLevel::l_AddTroll(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1, "b", "AddTroll"))
	{
		if (lp.GetBool(-1))
		{
			if (GetLevel()->GetSelectedObject())
			{
				Troll * t = (Troll*)GetLevel()->CreateObject(EBO_Troll);
				t->SetPosition(0, 0, 0);
				GetLevel()->AddObject(t);
				BecherObject * b = GetLevel()->GetSelectedObject();
				t->FindJob(dynamic_cast<BecherBuilding*>(b));
			}
			else
				lp.Error("No object selected");
		}
		else
			GetLevel()->AddTroll(0,0);
	}
	return 0;
}

int BecherLevel::l_SetBuilding(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(4,"nnnn", "SetBuilding"))
	{
		GetLevel()->AddBuildObject(lp.GetNum(-4), lp.GetNum(-3), lp.GetNum(-2), lp.GetNum(-1));
	}
	return 0;
}

int BecherLevel::l_AddCash(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1, "n", "AddCash"))
	{
		GetLevel()->GetCash()->Add(lp.GetNum(-1));
	}
	return 0;
}

int BecherLevel::l_GetCash(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(0,0, "GetCash"))
		lp.PushNum(GetLevel()->GetCash()->GetLimitCash());
	return 1;
}

int BecherLevel::l_Camera(lua_State * L)
{
	BecherLevel *level = GetLevel();
	HoeGame::LuaParam lp(L);
	if (lp.GetNumParam() == 1 && lp.CheckPar(1,"n", "Camera"))
	{
		BecherObject * bo = level->GetObj(lp.GetNum(-1));
		if (bo)
			level->GetView()->SetTrack(bo->GetPosX(), bo->GetPosY(), v_camera.GetFloat());
	}
	else if (lp.GetNumParam() == 2 && lp.CheckPar(2,"nn", "Camera"))
	{
		level->GetView()->SetTrack(lp.GetNum(-2), lp.GetNum(-1), v_camera.GetFloat());
	}
	return 0;
}










