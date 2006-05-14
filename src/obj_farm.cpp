
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
	Store * s = FindStore();

	HoeGame::LuaFunc f(GetLua(), "i_farm");
	f.PushTable();
	// suroviny
	f.SetTableInteger("works", this->worked ? 1:0);
	f.SetTableInteger("works_free", this->worked ? 0:1);
	f.Run(1);
	if (f.IsNil(-1))
	{
		f.Pop(1);
		return false;
	}

	int r = f.GetTableInteger("type", -1);
	f.Pop(1);
	
	/*switch (r)
	{
	case EBS_Cane:
		SetIn(t,s);
		return true;
	case 2:
		SetWork(t);
		return true;
	case 3:
		SetOut(t,s);
		return true;
	}*/

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



