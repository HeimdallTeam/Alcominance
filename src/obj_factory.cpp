
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
static CVar v_recept("factory_recept", "S10+A10+H3+W20=1", 0); // recept pro jednu davku
static CVar v_coalmax("coal_max", 200, TVAR_SAVE); // maximalni kapacita pro uhli


#ifndef BECHER_EDITOR
FactoryStatic Factory::m_userhud;

FactoryStatic::FactoryStatic()
{
	m_act = NULL;
}

void FactoryStatic::SetAct(Factory * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetText(m_trtinainfo);
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetText(m_sugarinfo);

}

void FactoryStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		//sprintf(m_sugarinfo,"%d cukru.", m_act->m_sugar.GetNum());
		//sprintf(m_trtinainfo,"%d trtina.", m_act->m_cane.GetNum());

		ObjectHud::Draw(h2d);
	}
}
#endif // BECHER_EDITOR

////////////////////////////////////////////////////////
Factory::Factory(IHoeScene * scn) : FactoryBuilding(scn), m_becher(EBS_Becher)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_FACTORY));
	m_infoselect.s_x = 6.7f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 6.7f;
	m_sugar.SetOwner(this);
	m_alco.SetOwner(this);
	m_herbe.SetOwner(this);
	m_water.SetOwner(this);
	m_w.SetOwner(this);
	m_becher.SetOwner(this); CRR::Get()->Register(&m_becher);
	m_w.SetRecept(&v_recept);
	m_progress = 0.f;

    m_wrk_alco = 0;
    m_wrk_sugar = 0;
    m_wrk_herbe = 0;
    m_wrk_water = 0;
    m_wrk_coal = 0;
}

Factory::~Factory()
{
}

bool Factory::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteReservedWords(30);
	return true;
}

bool Factory::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(30);
	OnUpdateSur();
	return true;
}

#ifndef BECHER_EDITOR

ResourceBase * Factory::GetResource(ESurType type)
{
	switch (type)
	{
	case EBS_Coal:
		return &m_w;
	case EBS_Sugar:
		return &m_sugar;
	case EBS_Alco:
		return &m_alco;
	case EBS_Water:
		return &m_water;
	case EBS_Herbe:
		return &m_herbe;
	case EBS_Becher:
		return &m_becher;
	default:
		return NULL;
	};
}

const char * Factory::BuildPlace(float x, float y)
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

bool Factory::InsertSur(ESurType type, uint *s)
{
	switch (type)
	{
	case EBS_Sugar:
		return m_sugar.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
	case EBS_Water:
		return m_water.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
	case EBS_Alco:
		return m_alco.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
	case EBS_Herbe:
		return m_herbe.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
	case EBS_Coal:
		return m_w.Add(s,*s);
	default:
		assert(!"insert bad type");
		return false;
	};
	return false;
}

bool Factory::SetToWork(Troll * t)
{
    switch (t->GetJob().type){
    case TJob::jtWork:
	    if (m_worked.Count() >= (uint)v_numworks.GetInt()) return false;
	    m_worked.Add(t);
        break;
    case TJob::jtGotoRes:
        switch (t->GetJob().surtype){
        case EBS_Alco:
            m_wrk_alco++;
            break;
        case EBS_Sugar:
            m_wrk_sugar++;
            break;
        case EBS_Herbe:
            m_wrk_herbe++;
            break;
        case EBS_Water:
            m_wrk_water++;
            break;
        case EBS_Coal:
            m_wrk_coal++;
            break;
        }
        break;

    }
	return true;
}

void Factory::UnsetFromWork(Troll * t)
{
    switch (t->GetJob().type){
    case TJob::jtWork:
	    m_worked.Remove(t);
        break;
    case TJob::jtGotoRes:
        switch (t->GetJob().surtype){
        case EBS_Alco:
            m_wrk_alco--;
            break;
        case EBS_Sugar:
            m_wrk_sugar--;
            break;
        case EBS_Herbe:
            m_wrk_herbe--;
            break;
        case EBS_Water:
            m_wrk_water--;
            break;
        case EBS_Coal:
            m_wrk_coal--;
            break;
        }
        break;
    }
}

