
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_sugar.h"
#include "obj_store.h"
#include "crr.h"


static CVar v_numzpr("sugar_work", 1.f, 0);
static HoeGame::CTimer t_numzpr(v_numzpr);
static CVar v_sklad("sugar_max", 50, 0);

////////////////////////////////////////////////////////
Sugar::Sugar(IHoeScene * scn) : FactoryBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_SUGAR));
	GetCtrl()->SetFlags(HOF_SHOW);
	m_cane = 0;
	m_sugar = 0;
	//m_mode = wmIn;
	m_worked.num = 0;
}

Sugar::~Sugar()
{
}

#ifndef BECHER_EDITOR

int Sugar::AddSur(ESurType type, int s)
{
	if (type==EBS_Cane)
	{
		// kolik se trtiny vejde
		int max = v_sklad.GetInt() - m_sugar - m_cane;
		if (s>max)
		{
			m_cane += max; return s - max;
		}
		else
		{
			m_cane += s; return 0;
		}
	}
	else
	{
		return s;
	}
}

int Sugar::GetSur(ESurType type, int req, bool upln)
{
	if (type!=EBS_Sugar)
		return 0;
	if (req <= this->m_sugar)
	{
		m_sugar -= req;
		return req;
	}
	else if (!upln)
	{
		req = m_sugar;
		m_sugar = 0;
		return req;
	}
	else
	{
		return 0;
	}
}

void Sugar::AddToWork(Troll *t)
{
	if (!m_worked.Add(t))
		t->StopWork();
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
		sprintf(buff, "cukr: %d", m_sugar);break;
	case 1:
		sprintf(buff, "trtina: %d", m_cane);break;
	};
	return 1;
}

void Sugar::Update(const double t)
{
	if (m_worked.num > 0)
	{
		if (m_cane > 0)
		{
			int p = t_numzpr.Compute(t * m_worked.num);
			if (p > m_cane)
				p = m_cane;
			m_cane-=p;
			m_sugar+=p;
		}
		else
		{
			for (size_t i=0;i < m_worked.num;i++)
				m_worked.works[i]->StopWork();
			m_worked.num = 0;
		}
	}
}


bool Sugar::Select()
{
	if (!IsBuildMode())
        GetLua()->func("s_cukr");
	return true;
}

void Sugar::SetWork(Troll *t)
{
	Job job;
	job.owner = this;
	job.type = Job::jtWork;
	//t->SetJob(job);
}

void Sugar::SetIn(Troll *t, Store * s)
{
	Job job;
	job.owner = this;
	job.type = Job::jtPrines;
	//job.store = s;
	job.num = 10;
	job.surtype = EBS_Cane;
	//t->SetJob(job);
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
	ResourceItem * ri = CRR::Get()->Find(EBS_Cane); // urceni priorit
	
	HoeGame::LuaFunc f(GetLua(), "i_sugar");
	f.PushTable();
	// suroviny
	// informace o surovinach
	f.SetTableInteger("max_store", v_sklad.GetInt());
	f.SetTableInteger("cane_avail", ri ? ri->GetNum():0);
	f.SetTableInteger("cane", m_cane);
	f.SetTableInteger("sugar", m_sugar);
	// works
	f.SetTableInteger("works", this->m_worked.num);
	f.SetTableInteger("works_free", this->m_worked.NumFree());
	f.Run(1);
	if (f.IsNil(-1))
	{
		f.Pop(1);
		return false;
	}

	// prevest zpatky na job
	int r = f.GetTableInteger("type", -1); // typ prace
	j->percent = f.GetTableInteger("percent", -1); // na kolik procent je vyzadovano
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

