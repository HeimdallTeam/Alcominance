
#include "StdAfx.h"
#include "becher.h"
#include "obj_coalmine.h"
#include "troll.h"

static CVar v_numzpr("coal_mine", 1.f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_cena("coal_cost" , 150, TVAR_SAVE);
/*
static CVar v_cena_drevo("coal_cost_wood" , 60, TVAR_SAVE);
static CVar v_cena_kamen("coal_cost_stone", 60, TVAR_SAVE);
*/
static CVar v_numworks("coal_maxwork", 2, TVAR_SAVE);
static CVar v_build("coal_build", "1.4:K1+D1=0.011", TVAR_SAVE); // recept pro staveni

////////////////////////////////////////////////////////////
CoalMine::CoalMine(IHoeScene * scn) : SourceBuilding(scn, v_build), m_coal(EBS_Coal)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_COALMINE));
	m_coal.SetOwner(this); CRR::Get()->Register(&m_coal);
	m_coal.SetPriority(EBSP_TimeWork);
}

bool CoalMine::Save(ChunkDictWrite &w)
{
	BecherBuilding::Save(w);
	m_coal.Save(w);
	return true;
}

bool CoalMine::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	m_coal.Load(r);
	r.ReadReservedWords(9);
	return true;
}

void CoalMine::Update(const float dtime)
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
			n = m_coal.Get(n, false);
			slot.num += n;
			if (slot.num >= slot.req || m_coal.GetNum() == 0)
			{
				//slot.troll->SurIn(EBS_Coal, slot.num);
				m_worked.Remove(slot);
				i--;
			}
		}
	}
}


int CoalMine::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		if (strcmp(str, "coal") == 0)
			type = BINFO_NumCoal;
	}
	switch (type)
	{
	case BINFO_NumCoal:
		ret = m_coal.GetNum();
		if (str)
			snprintf(str, n, "%d", ret);
		return ret;
	default:
		return BecherBuilding::GetInfo(type, str, n);
	};
	return 0;
}

int CoalMine::GameMsg(int msg, int par1, void * par2, uint npar2)
{
#ifndef BECHER_EDITOR
	switch (msg)
	{
	case BMSG_Select:
		GetLevel()->GetPanel()->SetObjectHud("scripts/mine.menu",this);
		GetLua()->func("s_coalmine");
		break;
	}
#endif
	return BecherBuilding::GameMsg(msg, par1, par2, npar2);
}

#ifndef BECHER_EDITOR

bool CoalMine::SetToGet(Troll * t, uint num)
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

bool CoalMine::Select()
{
	//SourceBuilding::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Resources"));
	GetProp()->AppendLong(1, _("Coal"), m_coal.GetNum());
	GetProp()->End();	
	return true;
}

void CoalMine::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 1:
		m_coal.SetNum((int)pi.GetLong());
		break;
	};
}

#endif // BECHER_OBJECT



