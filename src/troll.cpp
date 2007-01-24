
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "crr.h"
#include "troll.h"
#include "phys_prov.h"
#include "b_msg.h"

static CVar v_speed("troll_speed", 35.f, TVAR_SAVE);
static CVar v_num("troll_num", 10, TVAR_SAVE);
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
	/*memset(&m_job, 0, sizeof(m_job));
	m_job.type = TJob::jtNone;
	m_load.locked = false;
	m_load.numsur = 0;
	m_load.surtype = EBS_None;*/

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
	w.WriteValue<bool>(m_load.locked);
	w.WriteValue<uint>(m_load.numlocked);
	w.WriteValue<uint>(m_load.numsur);
	w.WriteValue<dword>(m_load.surtype);
	// path
	w.WriteValue<bool>(false);
	return true;
}

bool Troll::Load(BecherGameLoad &r)
{
	BecherObject::Load(r);
	//m_job.Load(r);	
	m_load.locked = r.Read<bool>();
	m_load.numlocked = r.Read<uint>();
	m_load.numsur = r.Read<uint>();
	m_load.surtype = (ESurType)r.Read<dword>();
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
		GetLevel()->GetMJobs()->AddPay(v_cost_bring.GetFloat() * t);
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
		TJob j = m_job;
		//j.type = TJob::jtGoto;
		this->m_path.Go(pos.x, pos.y);
		//SetJob(j);
		return 0; }
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

#ifndef BECHER_EDITOR

bool TrollPath::Step(Troll * t, const float time)
{
	bool finish;
	float posX = t->GetPosX();
	float posY = t->GetPosY();
	float puvX = posX;
	float puvY = posY;
	finish = GetNextPos(time, posX,posY);
	float angle = -atan2f(-posX*10.f+puvX*10.f,-posY*10.f+puvY*10.f);
	t->SetAngle(angle);
	// nastavit pozici podle terenu
	t->SetPosition( posX, posY, GetLevel()->GetScene()->GetScenePhysics()->GetHeight(posX,posY));
	return finish;
}

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

