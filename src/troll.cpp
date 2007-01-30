
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "crr.h"
#include "troll.h"
#include "phys_prov.h"
#include "b_msg.h"

static CVar v_speed("troll_speed", 35.f, TVAR_SAVE);
CVar v_troll_num("troll_num", 10, TVAR_SAVE);
static CVar v_cost_work("troll_cost_work", 0.1f, TVAR_SAVE);
static CVar v_cost_bring("troll_cost_bring", 0.1f, TVAR_SAVE);
static CVar v_cost_wait("troll_cost_wait", 0.1f, TVAR_SAVE);
static const float scale = 0.35f;

Troll::Troll(IHoeScene * scn) : BecherObject(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_TROLL));
	GetCtrl()->SetScale(HoeMath::Vector3(scale,scale,scale));
	GetCtrl()->SetFlags(HOF_SHOW|HOF_UPDATE|HOF_SCALED);
	SetRingParam(.8f,.8f,2.f);
	memset(&m_job, 0, sizeof(m_job));
	m_load.num = 0;
	m_load.sur = EBS_None;
	m_action = EA_None;
	anim = 0.f;
}

Troll::~Troll()
{
}

bool Troll::Save(BecherGameSave &w)
{
	BecherObject::Save(w);
	// ulozit job a path
	this->m_job.Save(w);
	w.WriteValue<bool>(0);
	w.WriteValue<uint>(0);
	w.WriteValue<uint>(0);
	w.WriteValue<dword>(0);
	// path
	w.WriteValue<bool>(false);
	return true;
}

bool Troll::Load(BecherGameLoad &r)
{
	BecherObject::Load(r);
	//m_job.Load(r);	
	r.Read<bool>();
	r.Read<uint>();
	r.Read<uint>();
	(ESurType)r.Read<dword>();
	// path
	r.Read<bool>();
	return true;
}

bool Troll::Step(float t)
{
	anim += ((t * v_speed.GetFloat()) / (scale * 20.f)) ;
	if (anim > 1.f)
		anim = 0.f;
	this->SetAnimationTime(anim);
	return m_path.Step(this, (float)t*v_speed.GetFloat());
}

void Troll::Update(const float t)
{
#ifndef BECHER_EDITOR
	switch (m_action)
	{
	case EA_NewJob:
		Finish();
		break;
	case EA_Go:
		if (Step(t))
			Finish();
		GetLevel()->GetMJobs()->AddPay(v_cost_bring.GetFloat() * t);
		break;
	};
	// pokud chodi tak chodi
	// pokud ceka, tak nic
	/*switch (m_job.type)
	{
	case TJob::jtFly:
		// jde pryc
		anim += ((t * v_speed.GetFloat()) / (scale * 20.f)) ;
		if (anim > 1.f)
			anim = 0.f;
		this->SetAnimationTime(anim);
		if (m_path.Step(this, (float)t*v_speed.GetFloat()))
			Finish();
		break;
	case TJob::jtGotoRes:
	case TJob::jtGotoOwnerWithRes:
	case TJob::jtGotoWork:
	case TJob::jtGoto:
		// update cesty, pokud cesta hotova tak finish
		anim += ((t * v_speed.GetFloat()) / (scale * 20.f)) ;
		if (anim > 1.f)
			anim = 0.f;
		this->SetAnimationTime(anim);
		if (m_path.Step(this, (float)t*v_speed.GetFloat()))
		{
			// job finish
			Finish();
		}
		// prachy
		
		break;
	case TJob::jtFindJob:
		GetLevel()->GetMJobs()->AddPay(v_cost_wait.GetFloat() * t);
		break;
	case TJob::jtWork:
		GetLevel()->GetMJobs()->AddPay(v_cost_work.GetFloat() * t);
		break;
	};*/
#endif
}

