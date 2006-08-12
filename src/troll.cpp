
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "infos.h"
#include "game.h"
#include "troll.h"
#include "obj_store.h"

static CVar v_speed("troll_speed", 20.f, 0);

Troll::Troll(IHoeScene * scn) : BecherObject(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_TROLL));
	GetCtrl()->SetFlags(HOF_SHOW|HOF_UPDATE);
	memset(&m_job, 0, sizeof(m_job));
}

Troll::~Troll()
{
}

#ifndef BECHER_EDITOR

void Troll::Update(const double t)
{
	EPhaseResult res;
	res = MakePhase(t);
	while (res==PhaseRepeat)
	{
		res = MakePhase(t);
	}
	if (res==PhaseEnd)
	{
		// podle jobu a faze urcit dalsi akci
		switch (m_job.type)
		{
		case Job::jtPrines:
			if (m_phase == PhaseStart)
			{
				// nastavit novou cestu na misto urceni
				m_phase = GoTo;
				m_path.SetPosTo(m_job.ritem->GetOwner());
			}
			else if (m_phase == GoTo)
			{
				//job.sur = job.ritem->GetSur(job.surtype, job.num, false);
				m_path.SetPosTo(m_job.owner);
				m_phase = GoFrom;
			}
			else if (m_phase == GoFrom)
			{
				//job.owner->AddSur(job.surtype, job.sur);
				FindJob(m_job.owner);
			}
			break;
		/*case Job::jtWork:
			if (job.phase == JobEx::GoTo)
			{
				job.owner->AddToWork(this);
				job.phase = JobEx::GoToNew;
			}
			break;
		case Job::jtNone:
			FindJob(NULL);
			break;*/
		};
	}
}

EPhaseResult Troll::MakePhase(const double t)
{
	switch (m_phase)
	{
	case PhaseStart:
		return PhaseEnd;
	case GoTo:
	case GoFrom:
		if (m_path.Step(this, (float)t*v_speed.GetFloat()))
			return PhaseEnd;
	}
	return PhaseContinue;
}

bool Troll::Select()
{
	GetLua()->func("s_tupoun");
	return true;
}

void Troll::MakeJob(const Job & j)
{
	//job = j; nastaveni zakladnich vlastnosti (treba cesty atd)
	m_job = j;
	// nastavit pocatecni hodnoty
	m_phase = PhaseStart;
}

void Troll::StopWork()
{
	//FindJob(NULL);
}

bool Troll::FindJob(BecherBuilding * pref)
{
	Job job;
	if (pref && pref->Idiot(&job))
	{
		MakeJob(job);
		return true;
	}
	if (m_job.owner && m_job.owner->Idiot(&job)) 
	{
		MakeJob(job);
		return true;
	}
	/*if (job.type == Job::jtNone || !job.owner->Idiot(this))
	{
		// hledani noveho idiota
		for (int i=0;i < GetBecher()->GetLevel()->GetNumObj();i++)
		{
			BecherObject * bo = GetBecher()->GetLevel()->GetObj(i);
			if (BecherMap::GetObjectClass(bo->GetType()) != EBC_Building)
				continue;
			if (dynamic_cast<BecherBuilding*>(bo)->Idiot(this))
				return;
		}
		job.SetNone();
	}*/
	return false;
}

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
	t->SetAngle(-atan2f(posX-puvX,posY-puvY));
	// nastavit pozici podle terenu
	t->SetPosition( posX, posY, GetBecher()->GetLevel()->GetScene()->GetScenePhysics()->GetHeight(posX,posY));
	return finish;
}

#endif // BECHER_EDITOR

