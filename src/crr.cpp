#include "StdAfx.h"
#include "utils.h"
#include "becher.h"
#include "crr.h"

////////////////////////////////////////////
ResourceItem::ResourceItem(ESurType type)
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

ResourceItem * CRR::Find(ESurType type)
{
	return s;
}

void CRR::Register(ResourceItem* item)
{
	// zaregistrovani itemu
	s = item;
}

