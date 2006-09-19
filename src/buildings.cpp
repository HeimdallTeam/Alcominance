
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "id.h"
#include "troll.h"
#include "obj_store.h"

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
	m_mode = EBM_Normal;
}

#ifndef BECHER_EDITOR
bool BecherBuilding::StartBuilding(int gold, int wood, int stone)
{
	/*if (!GetBecher()->GetCash()->Add(-gold))
	{
		GetBecher()->GetInfoPanel()->Add(1);
		return false;
	}*/

	return true;
}

#endif

bool BecherBuilding::Save(BecherGameSave &w)
{
	BecherObject::Save(w);
	dword m = m_mode;
	w.Write<dword>(m);
	return true;
}

bool BecherBuilding::Load(BecherGameLoad &r)
{
	BecherObject::Load(r);
	this->SetMode((EBuildingMode)r.Read<dword>());
	return true;
}

float getheight(IHoeModel*m)
{
	THoeParameter p;
	m->GetParameter("boundbox", &p);
	return p.box.top;
}













