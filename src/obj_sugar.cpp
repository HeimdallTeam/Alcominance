
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_sugar.h"

static CVar v_numzpr("sugar_speed", 1.f, 0); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_sklad("sugar_max", 50, 0); // maximalni velikost miniskladu
static CVar v_numworks("sugar_maxwork", 4, 0); // maximalni pocet pracujicich
static CVar v_recept("sugar_recept", "C1=1", 0); // recept pro jednu davku

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

/*bool Sugar::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(10);
	OnUpdateSur();
	return true;
}*/

void Sugar::SetMode(EBuildingMode mode)
{
	// odmazat
	if (mode == m_mode)
		return;
	// pri buildingu nastavit kolize
	switch (m_mode)
	{
	case EBM_Normal:
		CRR::Get()->Unregister(&m_sugar);
		break;
	};
	switch (mode)
	{
	case EBM_Normal:
		CRR::Get()->Register(&m_sugar);
		break;
	};
	m_mode = mode;
}

#ifndef BECHER_EDITOR

bool Sugar::InsertSur(ESurType type, uint *s)
{
	switch (type)
	{
	case EBS_Cane:
		return m_cane.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
	case EBS_Coal:
		return m_w.Add(s,*s);
	default:
		assert(!"insert bad type");
		return false;
	};
}

bool Sugar::SetToWork(Troll * t)
{
	if (m_worked.Count() >= (uint)v_numworks.GetInt())
		return false;
	m_worked.Add(t);
	return true;
}

void Sugar::UnsetFromWork(Troll * t)
{
	m_worked.Remove(t);
}

void Sugar::Update(const float t)
{
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
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_cukr");
	return true;
}

bool Sugar::Idiot(TJob * j)
{
	// zjistit pripadny zdroj pro suroviny
	// 
	// navalit informace do tabulky, bud z crr nebo primo vybrane uloziste
	ResourceExp * ri = CRR::Get()->Find(EBS_Cane, this); // urceni priorit
	ResourceExp * rc = CRR::Get()->Find(EBS_Coal, this); // urceni priorit
	BecherBuilding * bout = CRR::Get()->FindAccept(EBS_Sugar, this);

	// najit sklad pro 
	HoeGame::LuaFunc f(GetLua(), "i_sugar");
	f.PushTable();
	// suroviny
	// informace o surovinach
	f.SetTableInteger("max_store", v_sklad.GetInt());
	f.SetTableInteger("cane_avail", ri ? ri->GetAvail():0);
	f.SetTableInteger("cane", m_cane.GetNum());
	f.SetTableInteger("sugar", m_sugar.GetNum());
	f.SetTableInteger("sugar_out", bout ? bout->AcceptSur(EBS_Sugar):0);
	f.SetTableInteger("coal", m_w.GetNum());
	f.SetTableInteger("coal_avail", rc ? rc->GetAvail():0);
	f.SetTableInteger("coal_max", 100);
	// works
	f.SetTableInteger("works", this->m_worked.Count());
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

