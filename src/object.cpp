
#include "StdAfx.h"
#include "becher.h"
#include "object.h"

static THoeVarIndex idx_obj[] = { "ring_x", ESSZ_ring_x, TVAR_FLOAT, NULL,
								  "ring_y", ESSZ_ring_y, TVAR_FLOAT, NULL,
								  "ring_h", ESSZ_ring_h, TVAR_FLOAT, NULL,
								  "height", ESSZ_height, TVAR_FLOAT, NULL,
								  "dobj",   ESSZ_dobj,   TVAR_FLOAT, NULL,
								  NULL };
static THoeVarIndex idx_buildings[] = { "tree", EBO_Tree * ESSZ_max, TVAR_STRUCT, idx_obj,
										"sugar", EBO_Sugar * ESSZ_max, TVAR_STRUCT, idx_obj,
										"herbe", EBO_HerbeWoman * ESSZ_max, TVAR_STRUCT, idx_obj,
										"farm", EBO_Farm * ESSZ_max, TVAR_STRUCT, idx_obj,
										"bridge", EBO_Bridge * ESSZ_max, TVAR_STRUCT, idx_obj,
										"destilate", EBO_Destilate * ESSZ_max, TVAR_STRUCT, idx_obj,
										"troll", EBO_Troll * ESSZ_max, TVAR_STRUCT, idx_obj,
										"factory", EBO_Factory * ESSZ_max, TVAR_STRUCT, idx_obj,
										"waterhole", EBO_WaterHole * ESSZ_max, TVAR_STRUCT, idx_obj,
										"store", EBO_Store * ESSZ_max, TVAR_STRUCT, idx_obj,
										"shop", EBO_Shop * ESSZ_max, TVAR_STRUCT, idx_obj,
										"saw", EBO_Saw * ESSZ_max, TVAR_STRUCT, idx_obj,
										"coalmine", EBO_CoalMine * ESSZ_max, TVAR_STRUCT, idx_obj,
										"stonemine", EBO_StoneMine * ESSZ_max, TVAR_STRUCT, idx_obj,
										  NULL };

static THoeVarValue vv_sizzing[EBO_Max*ESSZ_max] = {0};
CVar v_sizzing("sizzing", idx_buildings, vv_sizzing, EBO_Max * ESSZ_max);

struct CustomInfo
{
	const char * str;
	int type;
};

static const CustomInfo g_info[] = {"cane",BINFO_NumSur|EBS_Cane,
							  "sugar",BINFO_NumSur|EBS_Sugar,
							  "stone",BINFO_NumSur|EBS_Stone,
							  "wood",BINFO_NumSur|EBS_Wood,
							  "coal",BINFO_NumSur|EBS_Coal,
							  "alco",BINFO_NumSur|EBS_Alco,
							  "becher", BINFO_NumSur|EBS_Becher,
							  "wood", BINFO_NumSur|EBS_Wood,
							  "herbe", BINFO_NumSur|EBS_Herbe,
							  "water", BINFO_NumSur|EBS_Water,
							  "reqstone", BINFO_ReqSur|EBS_Stone,
							  "reqwood", BINFO_ReqSur|EBS_Wood,
							  "canstone", BINFO_CanSur|EBS_Stone,
							  "canwood", BINFO_CanSur|EBS_Wood,
							  "build",BINFO_BuildProgress,
							  "prod",BINFO_Production,

							  NULL };

BecherObject::BecherObject(IHoeScene * scn) : HoeGame::Strategy::StgObject(scn)
{
	m_angle = 0.f;
	GetCtrl()->SetOrientation(0,1,0,0);
	m_infoselect.model = NULL;//(IHoeModel*)GetResMgr()->ReqResource(ID_INFO_RING);
	m_selected = false;
	m_curactive = false;
	GetCtrl()->Link(THoeSubObject::Object, &m_infoselect);
}

void BecherObject::SetAngle(float angle)
{
	m_angle = angle;
	GetCtrl()->SetOrientation(0,1,0,m_angle);
}

bool BecherObject::Save(ChunkDictWrite &w)
{
	return true;
}

bool BecherObject::Load(const ChunkDictRead &r)
{
	return true;
}

void BecherObject::SetPosition(const float x, const float y, const float h)
{
	assert(GetCtrl());
	posX = x;
	posY = y;
	GetCtrl()->SetPosition(HoeMath::Vector3(x, h, y));
}

void BecherObject::Update(const float u)
{
	if (m_infoselect.model)
	{
		HoeMath::Matrix m;
		m.RotationY(u);
		m_infoselect.pos.MultiplyLeft(m);
	}
}

void BecherObject::UpdateRing()
{
	// get params from 
	int t = (int)GetType();
	m_infoselect.pos.Scale(
		v_sizzing.GetFloat(SIZZ(t,ESSZ_ring_x)),1,v_sizzing.GetFloat(SIZZ(t,ESSZ_ring_y)));
	HoeMath::Matrix m;
	m.Translate(0,v_sizzing.GetFloat(SIZZ(t,ESSZ_ring_h)),0);
	m_infoselect.pos.Multiply(m);
}

int BecherObject::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Select:
		m_infoselect.model = (IHoeModel*)GetResMgr()->ReqResource(model_INFO_RING);
		UpdateRing();
		m_selected = true;
		break;
	case BMSG_Unselect:
		m_infoselect.model = NULL;
		m_selected = false;
		break;
	case BMSG_CursorActive:
		if (!m_selected)
		{
			UpdateRing();
			m_infoselect.model = (IHoeModel*)GetResMgr()->ReqResource(model_INFO_RING2);
		}
		m_curactive = true;
		break;
	case BMSG_CursorInactive:
		if (!m_selected)
			m_infoselect.model = NULL;
		m_curactive = false;
		break;
	};
	return 0;
}

int BecherObject::DefaultCustomInfo(const char * str)
{
	hoe_assert(str != NULL);
	const CustomInfo * info = g_info;
	while (info->str)
	{
		if (*info->str == *str && strcmp(info->str, str) == 0)
			return info->type;
		info++;
	}
	return BINFO_Custom;
}

#ifdef BECHER_EDITOR
void BecherObject::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	GameMsg(BMSG_PropertyChange, id, (void*)&pi, 1);
}
#endif



















