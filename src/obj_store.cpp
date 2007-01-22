
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_store.h"
#include "panels.h"

float getheight(IHoeModel * m);

static CVar v_sklad("store_max", 160, 0);
static CVar v_numworks("store_maxwork", 4, 0);
static CVar v_cost("store_cost", 200, TVAR_SAVE); // cena za stavbu
static CVar v_cost_wood("store_cost_wood", 100, TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cost_stone("store_cost_stone", 150, TVAR_SAVE); // pocet kameni potrebneho na stavbu

IHoeModel * Store::g_models[EBS_Max] = {0};

void Store::LoadModels()
{
    if (g_models[EBS_Herbe] != NULL)
        return;
	g_models[EBS_Herbe] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model miska"));
	g_models[EBS_Alco] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model sud"));
	g_models[EBS_Stone] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model kblok"));
	g_models[EBS_Cane] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_becher"));
	g_models[EBS_Becher] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_becher"));
	g_models[EBS_Sugar] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_cukr"));
	g_models[EBS_Coal] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_dul"));
	g_models[EBS_Wood] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model kmen"));
	g_models[EBS_Water] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model vedro"));
}

////////////////////////////////////////////////////////////
Store::Store(IHoeScene * scn) : BecherBuilding(scn)
{
	// set owners
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_STORE));
	SetRingParam(2.5f,2.5f,3.f);
	memset(&m_info, 0, sizeof(m_info));
	const float p = getheight(GetModel()); 
	LoadModels();
	for (int i=0;i < 16;i++)
	{
		m_info[i].model = NULL;
		m_info[i].pos.Translate((float)8*(i/4)-12,p,(float)8*(i%4)-12);
	
		GetCtrl()->Link(THoeSubObject::Object, &m_info[i]);
	}

	for (int i=1;i<EBS_Max;i++)
	{
		m_res[i].SetType((ESurType)i);
		m_res[i].SetOwner(this); CRR::Get()->Register(&m_res[i]);
        m_res[i].SetNum(100);
	}
}

bool Store::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);

	for (int i=1;i < EBS_Max;i++)
		m_res[i].Save(w);

	return true;
}

bool Store::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);

	for (int i=1;i < EBS_Max;i++)
		m_res[i].Load(r);

	OnUpdateSur();
	return true;
}

int Store::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
#define STRREP(res, name) if (strcmp(str, name) == 0) type = BINFO_Num##res;
		STRREP(Cane,"cane")
		else STRREP(Sugar,"sugar")
		else STRREP(Coal,"coal")
		else STRREP(Stone,"stone")
		else STRREP(Alco,"alco")
		else STRREP(Becher,"becher")
		else STRREP(Wood,"wood")
		else STRREP(Water,"water")
		else STRREP(Herbe,"herbe")
#undef STRREP
	}
	switch (type)
	{
#define TYPECASE(res) case BINFO_Num##res: \
	ret = (int)this->m_res[EBS_##res].GetNum(); \
		if (str)\
			snprintf(str, n, "%d", ret); \
		return ret;

	TYPECASE(Sugar)
	TYPECASE(Coal)
	TYPECASE(Stone)
	TYPECASE(Alco)
	TYPECASE(Becher)
	TYPECASE(Wood)
	TYPECASE(Herbe)
	TYPECASE(Water)
#undef TYPECASE
	default:
		return BecherBuilding::GetInfo(type, str, n);
	};
	return 0;
}

int Store::GameMsg(int msg, int par1, void * par2, uint npar2)
{
#ifndef BECHER_EDITOR
	switch (msg)
	{
	case BMSG_Select:
		GetLevel()->GetPanel()->SetObjectHud("scripts/store.menu", this);
		GetLua()->func("s_sklad");
		break;
	case BMSG_SelectPlace:
	case BMSG_StartBuilding:
		return BuildPlace((float*)par2, 
			(IHoeModel*)GetResMgr()->ReqResource(model_SUGAR),50.f,200.f,msg==BMSG_StartBuilding);
	}
#endif 
	return BecherBuilding::GameMsg(msg, par1, par2, npar2);
}


#ifndef BECHER_EDITOR

bool Store::InsertSur(ESurType type, uint *s)
{
	bool ret = m_res[type].Add(s, *s);
	OnUpdateSur();
	return ret;
}

