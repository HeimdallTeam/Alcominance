
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
static CVar v_build("sugar_build", "1.4:K1+D1=0.01", TVAR_SAVE); // recept pro staveni
static CVar v_coalmax("sugar_coal_max", 100, TVAR_SAVE); // maximalni kapacita pro uhli
static CVar v_autowork("sugar_auto", 0.f, TVAR_SAVE);

CVar v_idiottime("time_idiot", 2.f, 0); // jak casto se ma poustet idiot

////////////////////////////////////////////////////////
Sugar::Sugar(IHoeScene * scn) : BecherBuilding(scn), 
	m_wbuild(&v_build), m_stone(EBS_Stone), m_wood(EBS_Wood),
	m_cane(EBS_Cane),m_sugar(EBS_Sugar), m_coal(EBS_Coal)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_SUGAR));
	SetRingParam(4.5f, 4.5f, 2.f);
	//m_w.SetOwner(this);
	m_cane.SetOwner(this);
	m_sugar.SetOwner(this);
	m_wood.SetOwner(this);
	m_stone.SetOwner(this);
	m_coal.SetOwner(this);

	// build
	m_buildprogress = 0;

	m_part.emitor = (IHoeParticleEmitor*)GetEngine()->Create("particle");
	m_part.pos.Set(-14.f, 23.f, 10.f);
	GetCtrl()->Link(THoeSubObject::Particle, &m_part);

	m_it.Start(v_idiottime, true);
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

uint ReqResource(const CVar & var, float progress, char r)
{
	const char * p = var.GetString();
	while (*p && *p!='=') p++;
	if (!*p)
		return 0;
	float dav = 0.f;
	if (sscanf(p+1, "%f", &dav)!=1 || dav == 0.f)
		return 0;
	p = var.GetString();
	while (*p && *p!=r) p++;
	if (!*p)
		return 0;
	int res = 0;
	if (sscanf(p+1, "%d", &res)!=1)
		return 0;
	float rem = (((1.f-progress)/dav)*res);
	if (rem < 0) return 0;
	return (uint)HoeMath::UpperRound(rem);
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
	case BINFO_NumStone:
		ret = (int)this->m_stone.GetNum();
		break;
	case BINFO_NumWood:
		ret = (int)this->m_wood.GetNum();
		break;
	case BINFO_NumCoal:
		ret = (int)this->m_coal.GetNum();
		break;
	case BINFO_ReqStone:
		ret = ReqResource(v_build, m_buildprogress, 'K') - m_stone.GetNum();
		break;
	case BINFO_ReqWood:
		ret = ReqResource(v_build, m_buildprogress, 'D') - m_wood.GetNum();
		break;
	case BINFO_CanStone:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Stone, this);
			if (ri) ret = ri->GetAvail();
		} break;
	case BINFO_CanWood:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Wood, this);
			if (ri) ret = ri->GetAvail();
		} break;
	case BINFO_BuildProgress:
		ret = (int)(this->m_buildprogress * 100);
		break;
	default:
		return BecherBuilding::GetInfo(type, str, n);
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
    case BMSG_Chief:
        if (npar2 == 1)
			m_chief.Make(this,reinterpret_cast<const char*>(par2));
		else
			for (int i=0;i < npar2;i++)
				m_chief.Make(this,reinterpret_cast<const char**>(par2)[i]);
        return 0;
    case BMSG_CreateImport: {
        // vytvoreni dodavky
        PAR_Favour * f = (PAR_Favour*)par2;
        hoe_assert(npar2 == 5);
        f->owner = this;
        // vyhledani zdroje
        ResourceExp * r = CRR::Get()->Find(f->sur, this);
        if (r == NULL) return 0;
        f->remote = r->GetOwner();
        // zalockovat??? asi jo
		f->locked = SendGameMsg(f->remote, BMSG_LockSur, 0, f, npar2);
        } return 1;
	case BMSG_TrollIncoming:
		m_chief.Incoming((Troll*)par2);
		return 0;
	case BMSG_InsertSur: {
		PAR_Load * l = (PAR_Load *)par2;
		if (l->sur == EBS_Wood)
			m_wood.Add((uint*)&l->num, 1000);
		else if (l->sur == EBS_Stone)
			m_stone.Add((uint*)&l->num, 1000);
		else if (l->sur == EBS_Coal)
			m_coal.Add((uint*)&l->num, 1000);
		else if (l->sur == EBS_Cane)
			m_cane.Add((uint*)&l->num, 1000);
		}
		return 0;
	}
	return BecherBuilding::GameMsg(msg, par1, par2, npar2);
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
		if (m_wbuild.BeginPass(m_chief.GetNumWorkers(EBW_Work)+v_autowork.GetFloat(), t))
		{
			m_wbuild << m_stone;
			m_wbuild << m_wood;
			m_wbuild >> m_buildprogress;
			m_wbuild.Commit();
		}
	}
	// update
	if (m_it.Update(t))
	{
        // pousti se po urcitem case, kdyz se zmeni personalie, pusti se hned,
        // ale cas se vyresetuje zase na zacatek
		Idiot();
	}

	/*if (m_worked.Count() > 0)
	{
		if (prog > 0.f)
			m_exitdelay.Reset();
		else if (m_exitdelay.AddTime((const float)t, m_worked.Count() == 1 ? 3.f:1.f))
		{
			m_exitdelay.Reset();
					// propustit jednoho workera
			m_worked.OneStopWork();
		}
	}*/

}


