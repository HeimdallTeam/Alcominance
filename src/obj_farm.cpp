
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_farm.h"

static CVar v_speed("farm_speed", 0.02f, 0); // rychlost rustu
static CVar v_num("farm_num", 20, 0); // pocet trtiny z urody

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
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("produkce", HoeGame::Gui::EText))->SetPtr(m_produkce);
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
	m_infoselect.s_x = 4.5f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 3.5f;
	m_cane.SetOwner(this); CRR::Get()->Register(&m_cane);
	//worked = NULL;
	//rust = 0.f;
	//skliz = 0;
	memset(&m_growinfo,0, sizeof(m_growinfo));
	m_growinfo.model = (IHoeModel*)GetResMgr()->ReqResource(model_FARM_POLE);
	m_growinfo.s_x = 1.f;
	m_growinfo.s_y = 1.f;
	m_growinfo.s_z = 1.f;
	m_growinfo.t_y = -5.f;
	GetCtrl()->Link(THoeSubObject::Object, &m_growinfo);
	m_grow = 0.f;
	m_growinfo.t_y = 5.f * m_grow - 5.f;
}

bool Farm::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteReservedWords(10);
	return true;
}

/*bool Farm::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(10);
	OnUpdateSur();
	return true;
}*/

#ifndef BECHER_EDITOR

void Farm::Update(const float dtime)
{
	/*static bool hl = false;
	if (!hl)
	{
		GetBecher()->GetInfoPanel()->Add("Farma potrebuje delnika!");
		hl = true;
	}
	if (worked && worked->GetActJob() == Job::jtWork)
	{
		rust += (float)dtime * v_rust.GetFloat();
		if (rust > 1.f)
		{
			skliz = 50;
			rust = 0;
			Idiot(worked);
		}
	}*/
	//if (worked) // roste
	{
		m_grow += v_speed.GetFloat() * dtime;
		if (m_grow >= 1.f)
		{
			m_cane.SetNum(v_num.GetInt());
			m_grow = 0.f;
		}
	}
	m_growinfo.t_y = 5.f * m_grow - 5.f;
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
	if (!IsBuildMode())
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



