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

ResourceExp * CRR::Find(ESurType type)
{
	HoeGame::PtrSet<ResourceExp*> & item = m_items[type];
	if (item.Count() == 0)
		return NULL;
	ResourceExp * best = item.Get(0);
	for (uint i=1;i < item.Count();i++)
	{
		if (best->GetNum() < item.Get(i)->GetNum())
			best = item.Get(i);
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