bool Sugar::Select()
{
	BecherBuilding::Select();
	GetLevel()->GetPanel()->SetObjectHud("scripts/sugar_build.menu", this);	
	GetLua()->func("s_cukr");
	return true;
}

bool Sugar::Idiot(TJob * job)
{
    return false;
}

void Sugar::Idiot()
{
	// zjistit pripadny zdroj pro suroviny
	// 
	// navalit informace do tabulky, bud z crr nebo primo vybrane uloziste
	//ResourceExp * ri = CRR::Get()->Find(EBS_Cane, this);
	//ResourceExp * rc = CRR::Get()->Find(EBS_Coal, this);
	//BecherBuilding * bout = CRR::Get()->FindAccept(EBS_Sugar, this);

	// najit sklad pro 
	HoeGame::LuaFunc f(GetLua(), "i_sugar");
    f.PushPointer((BecherObject*)this);
	f.PushTable();
	
	//f.SetTableInteger("max_store", v_sklad.GetInt());    

    // vstupni suroviny
	//f.SetTableInteger("cane_avail", ri ? ri->GetAvail():0);
    //f.SetTableInteger("cane_wrkcount", m_wrk_cane);
	f.SetTableInteger("cane", m_cane.GetNum());
	f.SetTableInteger("stone", m_stone.GetNum());
	f.SetTableInteger("wood", m_wood.GetNum());

    //f.SetTableInteger("coal_avail", rc ? rc->GetAvail():0);
    //f.SetTableInteger("coal_wrkcount", m_wrk_coal);
    f.SetTableInteger("coal", m_coal.GetNum());
	//f.SetTableInteger("coal_max", v_coalmax.GetInt());

    // vystupni suroviny
	f.SetTableInteger("sugar", m_sugar.GetNum());
	//f.SetTableInteger("sugar_out", bout ? bout->AcceptSur(EBS_Sugar):0);

	// works
	f.SetTableInteger("works_count", m_chief.GetNumWorkers(EBW_Work));
    f.SetTableInteger("works_cane", m_chief.GetNumWorkers(EBW_Import(EBS_Cane)));
    f.SetTableInteger("works_coal", m_chief.GetNumWorkers(EBW_Import(EBS_Coal)));
    f.SetTableInteger("works_sugar", m_chief.GetNumWorkers(EBW_Export(EBS_Sugar)));

	f.SetTableInteger("works_max", v_numworks.GetInt());

	f.Run(1);
	if (f.IsNil(-1))
	{
		f.Pop(1);
		return;
	}
    /*
	// prevest zpatky na job
	int r = f.GetTableInteger("type", -1); // typ prace
	j->percent = f.GetTableFloat("percent", -1); // na kolik procent je vyzadovano
	j->owner = this;
	switch (r)
	{
	case 0:
		j->surtype = (ESurType)f.GetTableInteger("sur", -1); // typ suroviny
		j->type = TJob::jtGotoRes;
		j->num = f.GetTableInteger("num", -1); // pocet k prineseni
		j->from = j->surtype == EBS_Cane ? ri:rc;
		j->to = this;
		break;
	case 1:
		j->type = TJob::jtGotoWork;
		break;
	case 2:
		j->type = TJob::jtGotoRes;
		j->surtype = EBS_Sugar;
		j->from = &m_sugar;
		j->num = f.GetTableInteger("num", -1); // pocet k prineseni
		j->to = bout; // sklad
		break;
	};
		
	f.Pop(1);

	// nebo lua bude posilat zpravy??
	// to by bylo lepsi.. mozna
	// propust, najmi, atd..
	// propust je jasny
	// ale to by vlastne mohla nejen lua posilat
	// lua by byl v podstate jen takovej automat
	// zpravy typu, propust workery -> pocet
	// nastav volnym workerum aby nosili neco
	// dej praci workerum
    */
}

#else

bool Sugar::Select()
{
	FactoryBuilding::Select();
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

