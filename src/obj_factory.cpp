
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_factory.h"

static CVar v_numzpr("factory_speed", 0.3f, TVAR_SAVE);
static CVar v_sklad("factory_max", 500, TVAR_SAVE);
static CVar v_cena("factory_cost", 1000, TVAR_SAVE);
static CVar v_cena_drevo("factory_cost_wood", 500, TVAR_SAVE);
static CVar v_cena_kamen("factory_cost_stone", 500, TVAR_SAVE);
static CVar v_numworks("factory_maxwork", 4, TVAR_SAVE);
static CVar v_recept("factory_recept", "3:S10+A10+H3+W20=1", 0); // recept pro jednu davku
static CVar v_build("factory_build", "1.4:K1+D1=0.011", TVAR_SAVE); // recept pro staveni
static CVar v_coalmax("coal_max", 200, TVAR_SAVE); // maximalni kapacita pro uhli


////////////////////////////////////////////////////////
Factory::Factory(IHoeScene * scn) : FactoryBuilding(scn, v_recept, v_build), 
 m_becher(EBS_Becher), m_water(EBS_Water), m_sugar(EBS_Sugar), m_alco(EBS_Alco),
 m_herbe(EBS_Herbe)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_FACTORY));
	//SetRingParam(6.7f,6.7f,2.f);
	m_sugar.SetOwner(this);
	m_alco.SetOwner(this);
	m_herbe.SetOwner(this);
	m_water.SetOwner(this);
	m_becher.SetOwner(this); 
    CRR::Get()->Register(&m_becher);
	m_it.Start(v_idiottime, true);

}

Factory::~Factory()
{
}

bool Factory::Save(ChunkDictWrite &w)
{
	FactoryBuilding::Save(w);
	return true;
}

bool Factory::Load(const ChunkDictRead &r)
{
	FactoryBuilding::Load(r);
	OnUpdateSur();
	return true;
}

int Factory::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
	case BINFO_NumSugar:
		ret = (int)this->m_sugar.GetNum();
		break;
	case BINFO_NumAlco:
		ret = (int)this->m_alco.GetNum();
		break;
	case BINFO_NumWater:
		ret = (int)this->m_water.GetNum();
		break;
	case BINFO_NumHerbe:
		ret = (int)this->m_herbe.GetNum();
		break;
	case BINFO_NumBecher:
		ret = (int)this->m_becher.GetNum();
		break;
	case BINFO_CanSugar:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Sugar, this);
			if (ri) ret = ri->GetAvail();
		} break;
	case BINFO_CanAlco:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Alco, this);
			if (ri) ret = ri->GetAvail();
		} break;
	case BINFO_CanWater:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Water, this);
			if (ri) ret = ri->GetAvail();
		} break;
	case BINFO_CanHerbe:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Herbe, this);
			if (ri) ret = ri->GetAvail();
		} break;
	default:
		return FactoryBuilding::GetInfo(type, str, n);
	};
	if (str)
		snprintf(str, n, "%d", ret);
	return ret;
}

int Factory::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Select:
		Select();
		break;
	case BMSG_InsertSur: {
		PAR_Load * l = (PAR_Load *)par2;
        switch (l->sur)
        {
        case EBS_Sugar:
            m_sugar.Add((uint*)&l->num, 1000);
            return 0;
        case EBS_Alco:
            m_alco.Add((uint*)&l->num, 1000);
            return 0;
         case EBS_Water:
            m_water.Add((uint*)&l->num, 1000);
            return 0;
         case EBS_Herbe:
            m_herbe.Add((uint*)&l->num, 1000);
            return 0;
		}
        } 
        break;
	}
	return FactoryBuilding::GameMsg(msg, par1, par2, npar2);
}

#ifndef BECHER_EDITOR

void Factory::Update(const float t)
{
	if (!InBuildProcess()
            && m_work.BeginPass(m_chief.GetNumWorkers(EBW_Work), t))
	{
		m_work << m_alco;
        m_work << m_sugar;
        m_work << m_water;
        m_work << m_herbe;
	    m_work >> m_becher;
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

void Factory::Idiot() 
{
	HoeGame::LuaFunc f(GetLua(), "i_factory");
    f.PushPointer((BecherObject*)this);
	f.PushTable();
	f.SetTableInteger("sugar", m_sugar.GetNum());
	f.SetTableInteger("alco", m_alco.GetNum());
	f.SetTableInteger("water", m_water.GetNum());
	f.SetTableInteger("herbe", m_herbe.GetNum());
	f.SetTableInteger("becher", m_becher.GetNum());
	f.SetTableInteger("coal", m_coal.GetNum());
	f.Run(2);
}

bool Factory::Select()
{
	//FactoryBuilding::Select();
	GetLevel()->GetPanel()->SetObjectHud("scripts/factory.menu", this);
	GetLua()->func("s_tovarna");
	return true;
}


#else
void Factory::Update(const float t)
{
}

bool Factory::Select()
{
	//FactoryBuilding::Select();
	/*GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Store"));
	GetProp()->AppendLong(6, _("Limit"), v_sklad.GetInt());
	GetProp()->End();*/	
	return true;
}

#endif

