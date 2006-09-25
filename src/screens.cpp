
#include "StdAfx.h"
#include "becher.h"
#include "screens.h"

BecherLogo::BecherLogo()
{
	scene = GetEngine()->CreateScene(HOETS_2D);
	scene->Set2DCallback(this);
	font = (IHoeFont*)GetEngine()->Create("font 'resource/font.ttf' 24");
}

BecherLogo::~BecherLogo()
{
	// delete scene
	font->Delete();
}

void BecherLogo::OnSet()
{
	GetEngine()->SetActiveScene(scene);
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Foreground, NULL);
}

void BecherLogo::OnUnset()
{
	GetEngine()->SetActiveScene(NULL);
}

void HOEAPI BecherLogo::_Paint(IHoe2D * h2d)
{
	h2d->SetRect(800,600);
	font->DrawText(535,507,0xffffffff,"Loading..."); 
}

//////////////////////////////////////////////

SlideShow * SlideShow::m_instance = NULL;

SlideShow::SlideShow()
{
	scene = GetEngine()->CreateScene(HOETS_2D);
	scene->Set2DCallback(this);
	//font = (IHoeFont*)GetEngine()->Create("font 'resource/font.ttf' 24");
	m_isend = false;
	m_waittime = 0.f;
	assert(m_instance == NULL);
	m_instance = this;
	//m_text[0] = 0;
	//m_image = NULL;
}

SlideShow::~SlideShow()
{
	assert(m_instance == this);
	m_instance = NULL;
	// delete scene
}

void SlideShow::OnSet()
{
	GetEngine()->SetActiveScene(scene);
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Foreground, NULL);
	HoeGetInput(GetEngine())->RegisterKeyboard(this);
}

void SlideShow::OnUnset()
{
	GetEngine()->SetActiveScene(NULL);
	HoeGetInput(GetEngine())->RegisterKeyboard(NULL);
}

void HOEAPI SlideShow::_Paint(IHoe2D * h2d)
{
	/*h2d->SetRect(1,1);
	if (m_image)
		h2d->BltFast(0,1,0,1, m_image);
	h2d->SetRect(800,600);
	if (m_text[0])
		font->DrawText(535,507,0xffffffff,m_text); */
	Draw(h2d);
	m_fade.Draw(h2d);
}

void SlideShow::OnKeyDown(int key)
{
	m_isend = true;
}

void SlideShow::Update(float dtime)
{
	// cekajici funkce
	if (m_isend)
		return;
	if (m_fade.IfFading())
	{
		m_fade.Update(dtime);
		return;
	}
	m_waittime -= dtime;
	if (m_waittime <= 0.f)
	{
		int ii = lua_resume(m_lua.GetLua(),0);
		m_isend = ii == 0;
	}
}

void SlideShow::Run()
{
	m_lua.Init();
	m_lua.Connect(GetApp()->GetFS());
	m_lua.Connect(GetEngine());
	m_lua.Load("scripts/init.lua", 0, true); 
	this->Load("scripts/intro.menu");
	HoeGame::Gui::StaticPicture * pic = (HoeGame::Gui::StaticPicture*)m_instance->ReqItem("pozadi", HoeGame::Gui::EStatic);
	pic->Hide();
	m_lua.AddFunc("Wait",SlideShow::l_wait);
	m_lua.AddFunc("SetText",SlideShow::l_settext);
	m_lua.AddFunc("LoadImage",SlideShow::l_loadimage);
	m_lua.AddFunc("Fade",SlideShow::l_fade);
	m_lua.Load("scripts/intro.lua", 0, false); 
	lua_resume(m_lua.GetLua(),0);
}

int SlideShow::l_wait(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1,"n", "Wait"))
	{
		m_instance->m_waittime = (float)lp.GetDouble(-1);
	}
	return lua_yield(L,0);
}

int SlideShow::l_settext(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(2,"ss", "SetText"))
	{
		HoeGame::Gui::Font * text = 
			(HoeGame::Gui::Font*)m_instance->ReqItem(lp.GetString(-2), HoeGame::Gui::EText);
		text->SetText(lp.GetString(-1));
		//strcpy(m_instance->m_text, lp.GetString(-1));
		//else
		//	tt = GetLang()->Get(lp.GetNum(-2));
		//GetLevel()->GetPanel()->AddButton(lp.GetNum(-3),lp.GetString(-1),tt);
	}
	return 0;
}

int SlideShow::l_loadimage(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1,"s", "LoadImage"))
	{
		HoeGame::Gui::StaticPicture * pic = (HoeGame::Gui::StaticPicture*)m_instance->ReqItem("pozadi", HoeGame::Gui::EStatic);
		if (pic->GetPicture())
			pic->GetPicture()->Delete();
		if (lp.GetString(-1)[0])
		{
			pic->Show();
			pic->SetPicture((IHoePicture*)GetEngine()->Create(lp.GetString(-1)));
		}
		else
		{
			pic->Hide();
			pic->SetPicture(NULL);
		}
	}
	return 0;
}

int SlideShow::l_fade(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(2,"ns", "Fade"))
	{
		dword color;
		sscanf(lp.GetString(-2),"%x", &color);
		m_instance->m_fade.ToFade(color, (float)lp.GetDouble(-1));
	}
	return lua_yield(L,0);
}

