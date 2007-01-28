#include "StdAfx.h"
#include "utils.h"
#include "becher.h"
#include "crr.h"
#include "buildings.h"
#include "troll.h"
#include "panels.h"

///////////////////////////////////////////
ResourceBase::ResourceBase(ESurType type)
{ 
	m_actual = 0; 
	m_owner = NULL;
	m_type = type;
}

void ResourceBase::SetNum(uint num)
{ 
	m_actual = num; 
	assert(m_owner != NULL);
	m_owner->OnUpdateSur(); 
}

uint ResourceBase::Get(uint req, bool p)
{
	assert(m_owner != NULL);
	// callback na update
	if (m_actual > req)
	{
		m_actual -= req;
		m_owner->OnUpdateSur();
		return req;
	}
	if (!p) return 0;
	// odebrat cast
	req = m_actual;
	m_actual = 0;
	m_owner->OnUpdateSur();
	return req;
}

bool ResourceBase::Add(uint *s, int max)
{
	assert(m_owner != NULL);
	if (*s<=(uint)max)
	{
		m_actual += *s; 
		m_owner->OnUpdateSur();
		return true;
	}
	else
	{
		*s = max - m_actual;
		m_actual += max;
		m_owner->OnUpdateSur();
		return false;
	}
}

bool ResourceBase::Save(BecherGameSave &w)
{
	return w.Write<uint>(m_actual);
}

bool ResourceBase::Load(BecherGameLoad &r)
{
	m_actual = r.Read<uint>();
	return true;
}

////////////////////////////////////////////
ResourceExp::ResourceExp(ESurType type) : ResourceBase(type)
{
	m_actual = 0;
	m_owner = NULL;
	m_priority = EBSP_Normal;
	m_max = 0;
	m_locked = 0;
}

