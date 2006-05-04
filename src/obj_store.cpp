
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_store.h"

float getheight(IHoeModel * m);

////////////////////////////////////////////////////////////
Store::Store(IHoeScene * scn) : BecherBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_STORE));
	GetCtrl()->SetFlags(HOF_ADVSHOW);
	m_stone = m_wood = m_sugar = m_water = m_becher = m_alcohol = 0;
	m_cane = 10000;
}


void Store::AdvPaint(IHoePaint3D * h3)
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

}

bool Store::Save(HoeFileWriter &w)
{
	BecherBuilding::Save(w);
	/*
	w.Write<int>(m_stone);
	w.Write<int>(m_wood);
	w.Write<int>(m_sugar);
	w.Write<int>(m_water);
	w.Write<int>(m_becher);
	w.Write<int>(m_alcohol);
	*/
	return true;
}

bool Store::Load(int ver, HoeFileReader &r)
{
	BecherBuilding::Load(ver, r);
	/*
	m_stone = r.Read<int>();
	m_wood = r.Read<int>();
	m_sugar = r.Read<int>();
	m_water = r.Read<int>();
	m_becher = r.Read<int>();
	m_alcohol = r.Read<int>();
	*/
	return true;
}

#ifndef BECHER_EDITOR

int * Store::EBSToPointer(ESurType type)
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
	default:
		assert(!"Unknown sur. type");
	}
	return NULL;
}

int Store::AddSur(ESurType type, int s)
{
	(*EBSToPointer(type))  += s;
	return 0;
}

int Store::GetSur(ESurType type, int req, bool upln)
{
	int * s = EBSToPointer(type);
	if (req <= *s)
	{
		*s -= req;
		return req;
	}
	else if (!upln)
	{
		req = *s;
		*s = 0;
		return req;
	}
	else
	{
		return 0;
	}
}

void Store::Update(const double t)
{
}

int Store::GetStatus(ESurType type)
{
	return *EBSToPointer(type);
}

bool Store::Idiot(Troll *t)
{
	return false;
}

void Store::AddToWork(Troll *t)
{
}

int Store::GetNumInfos()
{
	return 7;
}

int Store::GetInfo(int id, char * buff, size_t size)
{ 
	switch (id)
	{
	case 0:
		sprintf(buff, "Stone: %d", m_stone);break;
	case 1:
		sprintf(buff, "Wood: %d", m_wood);break;
	case 2:
		sprintf(buff, "Sugar: %d", m_sugar);break;
	case 3:
		sprintf(buff, "Water: %d", m_water);break;
	case 4:
		sprintf(buff, "Becher: %d", m_becher);break;
	case 5:
		sprintf(buff, "Lih: %d", m_alcohol);break;
	case 6:
		sprintf(buff, "Trtina: %d", m_cane);break;
	default:
		return 0;
	};
	return 1;
}

bool Store::Select()
{
	if (!IsBuildMode())
        GetLua()->func("s_sklad");
	return true;
}

#else
bool Store::Select()
{
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Materials"));
	GetProp()->AppendLong(0, _("Wood"), m_wood);
	GetProp()->AppendLong(1, _("Stone"), m_stone);
	GetProp()->AppendLong(2, _("Water"), m_water);
	GetProp()->AppendLong(3, _("Sugar"), m_sugar);
	GetProp()->AppendLong(4, _("Alcohol"), m_alcohol);
	GetProp()->AppendLong(5, _("Bechers"), m_becher);
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
		m_wood = pi.GetLong();
		break;
	case 1:
		m_stone = pi.GetLong();
		break;
	case 2:
		m_water = pi.GetLong();
		break;
	case 3:
		m_sugar = pi.GetLong();
		break;
	case 4:
		m_alcohol = pi.GetLong();
		break;
	case 5:
		m_becher = pi.GetLong();
		break;
	};
}



#endif


