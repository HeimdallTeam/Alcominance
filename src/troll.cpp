
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
	m_infoselect.s_x = .8f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = .8f;
	memset(&m_job, 0, sizeof(m_job));
	m_numsur = 0;
	m_phase = PhaseStart;
}

Troll::~Troll()
{
}

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
				m_phase = GoToSource;
				m_path.SetPosTo(m_job.ritem->GetOwner());
			}
			else if (m_phase == GoToSource)
			{
				// vyjmout surovinu

				//job.sur = job.ritem->GetSur(job.surtype, job.num, false);
				//
				assert(m_job.ritem);
				m_surtype = m_job.surtype;
				assert(m_surtype == m_job.ritem->GetType());
				m_numsur = m_job.ritem->Get(10,true);
				// jestli uz nic neni hledat novou praci
				if (m_numsur > 0)
				{
					m_path.SetPosTo(m_job.owner);
					m_phase = GoToOwner;
				}
				else
				{
					FindJob(m_job.owner);
					/*!!!*/ // dodelat aby kdyz nebude prace tak nic
				}
			}
			else if (m_phase == GoToOwner)
			{
				// odevzdat surovinu, mozna by mohl cekat dokud nebude volno ve skladu
				/*!!!*/
				assert(m_job.owner);
				m_job.owner->InsertSur(m_surtype, &m_numsur);
				//job.owner->AddSur(job.surtype, job.sur);
				FindJob(m_job.owner);
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
}

EPhaseResult Troll::MakePhase(const double t)
{
	switch (m_phase)
	{
	case PhaseStart:
	case GoToEnd:
		return PhaseEnd;
	case GoToOwner:
	case GoToSource:
		if (m_path.Step(this, (float)t*v_speed.GetFloat()))
			return PhaseEnd;
		break;
	case Works:
		return PhaseContinue;
	default:
		assert(!"Unknown phase");
	}
	return PhaseContinue;
}

bool Troll::Select()
{
	BecherObject::Select();
	GetLua()->func("s_tupoun");
	return true;
}

void Troll::SetJob(const Job & j)
{
	//job = j; nastaveni zakladnich vlastnosti (treba cesty atd)
	m_job = j;
	if (m_job.owner) m_job.owner->GetPosX();
	// nastavit pocatecni hodnoty
	m_phase = PhaseStart;
}

void Troll::StopWork()
{
	// nastavit na hledani
	// pokud pracuje tak odhlasit z prace
	if (m_phase == Works)
		m_phase = GoToEnd;
}

bool Troll::FindJob(BecherBuilding * pref)
{
	Job job;
	if (pref && pref->Idiot(&job))
	{
		SetJob(job);
		return true;
	}
	if (m_job.owner && m_job.owner->Idiot(&job)) 
	{
		SetJob(job);
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
	float angle = -atan2f(posX*10.f-puvX*10.f,posY*10.f-puvY*10.f);
	t->SetAngle(angle);
	// nastavit pozici podle terenu
	t->SetPosition( posX, posY, GetBecher()->GetLevel()->GetScene()->GetScenePhysics()->GetHeight(posX,posY));
	return finish;
}



