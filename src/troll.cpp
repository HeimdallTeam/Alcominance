
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "crr.h"
#include "troll.h"

static CVar v_speed("troll_speed", 20.f, 0);

Troll::Troll(IHoeScene * scn) : BecherObject(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_TROLL));
	GetCtrl()->SetFlags(HOF_SHOW|HOF_UPDATE);
	m_infoselect.s_x = .8f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = .8f;
	memset(&m_job, 0, sizeof(m_job));
	m_job.type = TJob::jtNone;
	m_load.locked = false;
	m_load.numsur = 0;
	m_load.surtype = EBS_None;
}

Troll::~Troll()
{
}

void Troll::Update(const float t)
{
	// pokud chodi tak chodi
	// pokud ceka, tak nic
	switch (m_job.type)
	{
	case TJob::jtGotoRes:
	case TJob::jtGotoOwnerWithRes:
	case TJob::jtGotoWork:
		// update cesty, pokud cesta hotova tak finish
		if (m_path.Step(this, (float)t*v_speed.GetFloat()))
			Finish();
		break;
	case TJob::jtFindJob:
		FindJob(m_job.owner);
		break;
	};
	/*EPhaseResult res;
				{
					m_path.SetPosTo(m_job.owner);
					m_phase = GoToOwner;
				}
				else
				{
					FindJob(m_job.owner);
					// dodelat aby kdyz nebude prace tak nic
				}
			}
			else if (m_phase == GoToOwner)
			{
				
			}
			break;
		case Job::jtWork:
			if (m_phase == PhaseStart)
			{
				// nastavit novou cestu na misto urceni
				m_phase = GoToOwner;
				m_path.SetPosTo(m_job.owner);
			}
			else if (m_phase == GoToOwner)
			{
				m_job.owner->SetToWork(this);
				m_phase = Works;
			}
			else if (m_phase == GoToEnd)
			{
				m_job.owner->UnsetFromWork(this);
				FindJob(m_job.owner);
			}
			break;
		case Job::jtNone:
			FindJob(NULL);
			break;
		};
	}
	*/
}

void Troll::SetJob(const TJob & j)
{
	assert(j.owner != NULL);
	// opustit stary job
	switch (m_job.type)
	{
	case TJob::jtWork:
		assert(m_job.owner);
		m_job.owner->UnsetFromWork(this);
		break;
	};

	// nastavit novy
	if (m_load.locked)
	{
		m_job.ritem->Unlock(j.num);
		m_load.numlocked = 0;
		m_load.locked = false;
	}

	m_job = j;
	// nastavit parametry
	switch (j.type)
	{
	case TJob::jtGotoRes:
		if (m_job.ritem->GetPriority() != EBSP_TimeWork)
		{ 
			m_load.numlocked = m_job.ritem->Lock(j.num);
			m_load.locked = true;
		}
		m_path.SetPosTo(m_job.ritem->GetOwner());
		break;
	case TJob::jtGotoOwnerWithRes:
	case TJob::jtGotoWork:
		m_path.SetPosTo(m_job.owner);
		break;
	case TJob::jtWork:
		assert(m_job.owner);
		m_job.owner->SetToWork(this);
		break;
	};
}

