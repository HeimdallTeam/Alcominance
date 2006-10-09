#include "StdAfx.h"
#include "utils.h"
#include "becher.h"
#include "crr.h"
#include "object.h"
#include "buildings.h"
#include "obj_store.h"

//////////////////////////////////////////

CRR* CRR::this_ = NULL;

CRR::CRR()
{
	assert(this_==NULL);
    this_=this;
}

CRR::~CRR()
{
	assert(this_==this);
	this_=NULL;
}

CRR * CRR::Get()
{
	return this_;
}

bool IsBetter(ResourceExp * p, ResourceExp * n)
{
	if (n->GetAvail() == 0)
		return false;
	if (p->GetAvail() == 0)
		return true;
	if (p->GetPriority() < n->GetPriority())
		return true;
	if (p->GetPriority() > n->GetPriority())
		return false;
	return p->GetAvail() < n->GetAvail();
}

bool IsBetterStore(ResourceExp * p, ResourceExp * n)
{
	return p->GetAvail() < n->GetAvail();
}

ResourceExp * CRR::Find(ESurType type, BecherObject * owner)
{
	// kdyztak assert
	HoeCore::Set<ResourceExp*> & item = m_items[type];
	bool (*cmp)(ResourceExp * p, ResourceExp * n);

	if (owner->GetType() == EBO_Store)
		cmp = IsBetterStore;
	else
		cmp = IsBetter;
	if (item.Count() == 0)
		return NULL;
	ResourceExp * best = NULL;
	for (uint i=0;i < item.Count();i++)
	{
		if (static_cast<BecherObject*>(item.Get(i)->GetOwner()) != owner)
		{
			if (!best || cmp(best, item.Get(i)))
				best = item.Get(i);
		}
	}
	return best;
}

BecherBuilding * CRR::FindAccept(ESurType type, BecherObject * owner)
{
	if (GetLevel() == NULL)
		return NULL;
	for (int i=0;i < GetLevel()->GetNumObj();i++)
	{
		register BecherObject * bo = GetLevel()->GetObj(i);
		if (bo->GetType() == EBO_Store && dynamic_cast<Store*>(bo)->AcceptSur(type) > 0)
			return dynamic_cast<BecherBuilding*>(bo);
	}
	return NULL;
}

void CRR::Register(ResourceExp* item)
{
#ifndef BECHER_EDITOR
	// zaregistrovani itemu
	assert(item);
	this->m_items[item->GetType()].Add(item);
#endif // BECHER_EDITOR
}

void CRR::Unregister(ResourceExp* item)
{
#ifndef BECHER_EDITOR
	// zaregistrovani itemu
	assert(item);
	this->m_items[item->GetType()].Remove(item);
#endif // BECHER_EDITOR
}
