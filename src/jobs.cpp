
#include "StdAfx.h"
#include "id.h"
#include "jobs.h"
#include "becher.h"
//#include "game.h"

bool TJob::Save(BecherGameSave &w)
{
#ifndef BECHER_EDITOR
	//
	w.WriteValue<dword>(this->type);
	w.WriteValue<dword>(owner ? this->owner->GetID():0);
	w.WriteValue<int>(this->num);
	w.WriteValue<dword>(this->surtype);
	w.WriteValue<float>(this->percent);
	w.WriteValue<dword>(from ? this->from->GetOwner()->GetID():0);
	w.WriteValue<dword>(to ? this->to->GetID():0);
#endif
	return true;
}

bool TJob::Load(BecherGameLoad &r)
{
#ifndef BECHER_EDITOR
	type = (Type)r.Read<dword>();
	dword id = r.Read<dword>();
	if (id)
		owner = dynamic_cast<BecherBuilding*>(GetLevel()->GetObjFromID(id));
	else
		owner = NULL;
	num = r.Read<int>();
	surtype = (ESurType)r.Read<dword>();
	percent = r.Read<float>();
	id = r.Read<dword>();
	if (id);
		//from = (ResourceExp*)
		//dynamic_cast<BecherBuilding*>(GetLevel()->GetObjFromID(id))->GetResource(surtype);
	else
		from = NULL;
	id = r.Read<dword>();
	if (id)
		to = dynamic_cast<BecherBuilding*>(GetLevel()->GetObjFromID(id));
	else
		to = NULL;
#endif //BECHER_EDITOR
	return true;
}




