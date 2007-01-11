
#include "StdAfx.h"
#include "becher.h"
#include "object.h"

struct CustomInfo
{
	const char * str;
	int type;
};

static const CustomInfo g_info[] = {"cane",BINFO_NumCane,
							  "sugar",BINFO_NumSugar,
							  "stone",BINFO_NumStone,
							  "wood",BINFO_NumWood,
							  "coal",BINFO_NumCoal,
							  "reqstone", BINFO_ReqStone,
							  "reqwood", BINFO_ReqWood,
							  "build",BINFO_BuildProgress,
							  NULL };

BecherObject::BecherObject(IHoeScene * scn) : HoeGame::Strategy::StgObject(scn)
{
	m_angle = 0.f;
	GetCtrl()->SetOrientation(0,1,0,0);
	m_infoselect.model = NULL;//(IHoeModel*)GetResMgr()->ReqResource(ID_INFO_RING);
	SetRingParam(2.f,2.f,3.f);
	GetCtrl()->Link(THoeSubObject::Object, &m_infoselect);
}

void BecherObject::SetAngle(float angle)
{
	m_angle = angle;
	GetCtrl()->SetOrientation(0,1,0,m_angle);
}

bool BecherObject::Save(BecherGameSave &w)
{
	// ulozit 10 x dword, jako reserved
	w.WriteReservedWords(10);
	return true;
}

bool BecherObject::Load(BecherGameLoad &r)
{
	r.ReadReservedWords(10);
	return true;
}

void BecherObject::SetPosition(const float x, const float y, const float h)
{
	assert(GetCtrl());
	posX = x;
	posY = y;
	GetCtrl()->SetPosition(HoeMath::Vector3(x, h, y));
}

bool BecherObject::Select()
{
	m_infoselect.model = (IHoeModel*)GetResMgr()->ReqResource(model_INFO_RING);
	return true;
}

void BecherObject::Unselect()
{
	m_infoselect.model = NULL;
}

void BecherObject::SetCurActive(bool active)
{
	m_infoselect.model = active ? (IHoeModel*)GetResMgr()->ReqResource(model_INFO_RING2):NULL;
	
}

void BecherObject::SetRingParam(float sx, float sy, float h)
{
	m_infoselect.pos.Scale(sx,1,sy);
	HoeMath::Matrix m;
	m.Translate(0,h,0);
	m_infoselect.pos.Multiply(m);
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

int BecherObject::GameMsg(int msg, int par1, void * par2, uint npar2)
{
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

















