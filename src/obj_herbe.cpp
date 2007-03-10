
#include "StdAfx.h"
#include "becher.h"
#include "obj_herbe.h"

static CVar v_time("herbe_speed", 30, TVAR_SAVE); // za jak dlouho prileti baba
/*
static CVar v_cena("herbe_cost", , TVAR_SAVE); // cena za stavbu
static CVar v_cena("herbe_cost_wood", , TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cena("herbe_cost_stone", , TVAR_SAVE); // pocet kameni potrebneho na stavbu
*/
static CVar v_num("herbe_num", 10, TVAR_SAVE); // pocet bylin z urody

float getheight(IHoeModel *);

////////////////////////////////////////////////////////////
HerbeWoman::HerbeWoman(IHoeScene * scn) : SourceBuilding(scn), m_herbe(EBS_Herbe), m_jaga(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_BABA));
	SetRingParam(1.4f,2.f,2.f);
	m_info.model = NULL;
	m_info.pos.Scale(2.f,4.f,2.f);
	HoeMath::Matrix a;
	a.Translate(0.f,getheight(this->GetModel()) + 3.f,0.f);
	m_info.pos.Multiply(a);
	GetCtrl()->Link(THoeSubObject::Object, &m_info);
	m_herbe.SetOwner(this); CRR::Get()->Register(&m_herbe);
	m_wait = (float)v_time.GetInt();

	static THoeSub_Particle part;
	part.emitor = (IHoeParticleEmitor*)GetEngine()->Create("particle");
	part.pos.Set(-6.681629, 23.211294, 17.630821);
	GetCtrl()->Link(THoeSubObject::Particle, &part);
	part.emitor->Start();

	m_jaga.GetCtrl()->SetPosition(HoeMath::Vector3(0,30,0));
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

	// nastaveni pozice
	//HoeMath::Vector3 p = GetCtrl()->GetPosition() + HoeMath::Vector3(-6.681629, 23.211294, 17.630821);
	//m_jaga.GetCtrl()->SetPosition(p);
	return true;
}

#ifndef BECHER_EDITOR

void HerbeWoman::Update(const float dtime)
{
	// update baba
	m_jaga.Update(dtime);

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
	return true;
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

int HerbeWoman::GameMsg(int msg, int par1, void * par2, uint npar2)
{
#ifndef BECHER_EDITOR
	switch (msg)
	{
	case BMSG_Select:
		GetLevel()->GetPanel()->SetObjectHud("scripts/herbe.menu",this);
		GetLua()->func("s_herbe");
		break;
	};
#endif
	return BecherBuilding::GameMsg(msg, par1, par2, npar2);
}

//////////////////////////////////////////////////////
float g_angl = 0.f;

BabaJaga::BabaJaga(IHoeScene * scn) : BaseObject(scn)
{
	// nastaveni modelu
	// ifndef editor
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_BABAJAGA));

	Show(true);
}

void BabaJaga::Update(const float dtime)
{
	g_angl += dtime;
	this->SetPosition(sinf(g_angl)*100,30,cosf(g_angl)*100);
	SetOrientation(0,1,0,-g_angl+HoeMath::HOE_PIF/2);

}


