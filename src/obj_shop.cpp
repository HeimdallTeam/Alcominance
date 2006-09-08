
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_shop.h"



/////////////////////////////////////////////////////////////
Shop::Shop(IHoeScene * scn) : BecherBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_SHOP));
	m_infoselect.s_x = 1.5f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 3.f;
}

#ifndef BECHER_EDITOR
void Shop::Update(const double t)
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

bool Shop::Idiot(Job * t)
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


