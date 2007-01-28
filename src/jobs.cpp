
#include "StdAfx.h"
#include "id.h"
#include "jobs.h"
#include "becher.h"
//#include "game.h"

bool TJob::Save(BecherGameSave &w)
{
#ifndef BECHER_EDITOR
	//
/*w.WriteValue<dword>(this->type);
	w.WriteValue<dword>(owner ? this->owner->GetID():0);
	w.WriteValue<int>(this->num);
	w.WriteValue<dword>(this->surtype);
	w.WriteValue<float>(this->percent);
	w.WriteValue<dword>(from ? this->from->GetOwner()->GetID():0);
	w.WriteValue<dword>(to ? this->to->GetID():0);*/
#endif
	return true;
}

bool TJob::Load(BecherGameLoad &r)
{
#ifndef BECHER_EDITOR
	/*type = (Type)r.Read<dword>();
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
		to = NULL;*/
#endif //BECHER_EDITOR
	return true;
}

// jobs
static TJob::EType j_go[] = { TJob::EJT_Go, TJob::EJP_Point, TJob::EJT_None };
static TJob::EType j_import[] = { 
	TJob::EJT_Go, TJob::EJP_Source, 
	TJob::EJT_GetSur,
	TJob::EJT_Go, TJob::EJP_Owner, 
	TJob::EJT_InsertSur,
	TJob::EJT_Incoming, TJob::EJP_Owner,
	TJob::EJT_None };
static TJob::EType * jobs[] =
{
	j_go,
	j_import,
};

// jobs
void TJob::Go(float x, float y)
{
	njob = 0; npart = 0;
	point = HoeMath::Vector2(x, y);
}

void TJob::Import(PAR_Favour * fav)
{
	njob = 1; npart = 0;
	favour = *fav;
}

TJob::EType * TJob::Get()
{
	return jobs[njob] + npart;
}



