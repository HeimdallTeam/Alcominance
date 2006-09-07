
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "id.h"
#include "game.h"
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
	return true;
}

bool BecherBuilding::Load(BecherGameLoad &r)
{
	return true;
}

//////////////////////////////////////////////////////////////////
float getheight(IHoeModel *);

HerbeWoman::HerbeWoman(IHoeScene * scn) : BecherBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_BABA));
	m_infoselect.s_x = 1.4f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 2.f;
	static THoeSub_Model m;
	m.model = dynamic_cast<IHoeModel*>(GetEngine()->Create("model miska"));
	m.t_x = 0.f;
	m.t_y = getheight(this->GetModel()) + 3.f;
	m.t_z = 0.f;
	m.s_x = 2.f;
	m.s_y = 4.f;
	m.s_z = 2.f;
	GetCtrl()->Link(THoeSubObject::Object, &m);
}

HerbeWoman::~HerbeWoman()
{
}


/*void HerbeWoman::AdvPaint(IHoePaint3D * h3)
{
	// info
	if (this->IsCurActive() || this->IsSelected())
	{
		static IHoeModel * mod_i = dynamic_cast<IHoeModel*>(GetEngine()->Create("model info"));
		h3->ResetPos();
		h3->Scale(2.5, 0, 2.5);
		h3->Move(0,2,0);
		h3->Paint(mod_i);
	}

	// velikost skladu
	static float p = getheight(this->GetModel());

	static IHoeModel * mod = dynamic_cast<IHoeModel*>(GetEngine()->Create("model miska"));
	h3->ResetPos();
	
	h3->Scale(2,4,2);
	h3->RotateY(GetEngine()->SysFloatTime() * 2);
	h3->Move(0, p+3, 0);
	h3->Paint(mod);
	
}*/

float getheight(IHoeModel*m)
{
	THoeParameter p;
	m->GetParameter("boundbox", &p);
	return p.box.top;
}













