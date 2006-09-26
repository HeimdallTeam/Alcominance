
#include "StdAfx.h"
#include "becher.h"
#include "obj_waterhole.h"
#include "troll.h"

static CVar v_numzpr("water_mine", 1.f, 0); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_numworks("water_maxwork", 1, 0);
static CVar v_speed("water_speed", 0.21f, 0); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_max("water_max", 50, 0);


#ifndef BECHER_EDITOR
WaterHoleStatic WaterHole::m_userhud;

WaterHoleStatic::WaterHoleStatic()
{
	m_act = NULL;
}

void WaterHoleStatic::SetAct(WaterHole * act)
{
	m_act = act;
	// pripojit 
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("voda", HoeGame::Gui::EText))->SetText(m_waterinfo);
}

void WaterHoleStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		sprintf(m_waterinfo,"%d vody.", m_act->m_water.GetNum());
		ObjectHud::Draw(h2d);
	}
}
#endif // BECHER_EDITOR


////////////////////////////////////////////////////////////
WaterHole::WaterHole(IHoeScene * scn) : SourceBuilding(scn), m_water(EBS_Water)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_WATERHOLE));
	m_infoselect.s_x = 1.f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 1.f;
	m_water.SetOwner(this); CRR::Get()->Register(&m_water);
	m_water.SetPriority(EBSP_TimeWork);
	m_water.SetNum(v_max.GetInt());
	m_kap = 0.f;
}

bool WaterHole::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteReservedWords(10);
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
				slot.troll->SurIn(EBS_Water, slot.num);
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
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_studna");
	return true;
}

bool WaterHole::Idiot(TJob *t)
{
	return false;
}

bool WaterHole::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool WaterHole::SetToWork(Troll * t)
{
	return false;
}

void WaterHole::UnsetFromWork(Troll * t)
{
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

bool WaterHole::Select()
{
	SourceBuilding::Select();
	return true;
}

void WaterHole::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



