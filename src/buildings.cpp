
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "id.h"
#include "troll.h"
#include "obj_store.h"
#include "obj_construct.h"
#include "b_msg.h"

CVar v_idiottime("time_idiot", 2.f, 0); // jak casto se ma poustet idiot
static CVar v_autowork("sugar_auto", 0.f, TVAR_SAVE);

/////////////////////////////////////////////////////////
BecherBuilding::BecherBuilding(IHoeScene * scn) : BecherObject(scn)
{
	m_mode = EBM_None;
}

#ifndef BECHER_EDITOR
bool BecherBuilding::StartBuilding(int gold, int wood, int stone)
{
	if (!GetLevel()->GetCash()->Add(-gold))
	{
		GetLevel()->GetPanel()->GetInfo()->Add(GetLang()->GetString(104));
		return false;
	}
	// build
	SetMode(EBM_Build);

	return true;
}

#endif

bool BecherBuilding::Save(ChunkDictWrite &w)
{
	BecherObject::Save(w);
	//w.WriteValue<dword>(m_mode);
	return true;
}

bool BecherBuilding::Load(const ChunkDictRead &r)
{
	BecherObject::Load(r);
	//this->SetMode((EBuildingMode)r.Read<dword>());
	return true;
}

int BecherBuilding::StatusPlace(float *pos)
{
	// pozice v mape
#ifndef BECHER_EDITOR

	// type, model
	// vysku, destinaci od ostatnich ziskam z... v_sizzing

	float min,max;
	float x=pos[0];
	float y=pos[1];
	int type = GetType();
	float height = v_sizzing.GetFloat(SIZZ(type,ESSZ_height));
	float dobj = v_sizzing.GetFloat(SIZZ(type,ESSZ_dobj));
	bool ok;
	THoeParameter par;
	IHoeModel * m = GetModel();
	if (m)
		m->GetParameter("boundbox",&par);
	else
		memset(&par,0, sizeof(par));
	max = min = 0.f;
	// 177 132
	ok = GetLevel()->GetScene()->GetScenePhysics()->GetCamber(
		x+par.box.left,x+par.box.right,y+par.box.front,y+par.box.back,min,max);
	SetPosition(x,y,max);
	if (!ok || (max-min) > height) 
	{
		GetCtrl()->SetOverColor(0xffff0000);
		return 1;
	}
	// zjistit zda muze byt cerveny nebo jiny
	for (int i=0; i < GetLevel()->GetNumObj();i++)
	{
		float x = GetLevel()->GetObj(i)->GetPosX();
		float y = GetLevel()->GetObj(i)->GetPosY();
		x -= GetPosX();
		y -= GetPosY();
		if (x*x+y*y < dobj)
		{
			GetCtrl()->SetOverColor(0xffff0000);
			return 2;
		}
	}
	GetCtrl()->SetOverColor(0xffffffff);
#endif
	return 0;
}

int BecherBuilding::BuildPlace(float *pos)
{
	int ret = StatusPlace(pos);
#ifndef BECHER_EDITOR
	switch (ret)
	{
	case 0: // postavit
		GetCtrl()->SetOverColor(0xffffffff);
		break;
	case 1: // nevhodny teren
		SendGameMsgId(0, BMSG_Info, 0,(void*)GetLang()->GetString(101), 1);
		break;
	case 2: // blizko objektu
		SendGameMsgId(0, BMSG_Info, 0,(void*)GetLang()->GetString(102), 1);
		break;
	case 3:
		SendGameMsgId(0, BMSG_Info, 0,(void*)GetLang()->GetString(103), 1);
		break;
	};
#endif
	return ret;
}

float getheight(IHoeModel*m)
{
	THoeParameter p;
	m->GetParameter("boundbox", &p);
	return p.box.top;
}

int BecherBuilding::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_SelectPlace:
		return StatusPlace((float*)par2);
	case BMSG_StartBuilding:
		return BuildPlace((float*)par2);
	};
	return BecherObject::GameMsg(msg, par1, par2, npar2);
}

