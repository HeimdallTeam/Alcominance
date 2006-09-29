
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_sugar.h"
#include "obj_construct.h"

static CVar v_numzpr("sugar_speed", 1.f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_cost("sugar_cost", 160, TVAR_SAVE);
static CVar v_cost_wood("sugar_cost_wood", 30, TVAR_SAVE);
static CVar v_cost_stone("sugar_cost_stone",40, TVAR_SAVE);
static CVar v_sklad("sugar_max", 50, TVAR_SAVE); // maximalni velikost miniskladu
static CVar v_numworks("sugar_maxwork", 4, TVAR_SAVE); // maximalni pocet pracujicich
static CVar v_recept("sugar_recept", "C1=1", TVAR_SAVE); // recept pro jednu davku
static CVar v_coalmax("coal_max", 100, TVAR_SAVE); // maximalni kapacita pro uhli

#ifndef BECHER_EDITOR
SugarStatic Sugar::m_userhud;

SugarStatic::SugarStatic()
{
	m_act = NULL;
}

void SugarStatic::SetAct(Sugar * act)
{
	m_act = act;
	// pripojit 
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetText(m_trtinainfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetText(m_sugarinfo);

}

void SugarStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		
		sprintf(m_sugarinfo,GetLang()->GetString(5), m_act->m_sugar.GetNum());
		sprintf(m_trtinainfo,GetLang()->GetString(6), m_act->m_cane.GetNum());

		ObjectHud::Draw(h2d);
	}
}
#endif // BECHER_EDITOR

////////////////////////////////////////////////////////
Sugar::Sugar(IHoeScene * scn) : FactoryBuilding(scn), m_sugar(EBS_Sugar)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_SUGAR));
	m_infoselect.s_x = 3.5f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 3.5f;
	//GetCtrl()->SetFlags(HOF_SHOW);
	//m_mode = wmIn;
	m_w.SetOwner(this);
	m_cane.SetOwner(this);
	m_sugar.SetOwner(this);

	m_part.emitor = (IHoeParticleEmitor*)GetEngine()->Create("particle");
	m_part.t_x = -14.f;
	m_part.t_y = 23.f;
	m_part.t_z = 10.f;
	GetCtrl()->Link(THoeSubObject::Particle, &m_part);

	m_w.SetRecept(&v_recept);
	m_progress = 0.f;
    m_wrk_cane = 0;
    m_wrk_coal = 0;
}

Sugar::~Sugar()
{
}

bool Sugar::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteReservedWords(10);
	if (m_construct)
		m_construct->Save(w);
	return true;
}

bool Sugar::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(10);
	if (m_construct)
		m_construct->Load(r);
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
		m_construct = NULL;
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
		m_construct = new Construct(this);
		m_construct->SetBuildTime(5.f);
		break;
	case EBM_Normal:
		Show(true);
		CRR::Get()->Register(&m_sugar);
		break;
	};
}

#ifndef BECHER_EDITOR

const char * Sugar::BuildPlace(float x, float y)
{
	// pozice v mape
	float min,max;
	bool ok;
	max = min = 0.f;
	ok = GetLevel()->GetScene()->GetScenePhysics()->GetCamber(x,x,y,y,min,max);
	SetPosition(x,y,min);
	if (!ok || (max-min) > 1.f) 
	{
		GetCtrl()->SetOverColor(0xffff0000);
		return GetLang()->GetString(101);
	}
	// zjistit zda muze byt cerveny nebo jiny
	for (int i=0; i < GetLevel()->GetNumObj();i++)
	{
		float x = GetLevel()->GetObj(i)->GetPosX();
		float y = GetLevel()->GetObj(i)->GetPosY();
		x -= GetPosX();
		y -= GetPosY();
		if (x*x+y*y < 4000.f)
		{
			GetCtrl()->SetOverColor(0xffff0000);
			return GetLang()->GetString(102);
		}
	}
	GetCtrl()->SetOverColor(0xffffffff);
	return NULL;
}

bool Sugar::InsertSur(ESurType type, uint *s)
{
	if (m_construct)
		return m_construct->InsertSur(type,s);
	switch (type)
	{
	case EBS_Cane:
		return m_cane.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
	case EBS_Coal:
		return m_w.Add(s,*s);
	// vlozit drevo a sutr
	default:
		assert(!"insert bad type");
		return false;
	};
}

