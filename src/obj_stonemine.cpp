
#include "StdAfx.h"
#include "becher.h"
#include "obj_stonemine.h"
#include "troll.h"

static CVar v_numzpr("stone_mine", 1.f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_numworks("stone_maxwork", 2, TVAR_SAVE);
static CVar v_cena("stone_cost", 150, TVAR_SAVE); // cena za stavbu
/*
static CVar v_cena("stone_cost_wood", 60, TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cena("stone_cost_stone", 60, TVAR_SAVE); // pocet kameni potrebneho na stavbu
*/

////////////////////////////////////////////////////////////
StoneMine::StoneMine(IHoeScene * scn) : SourceBuilding(scn), m_stone(EBS_Stone)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_STONEMINE));
	m_stone.SetOwner(this); CRR::Get()->Register(&m_stone);
	m_stone.SetPriority(EBSP_TimeWork);
}

bool StoneMine::Save(ChunkDictWrite &w)
{
	BecherBuilding::Save(w);
	return true;
}

bool StoneMine::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(10);
	OnUpdateSur();
	return true;
}

#ifndef BECHER_EDITOR

void StoneMine::Update(const float dtime)
{
	for (uint i=0;i<m_worked.Count();i++)
	{
		TTrollWorkSlot &slot = m_worked.Get(i);
		slot.t += v_numzpr.GetFloat() * dtime;
		uint n = (uint)slot.t;
		if (n > 0)
		{
			slot.t -= n;
			n = m_stone.Get(n, false);
			slot.num += n;
			if (slot.num >= slot.req || m_stone.GetNum() == 0)
			{
				//slot.troll->SurIn(EBS_Stone, slot.num);
				m_worked.Remove(slot);
				i--;
			}
		}
	}
}

bool StoneMine::Select()
{
	//SourceBuilding::Select();
	GetLua()->func("s_stonemine");
	return true;
}

bool StoneMine::SetToGet(Troll * t, uint num)
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
void StoneMine::Update(const float t)
{
}

bool StoneMine::Select()
{
	//SourceBuilding::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Resources"));
	GetProp()->AppendLong(1, _("Stone"), m_stone.GetNum());
	GetProp()->End();
	return true;
}

void StoneMine::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 1:
		m_stone.SetNum((int)pi.GetLong());
		break;
	};
}


#endif // BECHER_OBJECT



