
#include "StdAfx.h"
#include "becher.h"
#include "obj_waterhole.h"
#include "troll.h"
#include "sysobjs.h"

static CVar v_numzpr("water_mine", 1.f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_cena("water_cost", 80, TVAR_SAVE);
static CVar v_build("water_build", "1.4:K1+D1=0.011", TVAR_SAVE); // recept pro staveni

static CVar v_numworks("water_maxwork", 1, TVAR_SAVE);
static CVar v_speed("water_speed", 0.21f, TVAR_SAVE); // rychlost pricitani vody
static CVar v_max("water_max", 50, TVAR_SAVE);

/*
Zasoby vody ve studni se ridi podle vodnych zdroju

*/

////////////////////////////////////////////////////////////
WaterHole::WaterHole(IHoeScene * scn)
	: SourceBuilding(scn, v_build), m_water(EBS_Water)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_WATERHOLE));
	m_water.SetOwner(this); CRR::Get()->Register(&m_water);
	m_water.SetPriority(EBSP_TimeWork);
	m_water.SetNum(v_max.GetInt());
	m_kap = 0.f;
}

bool WaterHole::Save(ChunkDictWrite &w)
{
	SourceBuilding::Save(w);
	return true;
}

bool WaterHole::Load(const ChunkDictRead &r)
{
	SourceBuilding::Load(r);
	OnUpdateSur();
	return true;
}

void WaterHole::Update(const float dtime)
{
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
	m_kap += dtime * v_speed.GetFloat(); // jeste vynasobeno silou zdroje
	uint kap = (uint)m_kap;
	if (kap > 0)
	{
		m_kap -= kap;
		m_water.Add(&kap,v_max.GetInt()-m_water.GetNum());
	}
}

bool WaterHole::Select()
{
#ifndef BECHER_EDITOR
	//SourceBuilding::Select();
	GetLevel()->GetPanel()->SetObjectHud("scripts/waterhole.menu", this);	
	GetLua()->func("s_studna");
#endif
	return true;
}

// system pro hledani mista na stavbu


int WaterHole::StatusPlace(float *pos)
{
	int ret = SourceBuilding::StatusPlace(pos);
	if (ret) return ret;

	// zkontrolovat zdroj vody
	SystemObjectWater * source = NULL;
	float maxdist = 0.f;
	// get source power
	for (int i=0; i < GetLevel()->GetNumSysObj();i++)
	{
		BecherSystemObject * o = GetLevel()->GetSysObj(i);
		if (o->GetType()!=EBSys_Water)
			continue;
		SystemObjectWater * sw = dynamic_cast<SystemObjectWater*>(o);
		float d = sw->GetSourcePower(GetPosX(),GetPosY());
		if (d > maxdist)
		{
			maxdist = d;
			source = sw;
		}
	}	
	if (source == NULL)
	{
		GetCtrl()->SetOverColor(0xffff0000);
		return 3;
	}

	// nejaka voda je
	byte c = (byte)(0xff * maxdist);
	GetCtrl()->SetOverColor(0xff000000 | (0xff-c) << 16 | c);
	return 0;
}

int WaterHole::GetSourcePower()
{
	// sila zdroje
	return 0;
}


/*bool WaterHole::SetToGet(Troll * t, uint num)
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
}*/

int WaterHole::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
	case BINFO_NumWater:
		ret = (int)this->m_water.GetNum();
		break;
	default:
		return SourceBuilding::GetInfo(type, str, n);
	};
	if (str)
		snprintf(str, n, "%d", ret);
	return ret;
}

int WaterHole::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Select:
		Select();
		break;
	};
	return SourceBuilding::GameMsg(msg, par1, par2, npar2);
}





