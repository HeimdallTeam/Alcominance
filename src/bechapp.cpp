
#include "StdAfx.h"
#include "bechapp.h"
#include "becher.h"
#include "game.h"
#include "screens.h"

HoeGame::CVar v_level("map", "test.bm", 0);
static CVar v_skipintro("skip_intro", false, 0); // maximalni kapacita pro uhli


BechApp::BechApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeApp(instance, con)
{
	scene = NULL;
}

bool BechApp::InitGame()
{
	{
		// init
		// ukazka loga
	}
	// create scen
	// run intro
	if (!v_skipintro.GetBool())
	{
		SlideShow intro;
		SetScene(&intro);
		intro.Run();
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



