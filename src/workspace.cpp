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
	if (*s<=(uint)max)
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
	m_priority = EBSP_Normal;
	m_max = 0;
	m_locked = 0;
}

uint ResourceExp::Lock(uint num)
{
	// zalockovat surovinu
	m_locked += num;
	// kontrola
	if (m_locked < 0)
	{
		assert(!"nepravdepodobna situace");
		m_locked = 0;
	}
	if (m_locked > m_actual)
	{
		assert(!"nepravdepodobna situace");
		m_locked = m_actual;
	}
	return num;
}

void ResourceExp::Unlock(uint num)
{
	if (m_locked > m_actual)
	{
		assert(!"nepravdepodobna situace");
		m_locked = m_actual;
	}
	if (m_locked < num)
	{
		assert(!"nepravdepodobna situace");
		m_locked = 0;
	}
	else
		m_locked -= num;
	// kontrola

}

//////////////////////////////////////////
Workspace::Workspace()
{
	m_recept = NULL;
	m_progress = 0.f;
	m_out = 0;
	m_mout = 0;
}

int Workspace::GetNumber(char type)
{
	assert(m_recept != NULL);
	const char * p = m_recept->GetString();
	int i;
	while (*p && *p!=type) p++;
	if (!*p) return 0; // error
	if (sscanf(p+1, "%d", &i)==1)
	{
		return i;
	}
	return 0;
}

void Workspace::SetRecept(CVar * recept)
{
	m_recept = recept;
}

void Workspace::ToProcess()
{
	m_mout = GetNumber('=');
	m_progress = 1.f;
}

void Workspace::Update(const float t)
{
	// update progress
	if (m_progress > 0.f)
	{
		m_progress -= t;
		if (m_progress <= 0.f)
		{
			m_out = m_mout;
			m_mout = 0;
		}
	}
}

bool Workspace::In(ResourceBase * sur,char type, bool remove)
{
	uint n = GetNumber(type);
	if (n > sur->GetNum())
		return false;
	if (remove)
		sur->Get(n, true);
	return true;
}

uint Workspace::Out(bool remove)
{
	int ret = m_out;
	if (remove)
		m_out = 0;
	return ret;
}