//////////////////////////////////////////////////////////////
// Construct building
ConstructBuilding::ConstructBuilding(IHoeScene * scn, CVar &v_build)
    : BecherBuilding(scn),
    m_build(&v_build), m_stone(EBS_Stone), m_wood(EBS_Wood)
{
	m_wood.SetOwner(this);
	m_stone.SetOwner(this);
	// build
	m_buildprogress = 1.1f;
}

uint ConstructBuilding::ReqResource(const char * recept, float progress, char r)
{
	const char * p = recept;
	while (*p && *p!='=') p++;
	if (!*p)
		return 0;
	float dav = 0.f;
	if (sscanf(p+1, "%f", &dav)!=1 || dav == 0.f)
		return 0;
	p = recept;
	while (*p && *p!=r) p++;
	if (!*p)
		return 0;
	int res = 0;
	if (sscanf(p+1, "%d", &res)!=1)
		return 0;
	float rem = (((1.f-progress)/dav)*res);
	if (rem < 0) return 0;
	return (uint)HoeMath::UpperRound(rem);
}

int ConstructBuilding::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
	case BINFO_NumSur|EBS_Stone:
		ret = (int)this->m_stone.GetNum();
		break;
	case BINFO_NumSur|EBS_Wood:
		ret = (int)this->m_wood.GetNum();
		break;
	case BINFO_ReqSur|EBS_Stone:
		ret = ReqResource(m_build.GetRecept(), m_buildprogress, 'K') - m_stone.GetNum();
		break;
	case BINFO_ReqSur|EBS_Wood:
		ret = ReqResource(m_build.GetRecept(), m_buildprogress, 'D') - m_wood.GetNum();
		break;
	case BINFO_CanSur|EBS_Stone:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Stone, this);
			if (ri) ret = ri->GetAvail();
		} break;
	case BINFO_CanSur|EBS_Wood:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Wood, this);
			if (ri) ret = ri->GetAvail();
		} break;
	case BINFO_BuildProgress:
		ret = (int)(this->m_buildprogress * 100);
		break;
	default:
		return BecherBuilding::GetInfo(type, str, n);
	};
	if (str)
		snprintf(str, n, "%d", ret);
	return ret;
}

int ConstructBuilding::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
    case BMSG_Chief:
        if (npar2 == 1)
			return m_chief.Make(this,reinterpret_cast<const char*>(par2));
		else
		{
			int n=0;
			for (uint i=0;i < npar2;i++)
				n+=m_chief.Make(this,reinterpret_cast<const char**>(par2)[i]);
			return n;
		}
        return 0;
	case BMSG_TrollIncoming:
		m_chief.Incoming((Troll*)par2);
		return 0;
    case BMSG_CreateImport: {
        // vytvoreni dodavky
        PAR_Favour * f = (PAR_Favour*)par2;
        hoe_assert(npar2 == 5);
        f->owner = this;
        // vyhledani zdroje
        ResourceExp * r = CRR::Get()->Find(f->sur, this);
        if (r == NULL) return 0;
        f->remote = r->GetOwner();
        // zalockovat??? asi jo
		f->locked = SendGameMsg(f->remote, BMSG_LockSur, 0, f, npar2);
        } return 1;
	case BMSG_InsertSur: {
		PAR_Load * l = (PAR_Load *)par2;
		if (l->sur == EBS_Wood)
			m_wood.Add((uint*)&l->num, 1000);
		else if (l->sur == EBS_Stone)
			m_stone.Add((uint*)&l->num, 1000);
		else 
			break;
		}
		return 0;
	}
	return BecherBuilding::GameMsg(msg, par1, par2, npar2);
}

void ConstructBuilding::Update(const float t)
{
	if (m_buildprogress < 1.0f && m_build.BeginPass(m_chief.GetNumWorkers(EBW_Work)+v_autowork.GetFloat(), t))
	{
		m_build << m_stone;
		m_build << m_wood;
		m_build >> m_buildprogress;
		m_build.Commit();
	}
    
    BecherBuilding::Update(t);
}

