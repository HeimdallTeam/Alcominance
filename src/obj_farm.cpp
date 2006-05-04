
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_farm.h"
#include "obj_store.h"

static CVar v_rust("farm_rust", 0.02f, 0);

////////////////////////////////////////////////////////////
Farm::Farm(IHoeScene * scn) : BecherBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_FARM));
	worked = NULL;
	rust = 0.f;
	skliz = 0;
}

#ifndef BECHER_EDITOR

void Farm::Update(const double dtime)
{
	static bool hl = false;
	if (!hl)
	{
		GetBecher()->GetInfoPanel()->Add("Farma potrebuje delnika!");
		hl = true;
	}
	if (worked && worked->GetActJob() == Job::jtWork)
	{
		rust += (float)dtime * v_rust.GetFloat();
		if (rust > 1.f)
		{
			skliz = 50;
			rust = 0;
			Idiot(worked);
		}
	}

}

bool Farm::Select()
{
	GetLua()->func("s_farma");
	return true;
}

int Farm::AddSur(ESurType type, int s)
{

	return 0;
}

int Farm::GetSur(ESurType type, int req, bool upln)
{
	if (type != EBS_Cane)
		return 0;
	if (req <= this->skliz)
	{
		skliz -= req;
		return req;
	}
	else if (!upln)
	{
		req = skliz;
		skliz = 0;
		return req;
	}
	else
	{
		return 0;
	}
	return 0;
}

void Farm::AddToWork(Troll *t)
{
	worked = t;
}

bool Farm::Idiot(Troll *t)
{
	if (worked && t != worked)
		return false;
	worked = t;
	Job job;
	job.owner = this;
	if (skliz)
	{
		job.type = Job::jtOdnes;
		job.num = 10;
		job.surtype = EBS_Cane;
		//job.store;
		for (int i=0;i < GetBecher()->GetLevel()->GetNumObj();i++)
		{
			BecherObject * bo = GetBecher()->GetLevel()->GetObj(i);
			if (bo->GetType() != EBO_Store)
				continue;
			job.store = dynamic_cast<Store*>(bo);
			// pokud je lepsi nez ten druhy
		}
		if (job.store)
		{
			t->SetJob(job);
			return true;
		}
	}
	else
	{
		job.type = Job::jtWork;
		t->SetJob(job);
		return true;
	}
	return false;
}

int Farm::GetNumInfos()
{
	return 1;
}

int Farm::GetInfo(int id, char *buff, size_t size)
{
	sprintf(buff, "Rust: %f", rust);
	return 0;
}

#else // BECHER_OBJECT

bool Farm::Select()
{
	return true;
}

#endif // BECHER_OBJECT



