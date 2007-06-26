
#include "StdAfx.h"
#include "becher.h"
#include "level.h"

static CVar v_speed("payday_time", 15.f, TVAR_SAVE);

//////////////////////////
MinistryOfJobs::MinistryOfJobs()
{
	//
	m_paytime = v_speed.GetFloat();
	m_sound = NULL;
    m_pay = 0.f;
}

bool MinistryOfJobs::Init()
{
	m_sound = (IHoeSound*)GetResMgr()->ReqResource(sound_PAYDAY);
	return true;
}

void MinistryOfJobs::Update(const float dtime)
{
	m_paytime -= dtime;
	if (m_paytime <= 0.f)
	{
		PayDay();
		m_paytime = v_speed.GetFloat();
	}
}

void MinistryOfJobs::PayDay()
{
	GetLevel()->GetPanel()->GetInfo()->Add(GetLang()->GetString(201));
	GetLevel()->GetCash()->Add((int)-m_pay);
	if (m_sound)
		m_sound->Play(false);
	m_pay = 0.f;
}

