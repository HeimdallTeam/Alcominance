
#include "StdAfx.h"
#include "becher.h"
#include "obj_saw.h"

static CVar v_cost("saw_cost", 150, TVAR_SAVE); // cena za stavbu
static CVar v_cost_wood("saw_cost_wood", 80, TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cost_stone("saw_cost_stone", 10, TVAR_SAVE); // pocet kameni potrebneho na stavbu

#ifndef BECHER_EDITOR
SawStatic Saw::m_userhud;

SawStatic::SawStatic()
{
	m_act = NULL;
}

void SawStatic::SetAct(Saw * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetText(m_trtinainfo);
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetText(m_sugarinfo);
}

void SawStatic::Draw(IHoe2D * h2d)
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
Saw::Saw(IHoeScene * scn) : SourceBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_SAW));
}

bool Saw::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteReservedWords(10);
	return true;
}

bool Saw::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(10);
	OnUpdateSur();
	return true;
}

#ifndef BECHER_EDITOR

void Saw::Update(const float dtime)
{
}

bool Saw::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	GetLua()->func("s_saw");
	return true;
}

bool Saw::Idiot(TJob *t)
{
	return false;
}

bool Saw::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool Saw::SetToWork(Troll * t)
{
	return false;
}

void Saw::UnsetFromWork(Troll * t)
{
}

#else // BECHER_OBJECT

bool Saw::Select()
{
	SourceBuilding::Select();
	return true;
}

void Saw::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



