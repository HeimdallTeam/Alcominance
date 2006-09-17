
#include "StdAfx.h"
#include "becher.h"
#include "obj_coalmine.h"

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
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetPtr(m_trtinainfo);
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetPtr(m_sugarinfo);
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
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_COALMINE));
	m_coal.SetOwner(this); CRR::Get()->Register(&m_coal);
	m_coal.SetPriority(EBSP_TimeWork);
}

bool CoalMine::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteRI(m_coal);
	w.WriteReservedWords(9);
	return true;
}

bool CoalMine::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadRI(m_coal);
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
}

bool CoalMine::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_coalmine");
	return true;
}

bool CoalMine::Idiot(Job *t)
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



