
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_store.h"

float getheight(IHoeModel * m);

static CVar v_sklad("store_max", 50, 0);
static CVar v_numworks("store_maxwork", 4, 0);

StoreStatic Store::m_userhud;

StoreStatic::StoreStatic()
{
	m_act = NULL;
}

void StoreStatic::SetAct(Store * act)
{
	m_act = act;
	// pripojit 
	dynamic_cast<HoeGame::Font*>(ReqItem("trtina"))->SetPtr(m_caneinfo);
	dynamic_cast<HoeGame::Font*>(ReqItem("drevo"))->SetPtr(m_woodinfo);
	dynamic_cast<HoeGame::Font*>(ReqItem("kamen"))->SetPtr(m_stoneinfo);
	dynamic_cast<HoeGame::Font*>(ReqItem("byliny"))->SetPtr(m_herbeinfo);
	dynamic_cast<HoeGame::Font*>(ReqItem("voda"))->SetPtr(m_waterinfo);
	dynamic_cast<HoeGame::Font*>(ReqItem("cukr"))->SetPtr(m_sugarinfo);
	dynamic_cast<HoeGame::Font*>(ReqItem("lih"))->SetPtr(m_alcoinfo);
	dynamic_cast<HoeGame::Font*>(ReqItem("becher"))->SetPtr(m_becherinfo);
}

void StoreStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		sprintf(m_sugarinfo,"%d cukru.", m_act->m_sugar.GetNum());
		sprintf(m_caneinfo,"%d trtina.", m_act->m_cane.GetNum());
		sprintf(m_woodinfo,"%d dreva.", m_act->m_wood.GetNum());
		sprintf(m_stoneinfo,"%d sutru.", m_act->m_stone.GetNum());
		sprintf(m_herbeinfo,"%d bylin.", m_act->m_herbe.GetNum());
		sprintf(m_waterinfo,"%d vody.", m_act->m_water.GetNum());
		sprintf(m_alcoinfo,"%d lihu.", m_act->m_alcohol.GetNum());
		sprintf(m_becherinfo,"%d flasek.", m_act->m_becher.GetNum());

		ObjectHud::Draw(h2d);
	}
}

////////////////////////////////////////////////////////////
Store::Store(IHoeScene * scn) : BecherBuilding(scn), 
	m_stone(EBS_Stone), m_wood(EBS_Wood), m_sugar(EBS_Sugar), m_water(EBS_Water),
	m_becher(EBS_Becher), m_alcohol(EBS_Alco), m_cane(EBS_Cane), m_herbe(EBS_Herbe)
{
	// set owners
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_STORE));
	m_infoselect.s_x = 2.5f;
	m_infoselect.t_y = 3.f;
	m_infoselect.s_z = 2.5f;

	memset(&m_info, 0, sizeof(m_info));
	const float p = getheight(GetModel()); 
	for (int i=0;i < 16;i++)
	{
		m_info[i].model = dynamic_cast<IHoeModel*>(GetEngine()->Create("model sud"));
		m_info[i].s_x = m_info[i].s_y = m_info[i].s_z = 1.f;
		m_info[i].t_x = (float)8*(i/4)-12;
		m_info[i].t_y = p;
		m_info[i].t_z = (float)8*(i%4)-12;
		
		GetCtrl()->Link(THoeSubObject::Object, &m_info[i]);
	}

	m_stone.SetOwner(this); CRR::Get()->Register(&m_stone);
	m_wood.SetOwner(this); CRR::Get()->Register(&m_wood);
	m_sugar.SetOwner(this); CRR::Get()->Register(&m_sugar);
	m_water.SetOwner(this); CRR::Get()->Register(&m_water);
	m_becher.SetOwner(this); CRR::Get()->Register(&m_becher);
	m_alcohol.SetOwner(this); CRR::Get()->Register(&m_alcohol);
	m_cane.SetOwner(this); CRR::Get()->Register(&m_cane);
	m_herbe.SetOwner(this); CRR::Get()->Register(&m_herbe);
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

	static IHoeModel * mod[] = {dynamic_cast<IHoeModel*>(GetEngine()->Create("model miska")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model sud")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model kblok")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_becher")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_cukr")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_dul")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model kmen")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model vedro"))};
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
	w.WriteRI(m_stone);
	w.WriteRI(m_wood);
	w.WriteRI(m_sugar);
	w.WriteRI(m_water);
	w.WriteRI(m_becher);
	w.WriteRI(m_alcohol);
	return true;
}

