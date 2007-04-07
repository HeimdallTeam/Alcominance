
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_shop.h"

static CVar v_cost("shop_cost", 200, TVAR_SAVE); // cena za stavbu
static CVar v_cost_wood("shop_cost_wood", 100, TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cost_stone("shop_cost_stone", 50, TVAR_SAVE); // pocet kameni potrebneho na stavbu

/////////////////////////////////////////////////////////////
Shop::Shop(IHoeScene * scn) : BecherBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_SHOP));
	//SetRingParam(1.5f,3.f,2.f);
}

bool Shop::Save(ChunkDictWrite &w)
{
	BecherBuilding::Save(w);
	return true;
}

bool Shop::Load(const ChunkDictRead &r)
{
	BecherBuilding::Load(r);
	OnUpdateSur();
	return true;
}

#ifndef BECHER_EDITOR

void Shop::Update(const float t)
{
}

bool Shop::Select()
{
	//BecherBuilding::Select();
	GetLua()->func("s_shop");
	return true;
}


#else
void Shop::Update(const float t)
{
}

bool Shop::Select()
{
	//BecherBuilding::Select();
	return true;
}

#endif


