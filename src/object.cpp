
#include "StdAfx.h"
#include "becher.h"
#include "object.h"

BecherObject::BecherObject(IHoeScene * scn) : HoeGame::Strategy::StgObject(scn)
{
	m_angle = 0.f;
	GetCtrl()->SetOrientation(0,1,0,0);
	m_infoselect.model = NULL;//(IHoeModel*)GetResMgr()->ReqResource(ID_INFO_RING);
	m_infoselect.t_x = 0.f;
	m_infoselect.t_y = 3.f;
	m_infoselect.t_z = 0.f;
	m_infoselect.s_x = 2.f;
	m_infoselect.s_y = 1.f;
	m_infoselect.s_z = 2.f;
	m_infoselect.rotate = true;
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
	GetCtrl()->SetPosition(x, h, y);
}

void BecherObject::SetProperty(const char * prop, const char * value)
{

}

bool BecherObject::Select()
{
	m_infoselect.model = (IHoeModel*)GetResMgr()->ReqResource(ID_INFO_RING);
	return true;
}

void BecherObject::Unselect()
{
	m_infoselect.model = NULL;
}

void BecherObject::SetCurActive(bool active)
{
	m_infoselect.model = active ? (IHoeModel*)GetResMgr()->ReqResource(ID_INFO_RING2):NULL;
	
}