bool Sugar::SetToWork(Troll * t)
{
	if (m_construct)
		return m_construct->SetToWork(t);
    switch (t->GetJob().type)
	{
    case TJob::jtWork:
	    if (m_worked.Count() >= (uint)v_numworks.GetInt()) return false;
	    m_worked.Add(t);
        break;
    case TJob::jtGotoRes:
        switch (t->GetJob().surtype){
        case EBS_Coal:
            m_wrk_coal++;
            break;
        case EBS_Cane:
            m_wrk_cane++;
            break;
        }
        break;
    }
	return true;
}

void Sugar::UnsetFromWork(Troll * t)
{
	if (m_construct)
		return m_construct->UnsetFromWork(t);
    switch (t->GetJob().type){
    case TJob::jtWork:
	    m_worked.Remove(t);
        break;
    case TJob::jtGotoRes:
        switch (t->GetJob().surtype){
        case EBS_Coal:
            m_wrk_coal--;
            break;
        case EBS_Cane:
            m_wrk_cane--;
            break;
        }
    }
}

void Sugar::Update(const float t)
{
	if (m_construct)
	{
		return m_construct->Update(t);
	}

	// update
	float prog = m_w.InProcess() ? m_worked.Count()*v_numzpr.GetFloat():0.f;

	if (m_worked.Count() > 0)
	{
		m_w.Update(t*prog);

		if (m_w.CanOut() && ((int)m_w.Out(false)<=(v_sklad.GetInt() - GetMiniStoreCount())))
		{
			uint p = m_w.Out(true);
			m_sugar.Add(&p, p);
		}

		// naplneni
		if (m_w.CanIn() && m_w.In(&m_cane, 'C', true))
		{
			m_w.ToProcess();
		}
	}

	if (m_progress != prog)
	{
		// update 
		m_progress = prog;
		// pokud neni progress a nemuze se delat
		if (m_progress > 0.f)
			m_part.emitor->Start();
		else
			m_part.emitor->Stop();
	}

	if (m_worked.Count() > 0)
	{
		if (prog > 0.f)
			m_exitdelay.Reset();
		else if (m_exitdelay.AddTime((const float)t, m_worked.Count() == 1 ? 3.f:1.f))
		{
			m_exitdelay.Reset();
					// propustit jednoho workera
			m_worked.OneStopWork();
		}
	}

}


bool Sugar::Select()
{
	FactoryBuilding::Select();
	if (m_construct)
		return m_construct->Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	GetLua()->func("s_cukr");
	return true;
}

bool Sugar::Idiot(TJob * j)
{
	if (m_construct)
		return m_construct->Idiot(j);
	// zjistit pripadny zdroj pro suroviny
	// 
	// navalit informace do tabulky, bud z crr nebo primo vybrane uloziste
	ResourceExp * ri = CRR::Get()->Find(EBS_Cane, this);
	ResourceExp * rc = CRR::Get()->Find(EBS_Coal, this);
	BecherBuilding * bout = CRR::Get()->FindAccept(EBS_Sugar, this);

	// najit sklad pro 
	HoeGame::LuaFunc f(GetLua(), "i_sugar");
	f.PushTable();
	
	f.SetTableInteger("max_store", v_sklad.GetInt());    

    // vstupni suroviny
	f.SetTableInteger("cane_avail", ri ? ri->GetAvail():0);
    f.SetTableInteger("cane_wrkcount", m_wrk_cane);
	f.SetTableInteger("cane", m_cane.GetNum());

    f.SetTableInteger("coal_avail", rc ? rc->GetAvail():0);
    f.SetTableInteger("coal_wrkcount", m_wrk_coal);
    f.SetTableInteger("coal", m_w.GetNum());
	f.SetTableInteger("coal_max", v_coalmax.GetInt());

    // vystupni suroviny
	f.SetTableInteger("sugar", m_sugar.GetNum());
	f.SetTableInteger("sugar_out", bout ? bout->AcceptSur(EBS_Sugar):0);

	// works
	f.SetTableInteger("works_count", this->m_worked.Count());
	f.SetTableInteger("works_max", v_numworks.GetInt());

	f.Run(1);
	if (f.IsNil(-1))
	{
		f.Pop(1);
		return false;
	}

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
	
	return true;
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