uint ResourceExp::Lock(uint num)
{
	// zalockovat surovinu
	// muze zamknout malo
	if ((m_locked+num) > m_actual)
	{
		num = m_actual - m_locked;
		m_locked = m_actual;
		return num;
	}

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

bool ResourceExp::Save(BecherGameSave &w)
{
	if (!ResourceBase::Save(w))
		return false;
	w.WriteValue<uint>(m_priority);
	w.WriteValue<uint>(m_locked);
	w.WriteReservedWords(1);
	return true;
}

bool ResourceExp::Load(BecherGameLoad &r)
{
	if(!ResourceBase::Load(r))
		return false;
	m_priority = (ESurPriority)r.Read<uint>();
	m_locked = r.Read<uint>();
	r.ReadReservedWords(1);
	return true;
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
	while (*p && *p!=type) p++; // vyjde nastejno jako hledani v tabulce
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

/////////////////////////////////////////////////////////
// static
int Workspace2::s_passes = 0; // znasobeni
int Workspace2::s_num_in=0,Workspace2::s_num_out=0;

Workspace2::Request Workspace2::s_req_in[20];
Workspace2::RequestOut Workspace2::s_req_out[5];

Workspace2::Workspace2(CVar * recept)
{
	m_recept = recept;
	// zjistit z receptu cas a nastavit na nejakou prijemnou hodnotu (rand)
	sscanf(m_recept->GetString(), "%f", &m_desttime);
	m_desttime = m_desttime * HoeCore::RandFloat(0.6f,1.f);
}

int Workspace2::BeginPass(float workers, float time)
{
	assert(m_recept != NULL);
	if (!workers || time <= 0.f)
		return 0;
	// vypocitat cas
	m_desttime -= time * workers;
	if (m_desttime > 0.f) // jeste neni cas
		return 0;

	float newtime;
	sscanf(m_recept->GetString(), "%f", &newtime);
	s_passes = (int)(-m_desttime/newtime);
	s_passes++;
	m_desttime += newtime * s_passes;

	// odecist a nastavit na vybrani
	s_num_in = s_num_out = 0;

	return s_passes;
}


bool Workspace2::operator << (ResourceBase & in)
{
	// prenastavit s_passes na to kolik se jich tam vejde
	if (s_passes <= 0)
		return false;

	// spocitat kolik by potreboval
	const char type = s_sur[in.GetType()];
	const char * p = m_recept->GetString();
	uint req=0;
	while (*p && *p!=type) p++; // vyjde nastejno jako hledani v tabulce
	if (!*p || sscanf(p+1, "%d", &req)!=1)
		return false;
	
	// mam req
	// mam num
	// muzu prenastavit passy
	int passes = in.GetNum() / req;
	if (passes < s_passes)
		s_passes = passes;

	s_req_in[s_num_in].res = &in;
	s_req_in[s_num_in].num = req;
	s_num_in++;

	return true;
}

bool Workspace2::operator >> (ResourceBase & out)
{
	// prenastavit s_passes na to kolik se jich tam vejde
	if (s_passes <= 0)
		return false;

	// spocitat kolik by potreboval
	const char type = s_sur[out.GetType()];
	const char * p = m_recept->GetString();
	uint add=0;
	while (*p && *p!='=') p++; // vyjde nastejno jako hledani v tabulce
	if (!*p || sscanf(p+1, "%d", &add)!=1)
		return false;
	
	// mam req
	// mam num
	// muzu prenastavit passy
	// jen omezeni na velikost odebrani -- int passes = in.GetNum() / req;
	//if (passes < s_passes)
	//	s_passes = passes;

	s_req_out[s_num_out].res = &out;
	s_req_out[s_num_out].num = (float)add;
	s_req_out[s_num_out].type = RequestOut::EResource;
	s_num_out++;

	return true;
}

bool Workspace2::operator >> (float & out)
{
	// prenastavit s_passes na to kolik se jich tam vejde
	if (s_passes <= 0)
		return false;

	// spocitat kolik by potreboval
	const char * p = m_recept->GetString();
	float add=0;
	while (*p && *p!='=') p++; // vyjde nastejno jako hledani v tabulce
	if (!*p || sscanf(p+1, "%f", &add)!=1)
		return false;
	
	// mam req
	// mam num
	// muzu prenastavit passy
	// jen omezeni na velikost odebrani -- int passes = in.GetNum() / req;
	//if (passes < s_passes)
	//	s_passes = passes;

	s_req_out[s_num_out].f = &out;
	s_req_out[s_num_out].num = add;
	s_req_out[s_num_out].type = RequestOut::EFloat;
	s_num_out++;

	return true;
}

void Workspace2::Commit()
{
	int i;
	if (s_passes <= 0)
		return;

	// odecist
	for (i=0;i < s_num_in;i++)
		s_req_in[i].res->Get(s_req_in[i].num*s_passes,true);
	for (i=0;i < s_num_out;i++)
	{
		switch (s_req_out[i].type)
		{
		case RequestOut::EResource: {
			uint n = (uint)s_req_out[i].num*s_passes;
			s_req_out[i].res->Add(&n,1000);
			} break;
		case RequestOut::EFloat: {
			*s_req_out[i].f += s_req_out[i].num*s_passes;
			} break;
		};
	}
	s_num_in = s_num_out = 0;
}

///////////////////////////////////////////////////////////
// CHIEF
int Chief::s_stats[EBW_Max];
Chief * Chief::s_lastupdater = NULL;

Chief::Chief()
{
    m_worked = 0;
}

ESurType GetSur(char n)
{
	for (int t=EBS_Becher;t <= EBS_Coal;t++)
		if (s_sur[t] == n)
			return (ESurType)t;
	return EBS_None;
}

int Chief::Make(BecherObject * owner,const char * cmd)
{
    // rozparsovat command
    // napr. "W=>S" - jeden tupoun prestane pracovat
    //       "3W=>S" - tri tupouni prestanou pracovat
    //       "2=>IC" - 2 volny tupouni pujdou pro trtinu
    GetCon()->Printf("Chief cmd `%s'",cmd);

	if (*cmd == 'N')
	{
		if (m_list.Count() >= 4)
		{
			GetLevel()->GetPanel()->GetInfo()->Add(GetLang()->GetString(151));
			return 0;
		}
		TTroll t;
		t.troll = GetLevel()->CreateTroll();
		t.type = EBW_Wait;
		t.troll->Show(true);
		SendGameMsg(t.troll, BMSG_Go, 0, owner,1);
		//t.troll->Go(100,100);
		m_list.Add(t);
		GetLevel()->GetPanel()->GetInfo()->Add(GetLang()->GetString(150));
		// dat pokyn aby sel do budovy
		ResetStat(false);
		// dat pokyn na vchod do budovy
		return 1;
	}
	// zjistit odkud..
	EWorkType from;
	if (*cmd == 'F')
	{
		from = EBW_Wait; cmd++;
	}
	else if (*cmd == 'W')
	{
		from = EBW_Work; cmd++;
	}
	while (*cmd == '>') cmd++;
	// kam
	if (*cmd == 'I')
	{
		ESurType sur = GetSur(cmd[1]);
		// najit trolla
		//
		for (uint t=0;t < m_list.Count();t++)
		{
			if (m_list[t].type == from)
			{
				PAR_Favour fav = { NULL, NULL, sur, 10, 0 };
				// nejak ziskat odkud to ma prinest
				if (fav.num > v_troll_num.GetInt())
					fav.num = v_troll_num.GetInt();
				if (SendGameMsg(owner, BMSG_CreateImport, 0, &fav, 5))
				{
					SendGameMsg(m_list[t].troll, BMSG_Import, 0, &fav, 5);
					SetJob(t, EBW_Import(sur));
				}
				return 1;
			}
		}
	}
	else if (*cmd == 'W')
	{
		for (uint t=0;t < m_list.Count();t++)
		{
			if (m_list[t].type == from)
			{
				SetJob(t, EBW_Work);
				return 1;
			}
		}

	}
    // register
    // 

	return 0;
}

void Chief::ComputeStatistik()
{
    memset(s_stats, 0, sizeof(s_stats));
    for (int i=0;i < m_list.Count();i++)
        s_stats[m_list[i].type]++;
    m_worked = s_stats[EBW_Work];
    s_lastupdater = this;
}

void Chief::SetJob(int t, EWorkType type)
{
	m_list[t].type = type;
	switch (type)
	{
	case EBW_Work:
		ResetStat(true); 
		break;
	default:
		ResetStat(false);
	};
}

int Chief::GetNumWorkers(EWorkType type)
{
    switch ((int)type)
    {
    case EBW_Work:
        return m_worked;
    default:
        // if stare tak prepocitat
        if (s_lastupdater != this)
            ComputeStatistik();
        return s_stats[type];
    }
}

// nastavi trolla na status free
void Chief::Incoming(Troll * t)
{
	for (int i=0;i < m_list.Count();i++)
        if (m_list[i].troll == t)
		{
			m_list[i].type = EBW_Wait;
			//t->Show(false);
			ResetStat(false);
			return;
		}
	hoe_assert(!"Troll not registered in this building.");
}

















