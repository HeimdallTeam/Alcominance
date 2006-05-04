
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_sugar.h"
#include "obj_store.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

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
	m_mode = wmIn;
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



bool Sugar::Idiot(Troll * t)
{
	// pokud neni trtina prinest trtinu
	// jestli neni pracujici a je vice nez 50% trtiny tak pracovat
	// pokud je vice nez 90% trtiny a je volne misto tak pracuj
	// pokud je cukru vice nez volneho mista pak odnest
	// odmitnout
	/*lua_getglobal(GetLua()->GetLua(), "i_farm");
	lua_newtable(GetLua()->GetLua());
	lua_pushinteger(GetLua()->GetLua(), 2);
	lua_setfield(GetLua()->GetLua(),-2,"bb");
	lua_pushinteger(GetLua()->GetLua(), 3);
	lua_setfield(GetLua()->GetLua(),-2,"cc");
	lua_resume(GetLua()->GetLua(),1);*/


	Store * s = FindStore();
	if (!s)
	{
		if (m_cane > 1 && m_worked.NumFree() > 1)
		{
			SetWork(t);
			return true;
		}
		else return false;
	}

	if (m_sugar >= (v_sklad.GetInt() - m_cane - m_sugar))
	{
		SetOut(t,s);
		return true;
	}

	if (m_cane == 0 && s->GetStatus(EBS_Cane) > 0)
	{
		SetIn(t,s);
		return true;			
	}

	if ((m_cane * 2 >= v_sklad.GetInt() && m_worked.num == 0) ||
		(m_cane >= v_sklad.GetInt() * 0.9 && m_worked.NumFree() > 1))
	{
		SetWork(t);
		return true;
	}

	if (m_sugar > 10)
	{
		SetOut(t,s);
		return true;
	}

	if ((v_sklad.GetInt()-m_cane-m_sugar)>0 && s->GetStatus(EBS_Cane) >0)
	{
		SetIn(t,s);
		return true;			
	}

	if (m_sugar > 0)
	{
		SetOut(t,s);
		return true;
	}

	return false;
}

void Sugar::SetWork(Troll *t)
{
	Job job;
	job.owner = this;
	job.type = Job::jtWork;
	t->SetJob(job);
}
void Sugar::SetOut(Troll *t, Store *s )
{
	Job job;
	job.owner = this;
	job.type = Job::jtOdnes;
	job.store = s;
	job.num = 10;
	job.surtype = EBS_Sugar;
	t->SetJob(job);
}
void Sugar::SetIn(Troll *t, Store * s)
{
	Job job;
	job.owner = this;
	job.type = Job::jtPrines;
	job.store = s;
	job.num = 10;
	job.surtype = EBS_Cane;
	t->SetJob(job);
}
#else

bool Sugar::Select()
{
	return true;
}

#endif
