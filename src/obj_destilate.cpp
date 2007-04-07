
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_destilate.h"
#include "obj_construct.h"

static CVar v_numzpr("dest_speed", 1.f, TVAR_SAVE); // rychlost zpracovani
static CVar v_sklad("dest_max", 500, TVAR_SAVE); // max. velikost miniskladu
static CVar v_cost("dest_cost", 180, TVAR_SAVE); // cena za stavbu
static CVar v_cost_wood("dest_cost_wood", 40, TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cost_stone("dest_cost_stone", 30, TVAR_SAVE); // pocet kameni potrebneho na stavbu
static CVar v_numworks("dest_maxwork", 4, TVAR_SAVE); // maximalni pocet pracujicich
static CVar v_recept("dest_recept", "1.0:S1=1", TVAR_SAVE); // recept pro jednu davku
static CVar v_build("dest_build", "1.4:K1+D1=0.011", TVAR_SAVE); // recept pro staveni
static CVar v_coalmax("dest_coal_max", 120, TVAR_SAVE); // maximalni kapacita pro uhli

////////////////////////////////////////////////////////
Destilate::Destilate(IHoeScene * scn) 
  : FactoryBuilding(scn, v_recept, v_build), 
  m_alco(EBS_Alco), m_sugar(EBS_Sugar)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_DESTILATE));
	//SetRingParam(4.5f, 4.5f, 2.f);
	//GetCtrl()->SetFlags(HOF_SHOW);
	//m_mode = wmIn;
	m_sugar.SetOwner(this);
	m_alco.SetOwner(this); 

	//m_part.emitor = (IHoeParticleEmitor*)GetEngine()->Create("particle");
	//m_part.pos.Set(4.f,13.f,-14.f);
	//GetCtrl()->Link(THoeSubObject::Particle, &m_part);
	CRR::Get()->Register(&m_alco);
	m_it.Start(v_idiottime, true);
}

Destilate::~Destilate()
{
}

bool Destilate::Save(ChunkDictWrite &w)
{
	FactoryBuilding::Save(w);
	return true;
}

bool Destilate::Load(const ChunkDictRead &r)
{
	FactoryBuilding::Load(r);
	OnUpdateSur();
	return true;
}

int Destilate::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
	case BINFO_NumAlco:
		ret = (int)this->m_alco.GetNum();
		break;
	case BINFO_NumSugar:
		ret = (int)this->m_sugar.GetNum();
		break;
	case BINFO_CanSugar:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Sugar, this);
			if (ri) ret = ri->GetAvail();
		} break;
	default:
		return FactoryBuilding::GetInfo(type, str, n);
	};
	if (str)
		snprintf(str, n, "%d", ret);
	return ret;
}

int Destilate::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Select:
		Select();
		break;
	case BMSG_InsertSur: {
		PAR_Load * l = (PAR_Load *)par2;
		if (l->sur == EBS_Sugar)
			m_sugar.Add((uint*)&l->num, 1000);
		else
			break;
		}
		return 0;
	case BMSG_GetSur: {
		PAR_Load * l = (PAR_Load *)par2;
		if (l->sur == EBS_Alco)
		{
			m_alco.Unlock(par1);
			return this->m_sugar.Get(l->num, true);
		}
		} break;
	case BMSG_LockSur: {
			PAR_Favour * f = (PAR_Favour *)par2;
			if (f->sur == EBS_Alco)
				return m_alco.Lock(f->num);
			break;
		} 
	}
	return FactoryBuilding::GameMsg(msg, par1, par2, npar2);
}

void Destilate::SetMode(EBuildingMode mode)
{
	// odmazat
	if (mode == m_mode)
		return;
	// pri buildingu nastavit kolize
	switch (m_mode)
	{
	case EBM_Build:
		GetCtrl()->SetOverColor(0xffffffff);
		break;
	case EBM_Select:
		GetCtrl()->SetOverColor(0xffffffff);
		break;
	case EBM_Normal:
		CRR::Get()->Unregister(&m_alco);
		break;
	};
	m_mode = mode;
	switch (mode)
	{
	case EBM_Build:
		GetCtrl()->SetOverColor(0xffffff00);
		break;
	case EBM_Normal:
		Show(true);
		CRR::Get()->Register(&m_alco);
		break;
	};
}

#ifndef BECHER_EDITOR

void Destilate::Update(const float t)
{
	if (m_chief.GetNumWorkers(EBW_Work) > 0 
            && !InBuildProcess() && m_work.BeginPass(m_chief.GetNumWorkers(EBW_Work), t))
	{
		m_work << m_sugar;
		m_work >> m_alco;
		m_work.Commit();
	}

	// update
	if (m_it.Update(t))
	{
        // pousti se po urcitem case, kdyz se zmeni personalie, pusti se hned,
        // ale cas se vyresetuje zase na zacatek
		if (InBuildProcess())
			IdiotBuild();
		else
			Idiot();
	}
	FactoryBuilding::Update(t);
}

void Destilate::Idiot() 
{
	HoeGame::LuaFunc f(GetLua(), "i_alco");
    f.PushPointer((BecherObject*)this);
	f.PushTable();
	f.SetTableInteger("alco", m_alco.GetNum());
	f.SetTableInteger("sugar", m_sugar.GetNum());
	f.SetTableInteger("coal", m_coal.GetNum());
	f.Run(2);
}

bool Destilate::Select()
{
	//BecherBuilding::Select();
	GetLevel()->GetPanel()->SetObjectHud("scripts/alco.menu",this);
	GetLua()->func("s_lihovar");
	return true;
}

#else

void Destilate::Update(const float t)
{
}

bool Destilate::Select()
{
	//FactoryBuilding::Select();
	/*GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Store"));
	GetProp()->AppendLong(6, _("Limit"), v_sklad.GetInt());
	GetProp()->End();*/	
	return true;
}

#endif