void Factory::Update(const float t)
{
	// update
	float prog = m_w.InProcess() ? m_worked.Count()*v_numzpr.GetFloat():0.f;

	if (m_worked.Count() > 0)
	{
		m_w.Update(t*prog);

		if (m_w.CanOut() && ((int)m_w.Out(false)<=(v_sklad.GetInt() - GetMiniStoreCount())))
		{
			uint p = m_w.Out(true);
			m_becher.Add(&p, p);
		}

		// naplneni
		if (m_w.CanIn() && m_w.In(&m_sugar, 'S', false)
			 && m_w.In(&m_alco, 'A', false)
			  && m_w.In(&m_herbe, 'H', false)
			   && m_w.In(&m_water, 'W', false))
		{
			m_w.In(&m_sugar, 'S', true);
			m_w.In(&m_alco, 'A', true);
			m_w.In(&m_herbe, 'H', true);
			m_w.In(&m_water, 'W', true);
			m_w.ToProcess();
		}
	}

	if (m_progress != prog)
	{
		// update 
		m_progress = prog;
		// pokud neni progress a nemuze se delat
		/*if (m_progress > 0.f)
			m_part.emitor->Start();
		else
			m_part.emitor->Stop();*/
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


bool Factory::Select()
{
	FactoryBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	GetLua()->func("s_tovarna");
	return true;
}

bool Factory::Idiot(TJob * j)
{
	// zjistit pripadny zdroj pro suroviny
	// 
	// navalit informace do tabulky, bud z crr nebo primo vybrane uloziste
	ResourceExp * ris = CRR::Get()->Find(EBS_Sugar, this);
	ResourceExp * ria = CRR::Get()->Find(EBS_Alco, this);
	ResourceExp * rih = CRR::Get()->Find(EBS_Herbe, this);
	ResourceExp * riw = CRR::Get()->Find(EBS_Water, this);
    ResourceExp * rc = CRR::Get()->Find(EBS_Coal, this);
	
	HoeGame::LuaFunc f(GetLua(), "i_factory");
	f.PushTable();
	
	f.SetTableInteger("max_store", v_sklad.GetInt());

    // vstupni suroviny
	f.SetTableInteger("sugar_avail", ris ? ris->GetNum():0);
    f.SetTableInteger("sugar_wrkcount", m_wrk_sugar);
	f.SetTableInteger("sugar", m_sugar.GetNum());
	
    f.SetTableInteger("alco_avail", ria ? ria->GetNum():0);
    f.SetTableInteger("alco_wrkcount", m_wrk_alco);
	f.SetTableInteger("alco", m_alco.GetNum());

	f.SetTableInteger("herbe_avail", rih ? rih->GetNum():0);
    f.SetTableInteger("herbe_wrkcount", m_wrk_herbe);
	f.SetTableInteger("herbe", m_herbe.GetNum());

	f.SetTableInteger("water_avail", riw ? riw->GetNum():0);
    f.SetTableInteger("water_wrkcount", m_wrk_water);
	f.SetTableInteger("water", m_water.GetNum());

    f.SetTableInteger("coal_avail", rc ? rc->GetAvail():0);
    f.SetTableInteger("coal_wrkcount", m_wrk_coal);
	f.SetTableInteger("coal", m_coal.GetNum());
    f.SetTableInteger("coal_max", v_coalmax.GetInt());

    // vystupni suroviny
	f.SetTableInteger("becher", m_becher.GetNum());

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
		switch (j->surtype)
		{
		case EBS_Alco:
			j->from = ria; break;
		case EBS_Sugar:
			j->from = ris; break;
		case EBS_Herbe:
			j->from = rih; break;
		case EBS_Water:
			j->from = riw; break;
		};
		if (j->num > j->from->GetNum())
			j->num = j->from->GetNum();
		break;
	case 1:
		j->type = TJob::jtGotoWork;
		break;
	};
		
	f.Pop(1);
	
	return true;

}

#else

bool Factory::Select()
{
	FactoryBuilding::Select();
	/*GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Store"));
	GetProp()->AppendLong(6, _("Limit"), v_sklad.GetInt());
	GetProp()->End();*/	
	return true;
}

void Factory::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	/*switch (id)
	{
	case 6:
		v_sklad.Set((int)pi.GetLong());
		break;
	};*/
}

#endif

