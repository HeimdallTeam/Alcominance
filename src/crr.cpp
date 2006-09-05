#include "StdAfx.h"
#include "utils.h"
#include "becher.h"
#include "crr.h"

///////////////////////////////////////////
uint ResourceBase::Get(uint req, bool p)
{
	// callback na update
	if (m_actual > req)
	{
		m_actual -= req;
		return req;
	}
	if (!p) return 0;
	// odebrat cast
	req = m_actual;
	m_actual = 0;
	return req;
}

bool ResourceBase::Add(uint *s, int max)
{
	if (*s<(uint)max)
	{
		m_actual += *s; 
		return true;
	}
	else
	{
		*s = max - m_actual;
		m_actual += max;
		return false;
	}
}

////////////////////////////////////////////
ResourceExp::ResourceExp(ESurType type)
{
	m_type = type;
	m_actual = 0;
	m_owner = NULL;
	if (type == EBS_Cane)
		m_actual = 100;
}

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
	// zaregistrovani itemu
	assert(item);
	this->m_items[item->GetType()].Add(item);
}

