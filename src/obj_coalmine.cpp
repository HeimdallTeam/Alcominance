
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

#ifndef BECHER_EDITOR
CoalMineStatic CoalMine::m_userhud;

CoalMineStatic::CoalMineStatic()
{
	m_act = NULL;
}

void CoalMineStatic::SetAct(CoalMine * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetText(m_trtinainfo);
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetText(m_sugarinfo);
}

void CoalMineStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		//sprintf(m_sugarinfo,"%d cukru.", m_act->m_sugar.GetNum());
		//sprintf(m_trtinainfo,"%d trtina.", m_act->m_cane.GetNum());
		ObjectHud::Draw(h2d);
	}
}
#endif // BECHER_EDITOR

////////////////////////////////////////////////////////////
CoalMine::CoalMine(IHoeScene * scn) : SourceBuilding(scn), m_coal(EBS_Coal)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_COALMINE));
	m_coal.SetOwner(this); CRR::Get()->Register(&m_coal);
	m_coal.SetPriority(EBSP_TimeWork);
}

bool CoalMine::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	m_coal.Save(w);
	w.WriteReservedWords(9);
	return true;
}

bool CoalMine::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	m_coal.Load(r);
	r.ReadReservedWords(9);
	return true;
}

#ifndef BECHER_EDITOR

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
				slot.troll->SurIn(EBS_Coal, slot.num);
				m_worked.Remove(slot);
				i--;
			}
		}
	}
}

bool CoalMine::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	GetLua()->func("s_coalmine");
	return true;
}

bool CoalMine::Idiot(TJob *t)
{
	// idiot bude vzdycku false,protoze pro dul nikdo nepracuje
	return false;
}

bool CoalMine::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool CoalMine::SetToWork(Troll * t)
{
	return false;
}

void CoalMine::UnsetFromWork(Troll * t)
{
}

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
	SourceBuilding::Select();
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



