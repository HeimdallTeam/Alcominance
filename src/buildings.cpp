
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "id.h"
#include "troll.h"
#include "obj_store.h"
#include "obj_construct.h"
#include "b_msg.h"

/////////////////////////////////////////////////////////
BecherBuilding::BecherBuilding(IHoeScene * scn) : BecherObject(scn)
{
	m_mode = EBM_None;
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

int BecherBuilding::BuildPlace(float *pos, IHoeModel * m, float height, float dobj,bool finish)
{
	// pozice v mape
#ifndef BECHER_EDITOR
	float min,max;
	float x=pos[0];
	float y=pos[1];
	bool ok;
	THoeParameter par;
	if (m)
		m->GetParameter("boundbox",&par);
	else
		memset(&par,0, sizeof(par));
	max = min = 0.f;
	// 177 132
	ok = GetLevel()->GetScene()->GetScenePhysics()->GetCamber(
		x+par.box.left,x+par.box.right,y+par.box.front,y+par.box.back,min,max);
	SetPosition(x,y,max);
	if (!ok || (max-min) > height) 
	{
		GetCtrl()->SetOverColor(0xffff0000);
		if (finish)
			SendGameMsgId(0, BMSG_Info, 0,(void*)GetLang()->GetString(101), 1);
		return 0;
	}
	// zjistit zda muze byt cerveny nebo jiny
	for (int i=0; i < GetLevel()->GetNumObj();i++)
	{
		float x = GetLevel()->GetObj(i)->GetPosX();
		float y = GetLevel()->GetObj(i)->GetPosY();
		x -= GetPosX();
		y -= GetPosY();
		if (x*x+y*y < dobj)
		{
			GetCtrl()->SetOverColor(0xffff0000);
			if (finish)
				SendGameMsgId(0, BMSG_Info, 0,(void*)GetLang()->GetString(102), 1);
			return 0;
		}
	}
	GetCtrl()->SetOverColor(0xffffffff);
#endif
	return 1;
}

float getheight(IHoeModel*m)
{
	THoeParameter p;
	m->GetParameter("boundbox", &p);
	return p.box.top;
}

int BecherBuilding::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	return BecherObject::GameMsg(msg, par1, par2, npar2);
}












