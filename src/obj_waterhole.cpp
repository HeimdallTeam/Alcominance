
#include "StdAfx.h"
#include "becher.h"
#include "obj_waterhole.h"
#include "troll.h"
#include "sysobjs.h"

static CVar v_numzpr("water_mine", 1.f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_cena("water_cost", 80, TVAR_SAVE);
/*
static CVar v_cena_drevo("water_cost_wood", 10, TVAR_SAVE);
static CVar v_cena_cena("water_cost_stone", 35,TVAR_SAVE);
*/
static CVar v_numworks("water_maxwork", 1, TVAR_SAVE);
static CVar v_speed("water_speed", 0.21f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_max("water_max", 50, TVAR_SAVE);


////////////////////////////////////////////////////////////
WaterHole::WaterHole(IHoeScene * scn) : SourceBuilding(scn), m_water(EBS_Water)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_WATERHOLE));
	SetRingParam(1.f,1.f,2.f);
	m_water.SetOwner(this); CRR::Get()->Register(&m_water);
	m_water.SetPriority(EBSP_TimeWork);
	m_water.SetNum(v_max.GetInt());
	m_kap = 0.f;
}

bool WaterHole::Save(ChunkDictWrite &w)
{
	BecherBuilding::Save(w);
	return true;
}

bool WaterHole::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(10);
	OnUpdateSur();
	return true;
}

#ifndef BECHER_EDITOR

void WaterHole::Update(const float dtime)
{
	// update cekajicich tupounu
	// cakajici tupoun dostane dalsi dil do batohu a vypadne
	// kazdy tupoun kope zvlast, ubiha to podle tupounu
	// kazdemu tupounovi se pricita naklad zvlast
	// takze forka pres vsechny tupouny, a kazdy dostane cast dilu co si vykopal
	for (uint i=0;i<m_worked.Count();i++)
	{
		TTrollWorkSlot &slot = m_worked.Get(i);
		slot.t += v_numzpr.GetFloat() * dtime;
		uint n = (uint)slot.t;
		if (n > 0)
		{
			slot.t -= n;
			n = m_water.Get(n, false);
			slot.num += n;
			if (slot.num >= slot.req || m_water.GetNum() == 0)
			{
				//slot.troll->SurIn(EBS_Water, slot.num);
				m_worked.Remove(slot);
				i--;
			}
		}
	}
	// update vody
	m_kap += dtime * v_speed.GetFloat();
	uint kap = (uint)m_kap;
	if (kap > 0)
	{
		m_kap -= kap;
		m_water.Add(&kap,v_max.GetInt()-m_water.GetNum());
	}
}

bool WaterHole::Select()
{
	//SourceBuilding::Select();
	GetLevel()->GetPanel()->SetObjectHud("scripts/waterhole.menu", this);	
	GetLua()->func("s_studna");
	return true;
}

const char * WaterHole::BuildPlace(float x, float y)
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
	// muze se postavit, ted jen najit zdroj
	SystemObjectWater * source = NULL;
	float maxdist = 0.f;
	for (int i=0; i < GetLevel()->GetNumSysObj();i++)
	{
		BecherSystemObject * o = GetLevel()->GetSysObj(i);
		if (o->GetType()!=EBSys_Water)
			continue;
		SystemObjectWater * sw = dynamic_cast<SystemObjectWater*>(o);
		float d = sw->GetDistance(GetPosX(),GetPosY());
		if (d > maxdist)
		{
			maxdist = d;
			source = sw;
		}
	}	
	if (source == NULL)
	{
		GetCtrl()->SetOverColor(0xffff0000);
		return GetLang()->GetString(103);
	}

	// nejaka voda je
	byte c = (byte)(0xff * maxdist);
	GetCtrl()->SetOverColor(0xff000000 | (0xff-c) << 8 | c);
	return NULL;
}

bool WaterHole::SetToGet(Troll * t, uint num)
{
	// nalezt volny slot
	if (m_worked.Count()>=(uint)v_numworks.GetInt())
		return false;

	TTrollWorkSlot slot;
	slot.troll = t;
	slot.req = num;
	slot.num = 0;
	slot.t = 0.f;
	m_worked.Add(slot);
	return true;
}

#else // BECHER_OBJECT
void WaterHole::Update(const float t)
{
}

bool WaterHole::Select()
{
	//SourceBuilding::Select();
	return true;
}

void WaterHole::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



