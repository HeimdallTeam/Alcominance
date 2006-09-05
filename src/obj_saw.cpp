
#include "StdAfx.h"
#include "becher.h"
#include "obj_saw.h"

bool Saw::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool Saw::SetToWork(Troll * t)
{
	return false;
}

void Saw::UnsetFromWork(Troll * t)
{
}

////////////////////////////////////////////////////////////
Saw::Saw(IHoeScene * scn) : BecherBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_SAW));
}

#ifndef BECHER_EDITOR

void Saw::Update(const double dtime)
{
}

bool Saw::Select()
{
	GetLua()->func("s_saw");
	return true;
}

bool Saw::Idiot(Job *t)
{
	return false;
}


#else // BECHER_OBJECT

bool Saw::Select()
{
	return true;
}

#endif // BECHER_OBJECT



