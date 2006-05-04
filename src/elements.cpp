
#include "StdAfx.h"
#include "elements.h"
#include "becher.h"
#include "id.h"
#include "game.h"

//////////////////////////////////////////////////////////////////

Tree::Tree(IHoeScene * scn) : BecherObject(scn)
{
#ifdef BECHER_PLUGIN
	SetOrientation(0,1,0,(rand() % 628) * 0.01);
#endif

	param.wood = 100;
	param.height = 1.f;
	SetTypeModel(1);
}

Tree::~Tree()
{
}

void Tree::LoadAdv(int ver, unsigned long size, HoeFileReader * stream)
{
	if (ver == 1)
	{
        stream->Read(&param, size);
		SetHeight(param.height);
		SetTypeModel(param.type);
	}

}

int Tree::SaveAdv(void ** buff, unsigned long * size)
{
	*buff = &param;
	*size = sizeof(param);
	return 1;
}

void Tree::SetHeight(float h)
{
	param.height = h;
	if (h != 1.f)
	{
		GetCtrl()->SetScale(1,h,1);
		GetCtrl()->SetFlags(HOF_SCALED);
	}
	else
		GetCtrl()->UnsetFlags(HOF_SCALED);
}

void Tree::SetTypeModel(int type)
{
	param.type = type;
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_STROM+type));
}

////////////////////////////////////////////////////////////
Bridge::Bridge(IHoeScene * scn) : BecherObject(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_MOST));

}

////////////////////////////////////////////////////////////
WaterHole::WaterHole(IHoeScene * scn) : BecherObject(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_WATERHOLE));

}

#ifndef BECHER_EDITOR

bool WaterHole::Select()
{
	GetLua()->func("s_studna");
	return true;
}

#endif


