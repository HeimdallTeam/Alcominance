
#include "StdAfx.h"
#include "bechapp.h"
#include "becher.h"
#include "game.h"

HoeGame::CVar v_level("map", "test.bm", 0);

class BecherLogo : public HoeGame::Scene
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
		font->DrawText(500,400,0xffffffff,"Loading..."); 
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
	this->SetTitle("Becher Game!");

	// create scen
	BecherLogo logo(GetEngine());
	SetScene(&logo);
	Frame(0);
	if (!InitBecher(this))
		return false;

	GetBecher()->Init();
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



