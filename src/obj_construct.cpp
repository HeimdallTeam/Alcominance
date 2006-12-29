
#include "StdAfx.h"
#include "becher.h"
#include "obj_construct.h"
#include "troll.h"

static CVar v_numzpr("construct_speed", 1.f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)

Construct::Construct(BecherBuilding * owner)
{
	m_owner = owner;
}

bool Construct::Save(BecherGameSave &w)
{
	return true;
}

bool Construct::Load(BecherGameLoad &r)
{
	return true;
}

#ifndef BECHER_EDITOR

void Construct::SetResources(uint wood, uint stone)
{ 
	m_actwood = m_reqwood = wood;
	m_actstone = m_reqstone = stone;
}


void Construct::Update(const float t)
{
	// updejt konstruktor
	m_acttime -= t;
	if (m_acttime <= 0.f)
		Finish();
}

bool Construct::Select()
{
	GetLua()->func("s_construct");
	return true;
}

bool Construct::Idiot(TJob * j)
{
	// idiot pro staveni
	// vybrat pro noseni
	ResourceExp * rw = CRR::Get()->Find(EBS_Wood, m_owner);
	ResourceExp * rs = CRR::Get()->Find(EBS_Stone, m_owner);

	// najit sklad pro 
	HoeGame::LuaFunc f(GetLua(), "i_construct");
	f.PushTable();
	f.SetTableInteger("wrk_count", m_worked.Count());
	f.SetTableInteger("wrk_count_wood",m_wrk_wood);
	f.SetTableInteger("wrk_count_stone",m_wrk_stone);
	f.SetTableInteger("wood_avail", rw ? rw->GetAvail():0);
	f.SetTableInteger("wood_rem",m_actstone);
	f.SetTableInteger("wood",0);
    f.SetTableInteger("stone_avail", rs ? rs->GetAvail():0);
	f.SetTableInteger("stone_rem",m_actwood);
	f.SetTableInteger("stone",0);
	f.Run(1);

	if (f.IsNil(-1))
	{
		f.Pop(1);
		return false;
	}

	// prevest zpatky na job
	int r = f.GetTableInteger("type", -1); // typ prace
	j->percent = f.GetTableFloat("percent", -1); // na kolik procent je vyzadovano
	j->owner = m_owner;
	switch (r)
	{
	case 0:
		j->surtype = (ESurType)f.GetTableInteger("sur", -1); // typ suroviny
		j->type = TJob::jtGotoRes;
		j->num = f.GetTableInteger("num", -1); // pocet k prineseni
		j->from = j->surtype == EBS_Stone ? rs:rw;
		j->to = m_owner;
		break;
	case 1:
		j->type = TJob::jtGotoWork;
		break;
	};
		
	f.Pop(1);
	
	return true;
}

bool Construct::SetToWork(Troll * t)
{
    switch (t->GetJob().type)
	{
    case TJob::jtWork:
	    m_worked.Add(t);
        return true;
    case TJob::jtGotoRes:
        switch (t->GetJob().surtype){
        case EBS_Stone:
            m_wrk_stone++;
            return true;
        case EBS_Wood:
            m_wrk_wood++;
            return true;
        }
        break;
    }
	return false;
}

void Construct::UnsetFromWork(Troll * t)
{
    switch (t->GetJob().type){
    case TJob::jtWork:
	    m_worked.Remove(t);
        break;
    case TJob::jtGotoRes:
        switch (t->GetJob().surtype){
        case EBS_Wood:
            m_wrk_wood--;
            break;
        case EBS_Stone:
            m_wrk_stone--;
            break;
        }
    }
}

bool Construct::InsertSur(ESurType type, uint *s)
{
	switch (type)
	{
	case EBS_Wood:
		this->m_actwood -= *s;
		if (m_actwood < 0)
			m_actwood = 0;
		return true;
	case EBS_Stone:
		this->m_actstone -= *s;
		if (m_actstone < 0)
			m_actstone = 0;
		return true;
	default:
		assert(!"insert bad type");
		return false;
	};
	return true;
}

void Construct::Finish()
{
	// dat budove vedet
	m_owner->SetMode(EBM_Normal);
	// jen pokud je vybrana
	if (GetLevel()->GetSelectedObject() == m_owner)
		m_owner->Select();
	// neco do infa?
	delete this; /* velmi nebezpecne */
}

#endif //







