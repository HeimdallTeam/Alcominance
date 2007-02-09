
#include "StdAfx.h"
#include "becher.h"
#include "obj_saw.h"

static CVar v_cost("saw_cost", 150, TVAR_SAVE); // cena za stavbu
static CVar v_cost_wood("saw_cost_wood", 80, TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cost_stone("saw_cost_stone", 10, TVAR_SAVE); // pocet kameni potrebneho na stavbu
static CVar v_build("saw_build", "1.4:K1+D1=0.011", TVAR_SAVE); // recept pro staveni

////////////////////////////////////////////////////////////
Saw::Saw(IHoeScene * scn) : ProductionBuilding(scn, v_build)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_SAW));
}

bool Saw::Save(BecherGameSave &w)
{
	ProductionBuilding::Save(w);
	w.WriteReservedWords(10);
	return true;
}

bool Saw::Load(BecherGameLoad &r)
{
	ProductionBuilding::Load(r);
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
	ProductionBuilding::Select();
	GetLua()->func("s_saw");
	return true;
}

#else // BECHER_OBJECT

bool Saw::Select()
{
	ProductionBuilding::Select();
	return true;
}

void Saw::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



