
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_shop.h"



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
	if (!IsBuildMode())
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


