
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_farm.h"

static CVar v_rust("farm_rust", 0.02f, 0);

////////////////////////////////////////////////////////////
Farm::Farm(IHoeScene * scn) : SourceBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_FARM));
	m_infoselect.s_x = 4.5f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 3.5f;
	worked = NULL;
	rust = 0.f;
	skliz = 0;
}

#ifndef BECHER_EDITOR

void Farm::Update(const float dtime)
{
	/*static bool hl = false;
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
	}*/

}
bool Farm::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool Farm::SetToWork(Troll * t)
{
	return false;
}

void Farm::UnsetFromWork(Troll * t)
{
}
bool Farm::Select()
{
	BecherBuilding::Select();
	GetLua()->func("s_farma");
	return true;
}

bool Farm::Idiot(Job *t)
{
	/*Store * s = FindStore();

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


#else // BECHER_OBJECT

bool Farm::Select()
{
	SourceBuilding::Select();
	return true;
}

void Farm::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



