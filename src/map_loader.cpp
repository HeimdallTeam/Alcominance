
#include "StdAfx.h"
#include "hoe3d.h"
#include "hoe_input_codes.h"
#include "map_loader.h"
#include "becher.h"
#include "lua_script.h"
#include "id.h"


MapLoader::MapLoader()
{
	m_font = NULL;
	progress = 0.f;
}

int MapLoader::LoadingProgress(iHoe2D * hoe2d)
{
	char text[256];
	sprintf(text,"Nahravani mapy... %d%%",(int)progress);
	m_font->DrawText(50,50,0xffffffff,text);
	return 0;
}

void MapLoader::Process(float proc)
{

	// update engine
	UpdateFrame();
}

iHoeMap * MapLoader::LoadMap(const char * fname)
{
	iHoeMap * map;
	m_font = (iHoeFont*)Becher::GetResMgr()->GetResource(ID_FONT_LOADER);

	Becher::GetEngine()->SetRenderState(HOERS_DISABLE3D,true);
	Becher::GetView()->Cursor(false);

	progress = 0;
	Process(progress);

	//m_hoe3D->LoadMap("../utils/hmap/map.out",new MapLoaderCallback);

	map = Becher::GetEngine()->LoadMap(fname,this);

	LoadScript("main.lua");

	progress = 100;
	Process(progress);

	Becher::GetEngine()->SetRenderState(HOERS_DISABLE3D,false);
	Becher::GetView()->Cursor(true);

	return map;
}

void MapLoader::UpdateFrame()
{
	// tady by melo jeste bejt update aplikace..
	Becher::GetEngine()->Process(0);
	Becher::GetEngine()->Frame();
}

void MapLoader::SetProgress(int proc, const char *msg)
{
	progress = proc;
	UpdateFrame();

}
