#include "StdAfx.h"
#include "utils.h"
#include "becher.h"
#include "crr.h"

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
	HoeGame::PtrSet<ResourceExp*> & item = m_items[type];
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
		if (item.Get(i)->GetOwner() != owner)
		{
			if (!best || cmp(best, item.Get(i)))
				best = item.Get(i);
		}
	}
	return best;
}

void CRR::Register(ResourceExp* item)
{
#ifndef BECHER_EDITOR
	// zaregistrovani itemu
	assert(item);
	this->m_items[item->GetType()].Add(item);
#endif // BECHER_EDITOR
}

