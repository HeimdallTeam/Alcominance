
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_shop.h"
#include "obj_store.h"

/////////////////////////////////////////////////////////////
Shop::Shop(IHoeScene * scn) : BecherBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_SHOP));
}

#ifndef BECHER_EDITOR
void Shop::Update(const double t)
{
}

bool Shop::Select()
{
	if (!IsBuildMode())
        GetLua()->func("s_shop");
	return true;
}

bool Shop::Idiot(Job * t)
{
	return false;
}

int Shop::GetNumInfos()
{
	return 0;
}

int Shop::GetInfo(int id, char *buff, size_t size)
{
	return 0;
}

#else
bool Shop::Select()
{
	return true;
}
#endif


