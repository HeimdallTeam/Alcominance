
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
	
}

Troll::~Troll()
{
}

#ifndef BECHER_EDITOR

void Troll::Update(const double t)
{
	switch (job.phase)
	{
	case JobEx::GoTo:
	case JobEx::GoFrom:
		if (!job.Step(this, (float)t*v_speed.GetFloat()))
			return;
	}
	switch (job.type)
	{
	case Job::jtPrines:
		if (job.phase == JobEx::GoTo)
		{
			job.sur = job.store->GetSur(job.surtype, job.num, false);
			job.path.SetPosTo(job.owner);
			job.phase = JobEx::GoFrom;
		}
		else if (job.phase == JobEx::GoFrom)
		{
			job.owner->AddSur(job.surtype, job.sur);
			NewJob();
		}
		break;
	case Job::jtOdnes:
		if (job.phase == JobEx::GoTo)
		{
			job.sur = job.owner->GetSur(job.surtype, job.num, false);
			job.path.SetPosTo(job.store);
			job.phase = JobEx::GoFrom;
		}
		else if (job.phase == JobEx::GoFrom)
		{
			job.store->AddSur(job.surtype, job.sur);
			NewJob();
		}
		break;
	case Job::jtWork:
		if (job.phase == JobEx::GoTo)
		{
			job.owner->AddToWork(this);
			job.phase = JobEx::GoToNew;
		}
		break;
	case Job::jtNone:
		NewJob();
		break;
	}
}

bool Troll::Select()
{
	GetLua()->func("s_tupoun");
	return true;
}

void Troll::SetJob(const Job & j)
{
	job = j;
}

void Troll::StopWork()
{
	NewJob();
}

void Troll::NewJob()
{
	if (job.type == Job::jtNone || !job.owner->Idiot(this))
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
	}
}

////////////////////////////////////////////////
JobEx::JobEx()
{
	SetNone();
	phase = JobEx::GoToNew;
}

const Job & JobEx::operator = (const Job & j)
{
	this->type = j.type;
	this->owner = j.owner;
	this->store = j.store;
	this->num = j.num;
	this->surtype = j.surtype;
	this->phase = JobEx::GoTo;
	switch (j.type)
	{
	case Job::jtPrines:
		path.SetPosTo(j.store);
		break;
	case Job::jtWork:
	case Job::jtOdnes:
		path.SetPosTo(j.owner);
		break;
	};
	return *this;
}

void JobEx::SetNone()
{
	type = jtNone;
}

bool JobEx::Step(Troll * t, const float time)
{
	bool finish;
	float posX = t->GetPosX();
	float posY = t->GetPosY();
	float puvX = posX;
	float puvY = posY;
	finish = path.GetNextPos(time, posX,posY);
	t->SetAngle(-atan2f(posX-puvX,posY-puvY));
	t->SetPosition( posX, posY, 0);
	return finish;
}

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


#endif // BECHER_EDITOR