void ConstructBuilding::IdiotBuild()
{
#ifndef BECHER_EDITOR
	HoeGame::LuaFunc f(GetLua(), "i_sugarbuild");
    f.PushPointer((BecherObject*)this);
	f.PushTable();
	
	f.SetTableInteger("stone", m_stone.GetNum());
	f.SetTableInteger("wood", m_wood.GetNum());

	// works
	f.SetTableInteger("works_count", m_chief.GetNumWorkers(EBW_Work));
    f.SetTableInteger("works_cane", m_chief.GetNumWorkers(EBW_Import(EBS_Cane)));
    f.SetTableInteger("works_coal", m_chief.GetNumWorkers(EBW_Import(EBS_Coal)));
    f.SetTableInteger("works_sugar", m_chief.GetNumWorkers(EBW_Export(EBS_Sugar)));
	f.SetTableInteger("works_max", m_chief.GetMaxWorkers());
	f.Run(2);
#endif
}

//////////////////////////////////////////////////////////////
// Source building
SourceBuilding::SourceBuilding(IHoeScene * scn, CVar &v_build)
	: ConstructBuilding(scn, v_build) 
{
}

int SourceBuilding::GetInfo(int type, char * str, size_t n)
{
	return ConstructBuilding::GetInfo(type, str, n);
}

int SourceBuilding::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_MiningRegister:
		// zaregistrovat trolla na cekani
		m_line.Register(reinterpret_cast<Troll*>(par2), par1);
		return 0;
	case BMSG_MiningCancel:
		m_line.Cancel(reinterpret_cast<Troll*>(par2));
		return 0;
	};
	return ConstructBuilding::GameMsg(msg, par1, par2, npar2);
	
}

//////////////////////////////////////////////////////////////
// WorkBuilding
WorkBuilding::WorkBuilding(IHoeScene * scn, CVar &v_build)
    : ConstructBuilding(scn, v_build)
{

}

int WorkBuilding::GetInfo(int type, char * str, size_t n)
{
	return ConstructBuilding::GetInfo(type, str, n);
	/*register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
	default:
		return ConstructBuilding::GetInfo(type, str, n);
	};
	if (str)
		snprintf(str, n, "%d", ret);
	return ret;*/
}

int WorkBuilding::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	/*switch (msg)
	{
	};*/
	return ConstructBuilding::GameMsg(msg, par1, par2, npar2);
}

void WorkBuilding::Update(const float t)
{
    ConstructBuilding::Update(t);
}

//////////////////////////////////////////////////////////////
// Production building
ProductionBuilding::ProductionBuilding(IHoeScene * scn, CVar &v_build)
	: WorkBuilding(scn, v_build)
{

}


//////////////////////////////////////////////////////////////
// Factory building
FactoryBuilding::FactoryBuilding(IHoeScene * scn, CVar &v_recept, CVar &v_build)
	: WorkBuilding(scn, v_build),
	m_work(&v_recept), m_coal(EBS_Coal)
{
	m_coal.SetOwner(this);
}

int FactoryBuilding::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
    case BINFO_NumSur|EBS_Coal:
		ret = (int)this->m_coal.GetNum();
		break;
	case BINFO_CanSur|EBS_Coal:
		{ ResourceExp * ri = CRR::Get()->Find(EBS_Coal, this);
			if (ri) ret = ri->GetAvail();
		} break;
	default:
		return WorkBuilding::GetInfo(type, str, n);
	};
	if (str)
		snprintf(str, n, "%d", ret);
	return ret;
}

int FactoryBuilding::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_InsertSur: {
		PAR_Load * l = (PAR_Load *)par2;
		if (l->sur == EBS_Coal)
			m_coal.Add((uint*)&l->num, 1000);
		else 
			break;
		}
		return 0;
	}
	return WorkBuilding::GameMsg(msg, par1, par2, npar2);
}

void FactoryBuilding::Update(const float t)
{
    WorkBuilding::Update(t);
}