void Troll::Finish()
{
	switch (m_job.type)
	{
	case TJob::jtGotoRes:
		if (m_job.ritem->GetPriority() == EBSP_TimeWork)
		{
			assert(m_load.locked == false);
			dynamic_cast<SourceBuilding*>(m_job.ritem->GetOwner())->SetToGet(this, m_job.num);
			// nastavit job na cekani
			TJob j = m_job;
			j.type = TJob::jtWaitToRes;
			SetJob(j);
		}
		else
		{
			// pokud jde pro surovinu, tak vyjmout a nastavit
			assert(m_load.locked == true);
			m_job.ritem->Unlock(m_load.numlocked);
			m_load.locked = false;
			m_load.surtype = m_job.surtype;
			m_load.numsur = m_job.ritem->Get(m_load.numlocked,true);
			// nastavit na chuzi k zpatky
			TJob j = m_job;
			j.type = TJob::jtGotoOwnerWithRes;
			SetJob(j);
		}
		break;
	case TJob::jtGotoOwnerWithRes:
		// vlozit do budovy a hledat novy job
// odevzdat surovinu, mozna by mohl cekat dokud nebude volno ve skladu
		assert(m_job.owner);
		m_job.owner->InsertSur(m_load.surtype, &m_load.numsur);
		m_load.numsur = 0;
		m_load.surtype = EBS_None;
		FindJob(m_job.owner);
		break;
	case TJob::jtGotoWork:
		{
			//this->ToBuilding();
			TJob j = m_job;
			j.type = TJob::jtWork;
			SetJob(j);
		}
		break;
	};
}

void Troll::SurIn(ESurType type, uint num)
{
	//assert(type == m_job.surtype);
	//assert(m_phase == WaitForSur);
	//m_numsur = num;
	//m_surtype = type;
	// prenastavit job
	m_load.surtype = type;
	m_load.numsur = num;
	TJob j = m_job;
	j.type = TJob::jtGotoOwnerWithRes;
	SetJob(j);
}

bool Troll::FindJob(BecherBuilding * pref)
{
	TJob job;
	assert(pref);
	if (pref->Idiot(&job))
	{
		SetJob(job);
		return true;
	}
	else
	{
		TJob j = m_job;
		j.type = TJob::jtFindJob;
		SetJob(j);
	}
	return false;
}

void Troll::StopWork()
{
	// nastavit job na none
	// hledat novy job
	// pokud nenalezen, zajit k budove
	TJob j = m_job;
	j.type = TJob::jtFindJob;
	SetJob(j);
}

#ifndef BECHER_EDITOR
bool Troll::Select()
{
	BecherObject::Select();
	GetLua()->func("s_tupoun");
	return true;
}
#else
bool Troll::Select()
{
	BecherObject::Select();
	return true;
}

#endif

////////////////////////////////////////////////
/*JobEx::JobEx()
{
	SetNone();
	phase = JobEx::GoToNew;
}

const Job & JobEx::operator = (const Job & j)
{
	this->type = j.type;
	this->owner = j.owner;
	this->num = j.num;
	this->surtype = j.surtype;
	this->phase = JobEx::GoTo;
	switch (j.type)
	{
	case Job::jtPrines:
		//path.SetPosTo(j.store);
		break;
	case Job::jtWork:
		path.SetPosTo(j.owner);
		break;
	};
	return *this;
}

void JobEx::SetNone()
{
	type = jtNone;
}*/

/////////////////////////////////////////
// Path
bool Path::GetNextPos(float l,float &px, float &py)
{
	while (l > 0)
	{
		float ux = x - px;
		float uy = y - py;
		float mag = sqrt(ux * ux + uy * uy);
		ux /= mag;
		uy /= mag;
		if (l < mag)
		{
			px += ux * l;
			py += uy * l;
			break;
		}
		px = x;
		py = y;
		return true;
	}
	return false;
}

void Path::SetPosTo(float X, float Y)
{
	x = X;y = Y;
}

void Path::SetPosTo(BecherObject * bo)
{
	SetPosTo( bo->GetPosX(), bo->GetPosY());
}

bool Path::Step(Troll * t, const float time)
{
	bool finish;
	float posX = t->GetPosX();
	float posY = t->GetPosY();
	float puvX = posX;
	float puvY = posY;
	finish = GetNextPos(time, posX,posY);
	float angle = -atan2f(posX*10.f-puvX*10.f,posY*10.f-puvY*10.f);
	t->SetAngle(angle);
	// nastavit pozici podle terenu
	t->SetPosition( posX, posY, GetLevel()->GetScene()->GetScenePhysics()->GetHeight(posX,posY));
	return finish;
}