bool Store::SetToWork(Troll * t)
{
	if (m_worked.Count() >= (uint)v_numworks.GetInt())
		return false;
	m_worked.Add(t);
	return true;
}

void Store::UnsetFromWork(Troll * t)
{
	m_worked.Remove(t);
}

void Store::Update(const float t)
{
	// update po snimcic
}



bool Store::Idiot(TJob *t)
{
	// zjistit pripadny zdroj pro suroviny
	// 
	// navalit informace do tabulky, bud z crr nebo primo vybrane uloziste
	/*ResourceExp * ri = CRR::Get()->Find(EBS_Cane); // urceni priorit
	
	// vybrat surovinu co je potreba sehnat...
	// takze to bude probihat spise stylem spoluprace s crr
	projit vsechny, a najit surovinu
	
	return true;*/
	// vyplnit podle
	int req[EBS_Max] = {0};
	int poc = 0;
	for (int i=1;i < EBS_Max;i++)
		if (m_res[i].IsEnable())
			poc++;
	if (poc==0)
		return false;
	poc = v_sklad.GetInt() / poc;
	for (int i=1;i < EBS_Max;i++)
		req[i] = m_res[i].IsEnable() ? m_res[i].GetNum()-poc: 0xfffffff;
	// vybirat prvni mozne
	while (1)
	{
		// vybrat nejmensi
		int min=1;
		for (int i=2;i < EBS_Max;i++)
			if (req[min] > req[i])
				min = i;
		if (req[min] == 0xfffffff)
			return false;
		ResourceExp * ri = CRR::Get()->Find((ESurType)min, this);
		if (ri && ri->GetAvail() > 0)
		{
			// return job
			t->type = TJob::jtGotoRes;
			t->owner = this;
			t->from = ri;
			t->surtype = (ESurType)min;
			t->num = ri->GetAvail();
			if (t->num > 10)
				t->num = 10;
			t->percent = 100;
			return true;
		}
		else
			req[min] = 0xfffffff;
	}
	return false;
}

bool Store::Select()
{
	BecherBuilding::Select();
	return true;
}

#else
bool Store::Select()
{
	BecherBuilding::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Materials"));
	GetProp()->AppendLong(EBS_Wood, _("Wood"), m_res[EBS_Wood].GetNum());
	GetProp()->AppendLong(EBS_Stone, _("Stone"), m_res[EBS_Stone].GetNum());
	GetProp()->AppendLong(EBS_Water, _("Water"), m_res[EBS_Water].GetNum());
	GetProp()->AppendLong(EBS_Sugar, _("Sugar"), m_res[EBS_Sugar].GetNum());
	GetProp()->AppendLong(EBS_Alco, _("Alcohol"), m_res[EBS_Alco].GetNum());
	GetProp()->AppendLong(EBS_Becher, _("Bechers"), m_res[EBS_Becher].GetNum());
	GetProp()->AppendLong(EBS_Cane, _("Cane"), m_res[EBS_Cane].GetNum());
	GetProp()->AppendLong(EBS_Herbe, _("Herbe"), m_res[EBS_Herbe].GetNum());
	GetProp()->AppendLong(EBS_Coal, _("Coal"), m_res[EBS_Coal].GetNum());

	GetProp()->AppendCategory(_("Store"));
	GetProp()->AppendLong(20, _("Limit"), 100);
	GetProp()->End();	
	return true;
}

void Store::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	if (id > EBS_None && id < EBS_Max)
		m_res[id].SetNum(pi.GetLong());
	/*else
	switch (id)
	{
	case 20:
		m_coal.SetNum(pi.GetLong());
		break;
	};*/
	OnUpdateSur();
}

#endif

void Store::OnUpdateSur()
{
	// project a vypocitat
	// maximum = ;
	int max = GetMiniStoreCount();
	if (v_sklad.GetInt() > max)
		max = v_sklad.GetInt();

	max = max / 16;
	int p = 0;
	for (int i=1;i < EBS_Max;i++)
	{
		int c = m_res[i].GetNum() > 0 ? m_res[i].GetNum() / max+1:0;
		for (;c > 0;c--)
		{
			m_info[p++].model = g_models[i];
			if (p >= 16)
				return;
		}
	}
	for (;p <= 16;p++)
		m_info[p].model = NULL;

}

int Store::GetStatus(ESurType type)
{
	return m_res[type].GetNum();
}

uint Store::AcceptSur(ESurType type)
{
	return 10;
}
