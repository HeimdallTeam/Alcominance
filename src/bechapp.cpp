
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
	BecherLogo(IHoe3DEngine * eng)
	{
		engine = eng;
		scene = engine->CreateScene(HOETS_2D);
		scene->Set2DCallback(this);
		font = (IHoeFont*)eng->Create("font 'resource/font.ttf' 24");
	}
	virtual void OnSet()
	{
		engine->SetActiveScene(scene);
		HoeGetInput(engine)->RegisterMouse(IHoeInput::MT_Foreground, NULL);
	}
	virtual void OnUnset()
	{
		engine->SetActiveScene(NULL);
	}
	virtual void HOEAPI _Paint(IHoe2D * h2d)
	{
		h2d->SetRect(800,600);
		font->DrawText(535,507,0xffffffff,"Loading..."); 
	}
	~BecherLogo()
	{
		// delete scene
		font->Delete();
	}
};

BechApp::BechApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeApp(instance, con)
{
	scene = NULL;
}

bool BechApp::InitGame()
{
	// create scen
	BecherLogo logo(GetEngine());
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
	if (!InitGame())
		throw;

	GetBecher()->LoadLevel(v_level.GetString());
	
	Run();
	// music destroy
	GetBecher()->Destroy();
	DestroyEngine();

	END_TRY(DestroyEngine(); HandleError(); )

	Destroy();
	return 0;
}



