
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_factory.h"
#include "obj_store.h"

static CVar v_numzpr("factory_speed", 1.f, 0);
static HoeGame::CTimer t_numzpr(v_numzpr);
static CVar v_sklad("factory_max", 50, 0);
static CVar v_numworks("factory_maxwork", 4, 0);

FactoryStatic Factory::m_userhud;

FactoryStatic::FactoryStatic()
{
	m_act = NULL;
}

void FactoryStatic::SetAct(Factory * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Font*>(ReqItem("trtina"))->SetPtr(m_trtinainfo);
	//dynamic_cast<HoeGame::Font*>(ReqItem("cukr"))->SetPtr(m_sugarinfo);

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

////////////////////////////////////////////////////////
Factory::Factory(IHoeScene * scn) : FactoryBuilding(scn), m_becher(EBS_Becher)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_FACTORY));
	m_infoselect.s_x = 6.7f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 6.7f;
	m_becher.SetOwner(this); CRR::Get()->Register(&m_becher);
}

Factory::~Factory()
{
}

#ifndef BECHER_EDITOR

bool Factory::InsertSur(ESurType type, uint *s)
{
	//assert(type==EBS_Cane);
	// max
	//return m_cane.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
	return false;
}

bool Factory::SetToWork(Troll * t)
{
	if (m_worked.Count() >= (uint)v_numworks.GetInt())
		return false;
	m_worked.Add(t);
	return true;
}

void Factory::UnsetFromWork(Troll * t)
{
	m_worked.Remove(t);
}

void Factory::Update(const double t)
{
	/*if (m_worked.Count() > 0)
	{
		if (m_cane.GetNum() > 0)
		{
			uint p = t_numzpr.Compute(t * m_worked.Count());
			p=m_cane.Get(p,true);
			m_sugar.Add(&p, v_sklad.GetInt() - GetMiniStoreCount());
			m_exitdelay.Reset();
		}
		else
		{
			// postupne propoustet
			if (m_exitdelay.AddTime(t, 3.f))
			{
				m_exitdelay.Reset();
				// propustit jednoho workera
				m_worked.OneStopWork();
			}
		}
	}*/
}


bool Factory::Select()
{
	FactoryBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_tovarna");
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

bool Factory::Idiot(Job * j)
{
	// zjistit pripadny zdroj pro suroviny
	// 
	// navalit informace do tabulky, bud z crr nebo primo vybrane uloziste
	/*ResourceExp * ri = CRR::Get()->Find(EBS_Cane); // urceni priorit
	
	HoeGame::LuaFunc f(GetLua(), "i_Factory");
	f.PushTable();
	// suroviny
	// informace o surovinach
	f.SetTableInteger("max_store", v_sklad.GetInt());
	f.SetTableInteger("cane_avail", ri ? ri->GetNum():0);
	f.SetTableInteger("cane", m_cane.GetNum());
	f.SetTableInteger("Factory", m_sugar.GetNum());
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
		j->type = Job::jtPrines;
		j->num = f.GetTableInteger("num", -1); // pocet k prineseni
		j->ritem = ri;
		break;
	case 1:
		j->type = Job::jtWork;
		break;
	};
		
	f.Pop(1);
	
	return true;*/
	return false;
}