
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_destilate.h"
#include "obj_store.h"

static CVar v_numzpr("dest_work", 1.f, 0); // rychlost zpracovani
static HoeGame::CTimer t_numzpr(v_numzpr);
static CVar v_sklad("dest_max", 50, 0); // velikost miniskladu, trtina muze zabirat max 95%

////////////////////////////////////////////////////////////
Destilate::Destilate(IHoeScene * scn) : FactoryBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_DESTILATE));
	m_sugar = m_alco = 0;
	m_worked.num = 0;
}

#ifndef BECHER_EDITOR

int Destilate::AddSur(ESurType type, int s)
{
	if (type==EBS_Sugar)
	{
		// kolik se trtiny vejde
		int max = v_sklad.GetInt() - m_sugar - m_alco;
		if (s>max)
		{
			m_sugar += max; return s - max;
		}
		else
		{
			m_sugar += s; return 0;
		}
	}
	else
	{
		return s;
	}
}

int Destilate::GetSur(ESurType type, int req, bool upln)
{
	if (type != EBS_Alco)
		return 0;
	if (req <= m_alco)
	{
		m_alco -= req;
		return req;
	}
	else if (!upln)
	{
		req = m_alco;
		m_alco = 0;
		return req;
	}
	else
	{
		return 0;
	}
}

void Destilate::AddToWork(Troll *t)
{
	if (!m_worked.Add(t))
		t->StopWork();
}

bool Destilate::Select()
{
	GetLua()->func("s_lihovar");
	return true;
}


bool Destilate::Idiot(Job * t)
{
	return false;
}

void Destilate::Update(const double t)
{
	if (m_worked.num > 0)
	{
		if (m_sugar > 0)
		{
			int p = t_numzpr.Compute(t * m_worked.num);
			if (p > m_sugar)
				p = m_sugar;
			m_sugar-=p;
			m_alco+=p;
		}
		else
		{
			for (size_t i=0;i < m_worked.num;i++)
				m_worked.works[i]->StopWork();
			m_worked.num = 0;
		}
	}
	/*if (IsBuildMode())
	{
		m_buildproc += (float)(t * 20);
		if (m_buildproc >= 100.f)
		{
			m_buildproc = 100.f;
			m_buildmode = false;
			OnUpdateInfo();
			GetBecher()->GetInfoPanel()->Add("cukrovar postavenej");
		}
	}*/
	//tim += (float)t;
	//this->SetOrientation(0,1,0,tim);
}

void Destilate::SetWork(Troll *t)
{
	Job job;
	job.owner = this;
	job.type = Job::jtWork;
	//t->SetJob(job);
}

void Destilate::SetIn(Troll *t, Store * s)
{
	Job job;
	job.owner = this;
	job.type = Job::jtPrines;
	//job.store = s;
	job.num = 10;
	job.surtype = EBS_Sugar;
	//t->SetJob(job);
}

int Destilate::GetNumInfos()
{
	return 2;
}

int Destilate::GetInfo(int id, char *buff, size_t size)
{
	switch (id)
	{
	case 0:
		sprintf(buff, "cukr: %d", m_sugar);break;
	case 1:
		sprintf(buff, "alco: %d", m_alco);break;
	};
	return 1;
}

#else

bool Destilate::Select()
{
	return true;
}
#endif // BECHER_EDITOR

