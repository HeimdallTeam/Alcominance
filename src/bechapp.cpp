
#include "StdAfx.h"
#include "bechapp.h"
#include "becher.h"
#include "game.h"

HoeGame::CVar v_level("map", "test.bm", 0);

class BecherLogo : public HoeGame::Scene, public XHoe2DCallback
{
	IHoeFont * font;
	IHoe3DEngine * engine;
	IHoeScene * scene;
public:
	BecherLogo()
	{
		scene = GetEngine()->CreateScene(HOETS_2D);
		scene->Set2DCallback(this);
		font = (IHoeFont*)GetEngine()->Create("font 'resource/font.ttf' 24");
	}
	~BecherLogo()
	{
		// delete scene
		font->Delete();
	}
	virtual void OnSet()
	{
		GetEngine()->SetActiveScene(scene);
		HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Foreground, NULL);
	}
	virtual void OnUnset()
	{
		GetEngine()->SetActiveScene(NULL);
	}
	virtual void HOEAPI _Paint(IHoe2D * h2d)
	{
		h2d->SetRect(800,600);
		font->DrawText(535,507,0xffffffff,"Loading..."); 
	}
};

class Fade
{
protected:
	bool m_fading;
	dword m_tocolor;
	dword m_color;
	float m_time;
	float m_finish;
public:
	Fade()
	{
		m_fading = false;
		m_color = 0x0;
	}
	bool IfFading() { return m_fading; }
	void Update(float dtime)
	{
		// prechod pro vsechny 4 barvy
		if (m_fading)
		{
			m_time += dtime;
			if (m_time >= m_finish)
			{
				m_color = m_tocolor;
				m_fading = false;
			}
			// u kazde barvy rozdil
			byte a,r,g,b,a1,r1,g1,b1;
			float p = m_time / m_finish;
			a = ((m_color&0xff000000) >> 24);
			r = ((m_color&0xff0000) >> 16);
			g = ((m_color&0xff00) >> 8);
			b = ((m_color&0xff));
			a1 = ((m_tocolor&0xff000000) >> 24);
			r1 = ((m_tocolor&0xff0000) >> 16);
			g1 = ((m_tocolor&0xff00) >> 8);
			b1 = ((m_tocolor&0xff));
			a += ((a1-a) * p);
			r += ((r1-r) * p);
			g += ((g1-g) * p);
			b += ((b1-b) * p);
			m_color = (a << 24) | (r << 16) | (r << 8) | b;
		}
	}
	void Draw(IHoe2D * h2d)
	{
		if (m_color & 0xff000000)
		{
			h2d->SetRect(1,1);
			h2d->PaintRect(0,1,0,1,m_color, true);
		}
	}
	void ToFade(dword color, float time)
	{
		if (time > 0)
		{
			m_tocolor = color;
			m_fading = true;
		}
		else
			m_color = color;
		m_finish = time;
		m_time = 0;
	}
};

class SlideShow : public HoeGame::Scene, public XHoe2DCallback, public HoeGame::Hoe2DFigure
{
protected:
	HoeGame::LuaScript m_lua;
	IHoeScene * scene;
	bool m_isend;
	static SlideShow * m_instance;
	float m_waittime;
	Fade m_fade;
public:
	SlideShow()
	{
		scene = GetEngine()->CreateScene(HOETS_2D);
		scene->Set2DCallback(this);
		//font = (IHoeFont*)GetEngine()->Create("font 'resource/font.ttf' 24");
		m_isend = false;
		m_waittime = 0.f;
		m_instance = this;
		//m_text[0] = 0;
		//m_image = NULL;
	}
	~SlideShow()
	{
		// delete scene
	}
	virtual void OnSet()
	{
		GetEngine()->SetActiveScene(scene);
		HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Foreground, NULL);
	}
	virtual void OnUnset()
	{
		GetEngine()->SetActiveScene(NULL);
	}
	virtual void HOEAPI _Paint(IHoe2D * h2d)
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
	virtual void Update(float dtime)
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
	bool IsEnd() { return m_isend; }
	void Run(const char * script)
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
		m_lua.Load(script, 0, false); 
		lua_resume(m_lua.GetLua(),0);
	}
	static int l_wait(lua_State * L)
	{
		HoeGame::LuaParam lp(L);
		if (lp.CheckPar(1,"n", "Wait"))
		{
			m_instance->m_waittime = lp.GetDouble(-1);
		}
		return lua_yield(L,0);
	}
	static int l_settext(lua_State * L)
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
	static int l_loadimage(lua_State * L)
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
	static int l_fade(lua_State * L)
	{
		HoeGame::LuaParam lp(L);
		if (lp.CheckPar(2,"ns", "Fade"))
		{
			dword color;
			sscanf(lp.GetString(-2),"%x", &color);
			m_instance->m_fade.ToFade(color, lp.GetDouble(-1));
		}
		return lua_yield(L,0);
	}
};

SlideShow * SlideShow::m_instance = NULL;

BechApp::BechApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeApp(instance, con)
{
	scene = NULL;
}

bool BechApp::InitGame()
{
	{
		// init

	}
	// create scen
	// run intro
	{
		SlideShow intro;
		SetScene(&intro);
		intro.Run("scripts/intro.lua");
		while (!intro.IsEnd())
		{
			if (!Frame())
				return false;
		}
	}

	BecherLogo logo;
	SetScene(&logo);
	Frame(0);
	// vytvorit game
	m_game = new BecherGame();

	HoeGetInfo(GetEngine())->LoadFont("resource/font.ttf",12); 

	if (!m_game->Init())
		return false;

	SetScene(NULL);
	return true;
}

void BechApp::SetScene(HoeGame::Scene * scn)
{
	if (scene)
		scene->OnUnset();
	scene = scn;
	if (scn)
        scn->OnSet();
}

void BechApp::OnUpdate(float dtime)
{
	if (scene)
		scene->Update(dtime);
}

#if 1
#undef BEGIN_TRY
#undef END_TRY 
#define BEGIN_TRY
#define END_TRY(code)
#endif

int BechApp::RunGame()
{
	BEGIN_TRY
	if (!Init("Alcominance Game"))
	{
		HandleError();
		return 0;
	}
	// init game
	if (InitGame())
	{
		GetBecher()->LoadLevel(v_level.GetString());
		
		Run();
	}

	// music destroy
	GetBecher()->Destroy();
	DestroyEngine();

	END_TRY(DestroyEngine(); HandleError(); )

	Destroy();
	return 0;
}



