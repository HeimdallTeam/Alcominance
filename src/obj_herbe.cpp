
#include "StdAfx.h"
#include "becher.h"
#include "obj_herbe.h"

static CVar v_time("herbe_speed", 30, TVAR_SAVE); // za jak dlouho prileti baba
/*
static CVar v_cena("herbe_cost", , TVAR_SAVE); // cena za stavbu
static CVar v_cena("herbe_cost_wood", , TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cena("herbe_cost_stone", , TVAR_SAVE); // pocet kameni potrebneho na stavbu
*/
static CVar v_num("herbe_num", 3, TVAR_SAVE); // pocet trtiny z urody

#ifndef BECHER_EDITOR
HerbeWomanStatic HerbeWoman::m_userhud;

HerbeWomanStatic::HerbeWomanStatic()
{
	m_act = NULL;
}

void HerbeWomanStatic::SetAct(HerbeWoman * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetText(m_trtinainfo);
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetText(m_sugarinfo);
}

void HerbeWomanStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		//sprintf(m_sugarinfo,"%d cukru.", m_act->m_sugar.GetNum());
		//sprintf(m_trtinainfo,"%d trtina.", m_act->m_cane.GetNum());
		ObjectHud::Draw(h2d);
	}
}
#endif // BECHER_EDITOR

float getheight(IHoeModel *);

////////////////////////////////////////////////////////////
HerbeWoman::HerbeWoman(IHoeScene * scn) : SourceBuilding(scn), m_herbe(EBS_Herbe)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_BABA));
	m_infoselect.s_x = 1.4f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 2.f;
	m_info.model = NULL;
	m_info.t_x = 0.f;
	m_info.t_y = getheight(this->GetModel()) + 3.f;
	m_info.t_z = 0.f;
	m_info.s_x = 2.f;
	m_info.s_y = 4.f;
	m_info.s_z = 2.f;
	m_info.rotate = true;
	GetCtrl()->Link(THoeSubObject::Object, &m_info);
	m_herbe.SetOwner(this); CRR::Get()->Register(&m_herbe);
	m_wait = (float)v_time.GetInt();
}

bool HerbeWoman::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	m_herbe.Save(w);
	w.Write<float>(m_wait);
	w.WriteValue<dword>(m_ocup ? 1:0);
	w.WriteReservedWords(8);
	return true;
}

bool HerbeWoman::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	m_herbe.Load(r);
	m_wait = r.Read<float>();
	m_ocup = r.Read<dword>() != 0;
	r.ReadReservedWords(8);
	OnUpdateSur();
	return true;
}

#ifndef BECHER_EDITOR

const char * HerbeWoman::BuildPlace(float x, float y)
{
	return "";
}

void HerbeWoman::Update(const float dtime)
{
	if (m_wait > 0.f)
	{
		m_wait -= dtime;
		if (m_wait <= 0.f)
		{
			// baba prileti
			m_herbe.SetNum(v_num.GetInt());
			m_wait = 0.f;
		}
	}
}

bool HerbeWoman::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	GetLua()->func("s_herbe");
	return true;
}

bool HerbeWoman::Idiot(TJob *t)
{
	return false;
}

bool HerbeWoman::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool HerbeWoman::SetToWork(Troll * t)
{
	return false;
}

void HerbeWoman::UnsetFromWork(Troll * t)
{
}

#else // BECHER_OBJECT

bool HerbeWoman::Select()
{
	SourceBuilding::Select();
	return true;
}

void HerbeWoman::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT

void HerbeWoman::OnUpdateSur()
{
	static IHoeModel * m = dynamic_cast<IHoeModel*>(GetEngine()->Create("model miska"));
	if (m_herbe.GetNum() > 0)
		m_info.model = m;
	else
		m_info.model = NULL;
	if (m_herbe.GetNum() == 0)
		m_wait = (float)v_time.GetInt();

}




