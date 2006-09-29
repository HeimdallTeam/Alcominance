
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "id.h"
#include "troll.h"
#include "obj_store.h"
#include "obj_construct.h"

// premistit
void TrollList::OneStopWork()
{
	// kvuli optimalizaci je lepsi pozpatku, protoze oni se hned z toho seznamu vymazavaj
	//for (int i=Count()-1;i >= 0;i--)
	//{
	//	Get(i)->StopWork();
	//}
	if (Count()>0)
		Get(Count()-1)->StopWork();
}



/////////////////////////////////////////////////////////
BecherBuilding::BecherBuilding(IHoeScene * scn) : BecherObject(scn)
{
	m_mode = EBM_None;
	m_construct = NULL;
}

#ifndef BECHER_EDITOR
bool BecherBuilding::StartBuilding(int gold, int wood, int stone)
{
	if (!GetLevel()->GetCash()->Add(-gold))
	{
		GetLevel()->GetPanel()->GetInfo()->Add(GetLang()->GetString(104));
		return false;
	}
	// build
	SetMode(EBM_Build);
	if (m_construct)
		m_construct->SetResources(wood, stone);
	else
		SetMode(EBM_Normal);

	return true;
}

#endif

bool BecherBuilding::Save(BecherGameSave &w)
{
	BecherObject::Save(w);
	w.WriteValue<dword>(m_mode);
	return true;
}

bool BecherBuilding::Load(BecherGameLoad &r)
{
	BecherObject::Load(r);
	this->SetMode((EBuildingMode)r.Read<dword>());
	return true;
}

void BecherBuilding::SetCurActive(bool active)
{
	if (m_mode == EBM_Select)
		BecherObject::SetCurActive(false);
	else
		BecherObject::SetCurActive(active);
}

float getheight(IHoeModel*m)
{
	THoeParameter p;
	m->GetParameter("boundbox", &p);
	return p.box.top;
}













