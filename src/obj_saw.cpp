
#include "StdAfx.h"
#include "becher.h"
#include "obj_saw.h"

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

int Saw::AddSur(ESurType type, int s)
{

	return 0;
}

int Saw::GetSur(ESurType type, int req, bool upln)
{
	return 0;
}

void Saw::AddToWork(Troll *t)
{
}

bool Saw::Idiot(Troll *t)
{
	return false;
}

int Saw::GetNumInfos()
{
	return 0;
}

int Saw::GetInfo(int id, char *buff, size_t size)
{
	
	return 0;
}

#else // BECHER_OBJECT

bool Saw::Select()
{
	return true;
}

#endif // BECHER_OBJECT



