
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
	m_infoselect.s_x = 1.5f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 3.f;
}

bool Shop::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteReservedWords(10);
	return true;
}

bool Shop::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(10);
	OnUpdateSur();
	return true;
}

#ifndef BECHER_EDITOR
ResourceBase * Shop::GetResource(ESurType type)
{
	switch (type)
	{
	default:
		return NULL;
	};
}

const char * Shop::BuildPlace(float x, float y)
{
	// pozice v mape
	float min,max;
	bool ok;
	max = min = 0.f;
	ok = GetLevel()->GetScene()->GetScenePhysics()->GetCamber(x,x,y,y,min,max);
	SetPosition(x,y,min);
	if (!ok || (max-min) > 1.f) 
	{
		GetCtrl()->SetOverColor(0xffff0000);
		return GetLang()->GetString(101);
	}
	// zjistit zda muze byt cerveny nebo jiny
	for (int i=0; i < GetLevel()->GetNumObj();i++)
	{
		float x = GetLevel()->GetObj(i)->GetPosX();
		float y = GetLevel()->GetObj(i)->GetPosY();
		x -= GetPosX();
		y -= GetPosY();
		if (x*x+y*y < 4000.f)
		{
			GetCtrl()->SetOverColor(0xffff0000);
			return GetLang()->GetString(102);
		}
	}
	GetCtrl()->SetOverColor(0xffffffff);
	return NULL;
}

void Shop::Update(const float t)
{
}

bool Shop::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool Shop::SetToWork(Troll * t)
{
	return false;
}

void Shop::UnsetFromWork(Troll * t)
{
}

bool Shop::Select()
{
	BecherBuilding::Select();
	GetLua()->func("s_shop");
	return true;
}

bool Shop::Idiot(TJob * t)
{
	return false;
}


#else
bool Shop::Select()
{
	BecherBuilding::Select();
	return true;
}

void Shop::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}

#endif