bool Store::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadRI(m_stone);
	r.ReadRI(m_wood);
	r.ReadRI(m_sugar);
	r.ReadRI(m_water);
	r.ReadRI(m_becher);
	r.ReadRI(m_alcohol);
	return true;
}

#ifndef BECHER_EDITOR

ResourceExp * Store::EBSToPointer(ESurType type)
{
	switch (type)
	{
	case EBS_Becher:
		return &m_becher;
	case EBS_Cane:
		return &m_cane;
	case EBS_Sugar:
		return &m_sugar;
	case EBS_Alco:
		return &m_alcohol;
	case EBS_Stone:
		return &m_stone;
	case EBS_Wood:
		return &m_wood;
	case EBS_Water:
		return &m_water;
	case EBS_Herbe:
		return &m_herbe;
	default:
		assert(!"Unknown sur. type");
	}
	return NULL;
}

bool Store::InsertSur(ESurType type, uint *s)
{
	return EBSToPointer(type)->Add(s, 10000000);
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

void Store::Update(const double t)
{
}

int Store::GetStatus(ESurType type)
{
	return EBSToPointer(type)->GetNum();
}

bool Store::Idiot(Job *t)
{
	// zjistit pripadny zdroj pro suroviny
	// 
	// navalit informace do tabulky, bud z crr nebo primo vybrane uloziste
	/*ResourceExp * ri = CRR::Get()->Find(EBS_Cane); // urceni priorit
	
	HoeGame::LuaFunc f(GetLua(), "i_Store");
	f.PushTable();
	// suroviny
	// informace o surovinach
	f.SetTableInteger("max_store", v_sklad.GetInt());
	f.SetTableInteger("cane_avail", ri ? ri->GetNum():0);
	f.SetTableInteger("cane", m_cane.GetNum());
	f.SetTableInteger("Store", m_sugar.GetNum());
	// works
	f.SetTableInteger("works", this->m_worked.Count());
	f.SetTableInteger("works_max", v_numworks.GetInt());
	f.Run(1);
	if (f.IsNil(-1))
	{
		f.Pop(1);
		return false;
	}

	// prevest zpatky na job
	int r = f.GetTableInteger("type", -1); // typ prace
	j->percent = f.GetTableFloat("percent", -1); // na kolik procent je vyzadovano
	j->owner = this;
	switch (r)
	{
	case 0:
		j->surtype = (ESurType)f.GetTableInteger("sur", -1); // typ suroviny
		j->type = Job::jtPrines;
		j->num = f.GetTableInteger("num", -1); // pocet k prineseni
		j->ritem = ri;
		break;
	case 1:
		j->type = Job::jtWork;
		break;
	};
		
	f.Pop(1);
	
	return true;*/
	return false;
}

bool Store::Select()
{
	BecherBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_sklad");
	return true;
}

#else
bool Store::Select()
{
	FactoryBuilding::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Materials"));
	GetProp()->AppendLong(0, _("Wood"), m_wood.GetNum());
	GetProp()->AppendLong(1, _("Stone"), m_stone.GetNum());
	GetProp()->AppendLong(2, _("Water"), m_water.GetNum());
	GetProp()->AppendLong(3, _("Sugar"), m_sugar.GetNum());
	GetProp()->AppendLong(4, _("Alcohol"), m_alcohol.GetNum());
	GetProp()->AppendLong(5, _("Bechers"), m_becher.GetNum());
	GetProp()->AppendCategory(_("Store"));
	GetProp()->AppendLong(6, _("Limit"), 100);
	GetProp()->End();	
	return true;
}

void Store::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 0:
		m_wood.SetNum(pi.GetLong());
		break;
	case 1:
		m_stone.SetNum(pi.GetLong());
		break;
	case 2:
		m_water.SetNum(pi.GetLong());
		break;
	case 3:
		m_sugar.SetNum(pi.GetLong());
		break;
	case 4:
		m_alcohol.SetNum(pi.GetLong());
		break;
	case 5:
		m_becher.SetNum(pi.GetLong());
		break;
	};
}



#endif