void Troll::Finish()
{
	bool repeat = false;
	do {
	repeat = false;
	// release action
	TJob::EType * job = m_job.Get();
	switch (job[0])
	{
	case TJob::EJT_Go:
		m_action = EA_Go;
		switch (job[1])
		{
		case TJob::EJP_Point:
			m_path.Go(m_job.point.x, m_job.point.y); break;
		case TJob::EJP_Source:
			m_path.Go(m_job.source); break;
		case TJob::EJP_Owner:
			m_path.Go(m_job.owner); break;
		};
		m_job.Skip(2);
		break;
	case TJob::EJT_Incoming:
		SendGameMsg(m_job.owner, BMSG_TrollIncoming, 0, this);
		m_job.Skip(2);
		repeat = true;
		break;
	case TJob::EJT_GetSur:
		// vykopirovat z objednavky
		m_load.sur = m_job.favour.sur;
		m_load.num = m_job.favour.num;
		m_load.num = SendGameMsg(m_job.source, BMSG_GetSur, m_job.favour.locked, &m_load, 2);
		m_job.Skip(1);
		repeat = true;
		break;
	case TJob::EJT_InsertSur:
		m_load.num = SendGameMsg(m_job.owner, BMSG_InsertSur, 0, &m_load, 2);
		m_job.Skip(1);
		repeat = true;
		break;
	default:
		m_action = EA_None;
	};
	} while (repeat);
}

#ifndef BECHER_EDITOR
bool Troll::Select()
{
	BecherObject::Select();
	GetLua()->func("s_tupoun");
	return true;
}
#else
bool Troll::Select()
{
	BecherObject::Select();
	return true;
}

#endif

int Troll::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Go: {
		HoeMath::Vector2 pos;
		if (npar2 == 1)
		{
			pos = reinterpret_cast<BecherObject*>(par2)->GetPos();
		}
		else
		{ hoe_assert(npar2 == 2);
		 pos = *(HoeMath::Vector2 *)par2;
		}
		m_job.Go(pos.x, pos.y);
		m_action = EA_NewJob;
		//j.type = TJob::jtGoto;
		//this->m_path.Go();
		//SetJob(j);
		return 0; }
	case BMSG_Import:
		m_job.Import(reinterpret_cast<PAR_Favour*>(par2));
		m_action = EA_NewJob;
		return 0;
	case BMSG_RightClick:

		return 0;
	default:
		return BecherObject::GameMsg(msg, par1, par2, npar2);
	};
	return -1;
}

////////////////////////////////////////////////
/*JobEx::JobEx()
{
	SetNone();
	phase = JobEx::GoToNew;
}

const Job & JobEx::operator = (const Job & j)
{
	this->type = j.type;
	this->owner = j.owner;
	this->num = j.num;
	this->surtype = j.surtype;
	this->phase = JobEx::GoTo;
	switch (j.type)
	{
	case Job::jtPrines:
		//path.SetPosTo(j.store);
		break;
	case Job::jtWork:
		path.SetPosTo(j.owner);
		break;
	};
	return *this;
}

void JobEx::SetNone()
{
	type = jtNone;
}*/

bool TrollPath::Go(float tx, float ty)
{
	m_stack.SetCount(0);
	Insert(tx, ty, true);
	act.virt = true;
	return true;
}

bool TrollPath::Go(BecherObject *to)
{
	return Go(to->GetPosX(), to->GetPosY());
}


bool TrollPath::Step(Troll * t, const float time)
{

    bool finish = true;
#ifndef BECHER_EDITOR
    float posX = t->GetPosX();
	float posY = t->GetPosY();
	float puvX = posX;
	float puvY = posY;
	finish = GetNextPos(time, posX,posY);
	float angle = -atan2f(-posX*10.f+puvX*10.f,-posY*10.f+puvY*10.f);
	t->SetAngle(angle);
	// nastavit pozici podle terenu
	t->SetPosition( posX, posY, GetLevel()->GetScene()->GetScenePhysics()->GetHeight(posX,posY));
#endif
	return finish;
}

#ifndef BECHER_EDITOR

bool TrollPath::GetNextPos(float l,float &px, float &py)
{
	if (this->m_stack.IsEmpty())
		return true;
	
	const Point &p = m_stack.GetTop();
	if (p.virt)
	{
		// cesta je virtualni, musi by se dohledat
		// napojeni na mapu a ziskani zeme
		// nejspis u levelu
        	HoeMath::Vector2 to = p.pos;
        	m_stack.Pop();
        	// najit cestu k to a vlozit
        	if (!GetLevel()->FindPath(HoeMath::Vector2(px,py),to,*this))
				return true; // todo
		return false;
	}
	float ux = p.pos.x - px;
	float uy = p.pos.y - py;
	float mag = sqrt(ux * ux + uy * uy);
	ux /= mag;
	uy /= mag;
	if (l < mag)
	{
		px += ux * l;
		py += uy * l;
		return false;
	}
	px = p.pos.x;
	py = p.pos.y;

	m_stack.Pop();

	return m_stack.IsEmpty();
}

#endif

