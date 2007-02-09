
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_sugar.h"
#include "obj_construct.h"

static CVar v_cost("sugar_cost", 160, TVAR_SAVE);
static CVar v_cost_wood("sugar_cost_wood", 30, TVAR_SAVE);
static CVar v_cost_stone("sugar_cost_stone",40, TVAR_SAVE);
static CVar v_sklad("sugar_max", 5000, TVAR_SAVE); // maximalni velikost miniskladu
static CVar v_numworks("sugar_maxwork", 4, TVAR_SAVE); // maximalni pocet pracujicich
static CVar v_recept("sugar_recept", "1.2:C1=1", TVAR_SAVE); // recept pro jednu davku
static CVar v_build("sugar_build", "1.4:K1+D1=0.011", TVAR_SAVE); // recept pro staveni
static CVar v_coalmax("sugar_coal_max", 100, TVAR_SAVE); // maximalni kapacita pro uhli

static CVar v_autowork("sugar_auto", 0.f, TVAR_SAVE);

////////////////////////////////////////////////////////
Sugar::Sugar(IHoeScene * scn) : FactoryBuilding(scn, v_build), 
	m_build(&v_recept), m_cane(EBS_Cane),m_sugar(EBS_Sugar)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_SUGAR));
	SetRingParam(4.5f, 4.5f, 2.f);
	//m_w.SetOwner(this);
	m_cane.SetOwner(this);
	m_sugar.SetOwner(this);

	m_part.emitor = (IHoeParticleEmitor*)GetEngine()->Create("particle");
	m_part.pos.Set(-14.f, 23.f, 10.f);
	GetCtrl()->Link(THoeSubObject::Particle, &m_part);

	m_it.Start(v_idiottime, true);

	CRR::Get()->Register(&m_sugar);
}

Sugar::~Sugar()
{
}

bool Sugar::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteReservedWords(10);
	return true;
}

bool Sugar::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(10);
	OnUpdateSur();
	return true;
}

void Sugar::SetMode(EBuildingMode mode)
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
		CRR::Get()->Unregister(&m_sugar);
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
		CRR::Get()->Register(&m_sugar);
		break;
	};
}

int Sugar::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
	case BINFO_NumCane:
		ret = (int)this->m_cane.GetNum();
		break;
	case BINFO_NumSugar:
		ret = (int)this->m_sugar.GetNum();
		break;
	case BINFO_NumCoal:
		ret = (int)this->m_coal.GetNum();
		break;
	default:
		return FactoryBuilding::GetInfo(type, str, n);
	};
	if (str)
		snprintf(str, n, "%d", ret);
	return ret;
}

int Sugar::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Select:
		Select();
		break;
	case BMSG_SelectPlace:
	case BMSG_StartBuilding:
		return BuildPlace((float*)par2, 
			(IHoeModel*)GetResMgr()->ReqResource(model_SUGAR),50.f,200.f,msg==BMSG_StartBuilding);
	case BMSG_InsertSur: {
		PAR_Load * l = (PAR_Load *)par2;
		if (l->sur == EBS_Cane)
			m_cane.Add((uint*)&l->num, 1000);
		else
			break;
		}
		return 0;
	case BMSG_GetSur: {
		PAR_Load * l = (PAR_Load *)par2;
		if (l->sur == EBS_Sugar)
		{
			m_sugar.Unlock(par1);
			return this->m_sugar.Get(l->num, true);
		}
		} break;
	case BMSG_LockSur: {
			PAR_Favour * f = (PAR_Favour *)par2;
			if (f->sur == EBS_Sugar)
				return m_sugar.Lock(f->num);
			break;
		} 
	}
	return FactoryBuilding::GameMsg(msg, par1, par2, npar2);
}

#ifndef BECHER_EDITOR

bool Sugar::InsertSur(ESurType type, uint *s)
{
	switch (type)
	{
	case EBS_Cane:
		return m_cane.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
	case EBS_Coal:
		//return m_w.Add(s,*s);
	// vlozit drevo a sutr
	default:
		assert(!"insert bad type");
		return false;
	};
}

bool Sugar::SetToWork(Troll * t)
{
    /*switch (t->GetJob().type)
	{
    case TJob::jtWork:
	    //if (m_worked.Count() >= (uint)v_numworks.GetInt()) return false;
	    //m_worked.Add(t);
        break;
    case TJob::jtGotoRes:
        switch (t->GetJob().surtype){
        case EBS_Coal:
            //m_wrk_coal++;
            break;
        case EBS_Cane:
            //m_wrk_cane++;
            break;
        }
        break;
    }*/
	return true;
}

void Sugar::UnsetFromWork(Troll * t)
{
    /*switch (t->GetJob().type){
    case TJob::jtWork:
	    //m_worked.Remove(t);
        break;
    case TJob::jtGotoRes:
        switch (t->GetJob().surtype){
        case EBS_Coal:
            //m_wrk_coal--;
            break;
        case EBS_Cane:
            //m_wrk_cane--;
            break;
        }
    }*/
}

void Sugar::Update(const float t)
{
	if (1)
	{
		if (this->InBuildProcess())
		{
			UpdateBuild(t);
		}
		else if (m_build.BeginPass(m_chief.GetNumWorkers(EBW_Work)+v_autowork.GetFloat(), t))
		{
			m_build << m_cane;
			m_build >> m_sugar;
			m_build.Commit();
		}
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


bool Sugar::Select()
{
	BecherBuilding::Select();
	if (m_buildprogress < 1.0f)
		GetLevel()->GetPanel()->SetObjectHud("scripts/sugar_build.menu", this);	
	else
		GetLevel()->GetPanel()->SetObjectHud("scripts/sugar.menu", this);	
	GetLua()->func("s_cukr");
	return true;
}

bool Sugar::Idiot(TJob * job)
{
    return false;
}

void Sugar::Idiot() 
{
	HoeGame::LuaFunc f(GetLua(), "i_sugar");
    f.PushPointer((BecherObject*)this);
	f.PushTable();
	f.SetTableInteger("cane", m_cane.GetNum());
	f.SetTableInteger("sugar", m_sugar.GetNum());
	f.SetTableInteger("coal", m_coal.GetNum());
	f.Run(2);
}

#else

bool Sugar::Select()
{
	BecherObject::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Store"));
	GetProp()->AppendLong(6, _("Limit"), v_sklad.GetInt());
	GetProp()->End();	
	return true;
}

void Sugar::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 6:
		v_sklad.Set((int)pi.GetLong());
		break;
	};
}

#endif

