
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_sugar.h"
#include "obj_store.h"
#include "crr.h"


static CVar v_numzpr("sugar_speed", 1.f, 0);
static HoeGame::CTimer t_numzpr(v_numzpr);
static CVar v_sklad("sugar_max", 50, 0);
static CVar v_numworks("sugar_maxwork", 4, 0);

////////////////////////////////////////////////////////
Sugar::Sugar(IHoeScene * scn) : FactoryBuilding(scn), m_sugar(EBS_Sugar)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_SUGAR));
	GetCtrl()->SetFlags(HOF_SHOW);
	//m_mode = wmIn;
}

Sugar::~Sugar()
{
}

#ifndef BECHER_EDITOR

bool Sugar::InsertSur(ESurType type, uint *s)
{
	assert(type==EBS_Cane);
	// max
	return m_cane.Add(s, v_sklad.GetInt() - GetMiniStoreCount());
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

int Sugar::GetNumInfos()
{
	return 2;
}

int Sugar::GetInfo(int id, char * buff, size_t size)
{ 
	switch (id)
	{
	case 0:
		sprintf(buff, "cukr: %d", m_sugar.GetNum());break;
	case 1:
		sprintf(buff, "trtina: %d", m_cane.GetNum());break;
	};
	return 1;
}

void Sugar::Update(const double t)
{
	if (m_worked.Count() > 0)
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
	}
}


bool Sugar::Select()
{
	if (!IsBuildMode())
        GetLua()->func("s_cukr");
	return true;
}

#else

bool Sugar::Select()
{
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

bool Sugar::Idiot(Job * j)
{
	// zjistit pripadny zdroj pro suroviny
	// 
	// navalit informace do tabulky, bud z crr nebo primo vybrane uloziste
	ResourceExp * ri = CRR::Get()->Find(EBS_Cane); // urceni priorit
	
	HoeGame::LuaFunc f(GetLua(), "i_sugar");
	f.PushTable();
	// suroviny
	// informace o surovinach
	f.SetTableInteger("max_store", v_sklad.GetInt());
	f.SetTableInteger("cane_avail", ri ? ri->GetNum():0);
	f.SetTableInteger("cane", m_cane.GetNum());
	f.SetTableInteger("sugar", m_sugar.GetNum());
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
	
	return true;
}

