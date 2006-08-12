
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

int Shop::AddSur(ESurType type, int s)
{
	
	//trtina += s;
	GetBecher()->GetCash()->Add(s);
	return 0;
}

bool Shop::Select()
{
	if (!IsBuildMode())
        GetLua()->func("s_shop");
	return true;
}

int Shop::GetSur(ESurType EBO_Shop, int req, bool upln)
{
	return 0;
}

void Shop::AddToWork(Troll *t)
{
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


