
#include "StdAfx.h"
#include "becher.h"
#include "troll.h"
#include "obj_store.h"
#include "panels.h"

float getheight(IHoeModel * m);

static CVar v_sklad("store_max", 160, 0);
static CVar v_numworks("store_maxwork", 4, 0);

StoreStatic Store::m_storepref;

StoreStatic::StoreStatic()
{
	m_act = NULL;
}

void StoreStatic::SetAct(Store * act)
{
	m_act = act;
	// pripojit 
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetPtr(m_caneinfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("drevo", HoeGame::Gui::EText))->SetPtr(m_woodinfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("kamen", HoeGame::Gui::EText))->SetPtr(m_stoneinfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("byliny", HoeGame::Gui::EText))->SetPtr(m_herbeinfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("voda", HoeGame::Gui::EText))->SetPtr(m_waterinfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetPtr(m_sugarinfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("lih", HoeGame::Gui::EText))->SetPtr(m_alcoinfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("becher", HoeGame::Gui::EText))->SetPtr(m_becherinfo);
	dynamic_cast<HoeGame::Gui::Font*>(ReqItem("uhli", HoeGame::Gui::EText))->SetPtr(m_coalinfo);
}

void StoreStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		sprintf(m_sugarinfo,"%d cukru.", m_act->GetStatus(EBS_Sugar));
		sprintf(m_caneinfo,"%d trtina.", m_act->GetStatus(EBS_Cane));
		sprintf(m_woodinfo,"%d dreva.", m_act->GetStatus(EBS_Wood));
		sprintf(m_stoneinfo,"%d sutru.", m_act->GetStatus(EBS_Stone));
		sprintf(m_herbeinfo,"%d bylin.", m_act->GetStatus(EBS_Herbe));
		sprintf(m_waterinfo,"%d vody.", m_act->GetStatus(EBS_Water));
		sprintf(m_alcoinfo,"%d lihu.", m_act->GetStatus(EBS_Alco));
		sprintf(m_becherinfo,"%d flasek.", m_act->GetStatus(EBS_Becher));
		sprintf(m_coalinfo,"%d uhli.", m_act->GetStatus(EBS_Coal));

		ObjectHud::Draw(h2d);
	}
}

void StoreStatic::LoadModels()
{
	for (int i=0;i < EBS_Max;i++)
		m_models[i] = NULL;
	m_models[EBS_Herbe] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model miska"));
	m_models[EBS_Alco] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model sud"));
	m_models[EBS_Stone] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model kblok"));
	m_models[EBS_Cane] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_becher"));
	m_models[EBS_Becher] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_becher"));
	m_models[EBS_Sugar] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_cukr"));
	m_models[EBS_Coal] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_dul"));
	m_models[EBS_Wood] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model kmen"));
	m_models[EBS_Water] = dynamic_cast<IHoeModel*>(GetEngine()->Create("model vedro"));
}

////////////////////////////////////////////////////////////
Store::Store(IHoeScene * scn) : BecherBuilding(scn)
{
	// set owners
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_STORE));
	m_infoselect.s_x = 2.5f;
	m_infoselect.t_y = 3.f;
	m_infoselect.s_z = 2.5f;

	memset(&m_info, 0, sizeof(m_info));
	const float p = getheight(GetModel()); 
	m_storepref.LoadModels();
	for (int i=0;i < 16;i++)
	{
		m_info[i].model = NULL;
		m_info[i].s_x = m_info[i].s_y = m_info[i].s_z = 1.f;
		m_info[i].t_x = (float)8*(i/4)-12;
		m_info[i].t_y = p;
		m_info[i].t_z = (float)8*(i%4)-12;
		
		GetCtrl()->Link(THoeSubObject::Object, &m_info[i]);
	}

	for (int i=1;i<EBS_Max;i++)
	{
		m_res[i].SetType((ESurType)i);
		m_res[i].SetOwner(this); CRR::Get()->Register(&m_res[i]);
	}
}

/*void Store::AdvPaint(IHoePaint3D * h3)
{
	// info
	if (this->IsCurActive() || this->IsSelected())
	{
		static IHoeModel * mod_i = dynamic_cast<IHoeModel*>(GetEngine()->Create("model info"));
		h3->ResetPos();
		h3->Scale(2.5, 0, 2.5);
		h3->Move(0,2,0);
		h3->Paint(mod_i);
	}

	static IHoeModel * mod[] = {
	static float p = getheight(this->GetModel());
	int m = 0;
	for (int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			IHoeModel * model = mod[(m++)%8];
			if (model)
			{
				h3->ResetPos();
				//h3->RotateY(GetEngine()->SysFloatTime()*2);
				h3->Move((float)8*i-12, p, (float)8*j-12);
				h3->Paint(model);
			}
		}

}*/

bool Store::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);

	for (int i=1;i < EBS_Max;i++)
		w.WriteRI(m_res[i]);

	return true;
}

bool Store::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);

	for (int i=1;i < EBS_Max;i++)
		r.ReadRI(m_res[i]);

	OnUpdateSur();
	return true;
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
	GetLevel()->SetObjectHud(&m_storepref);
	m_storepref.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_sklad");
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
			m_info[p++].model = m_storepref.GetModel(i);
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
