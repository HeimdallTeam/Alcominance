
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_farm.h"

static CVar v_speed("farm_speed", 0.02f, TVAR_SAVE); // rychlost rustu
static CVar v_farma("farm_cost", 130, TVAR_SAVE);
static CVar v_farma_drevo("farm_cost_wood", 60, TVAR_SAVE);
static CVar v_farma_kamen("farm_cost_stone", 20, TVAR_SAVE);
static CVar v_num("farm_num", 20, TVAR_SAVE); // pocet trtiny z urody

#ifndef BECHER_EDITOR
FarmStatic Farm::m_userhud;

FarmStatic::FarmStatic()
{
	m_act = NULL;
}

void FarmStatic::SetAct(Farm * act)
{
	m_act = act;
	// pripojit 
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("produkce", HoeGame::Gui::EText))->SetText(m_produkce);
}

void FarmStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		//sprintf(m_sugarinfo,"%d cukru.", m_act->m_sugar.GetNum());
		sprintf(m_produkce,"Produkce je z %d%% hotova.", int(m_act->m_grow * 100.f));
		ObjectHud::Draw(h2d);
	}
}
#endif // BECHER_EDITOR

////////////////////////////////////////////////////////////
Farm::Farm(IHoeScene * scn) : SourceBuilding(scn), m_cane(EBS_Cane)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_FARM));
	SetRingParam(4.5f,3.5f,2.f);
	m_cane.SetOwner(this); CRR::Get()->Register(&m_cane);
	//worked = NULL;
	//rust = 0.f;
	//skliz = 0;
	memset(&m_growinfo,0, sizeof(m_growinfo));
	m_growinfo.model = (IHoeModel*)GetResMgr()->ReqResource(model_FARM_POLE);
	m_growinfo.pos.Translate(0, -5.f,0);
	GetCtrl()->Link(THoeSubObject::Object, &m_growinfo);
	m_grow = 0.f;
	m_growinfo.pos.Translate(0,5.f * m_grow - 5.f,0);
	m_work = NULL;
}

bool Farm::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	// ulozit rust
	m_cane.Save(w);
	w.Write<float>(m_grow);
	w.WriteValue<uint>(m_work ? m_work->GetID():0);
	w.WriteReservedWords(5);
	return true;
}

bool Farm::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	m_cane.Load(r);
	m_grow = r.Read<float>();
	m_growinfo.pos.Translate(0,5.f * m_grow - 5.f,0);
	uint it = r.Read<uint>();
	m_work = dynamic_cast<Troll*>(GetLevel()->GetObjFromID(it));
	r.ReadReservedWords(5);
	// 
	OnUpdateSur();
	return true;
}

#ifndef BECHER_EDITOR

ResourceBase * Farm::GetResource(ESurType type)
{
	switch (type)
	{
	case EBS_Cane:
		return &m_cane;
	default:
		return NULL;
	};
}

void Farm::Update(const float dtime)
{
	BecherObject::Update(dtime);
	{
		m_grow += v_speed.GetFloat() * dtime;
		if (m_grow >= 1.f)
		{
			m_cane.SetNum(v_num.GetInt());
			m_grow = 0.f;
		}
	}
	m_growinfo.pos.Translate(0,5.f * m_grow - 5.f,0);
}

const char * Farm::BuildPlace(float x, float y)
{
	return BecherBuilding::BuildPlace(x,y, (IHoeModel*)GetResMgr()->ReqResource(model_FARM), 4.f, 200.f); 
	// pozice v mape
}

bool Farm::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool Farm::SetToWork(Troll * t)
{
	if (m_work == NULL)
	{
		m_work = t;
		return true;
	}
	return false;
}

void Farm::UnsetFromWork(Troll * t)
{
	assert(t == m_work);
	m_work = NULL;
}

bool Farm::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	GetLua()->func("s_farma");
	return true;
}

bool Farm::Idiot(TJob *t)
{
	// pokud nemuze pracovat tak upnout do prace, jinak neupnout
	if (m_work)
		return false;
	t->type = TJob::jtGotoWork;
	t->owner = this;

	return true;
}


#else // BECHER_OBJECT

bool Farm::Select()
{
	SourceBuilding::Select();
	return true;
}

void Farm::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



