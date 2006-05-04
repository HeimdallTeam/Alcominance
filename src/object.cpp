
#include "StdAfx.h"
#include "becher.h"
#include "object.h"

BecherObject::BecherObject(IHoeScene * scn) : HoeGame::Strategy::StgObject(scn)
{
	m_angle = 0.f;
	m_info = 0;
	GetCtrl()->SetOrientation(0,1,0,0);
}

void BecherObject::SetAngle(float angle)
{
	m_angle = angle;
	GetCtrl()->SetOrientation(0,1,0,m_angle);
}

bool BecherObject::Save(HoeFileWriter &w)
{
	return true;
}

bool BecherObject::Load(int ver, HoeFileReader &r)
{
	return true;
}

void BecherObject::SetPosition(const float x, const float y)
{
	assert(GetCtrl());
	posX = x;
	posY = y;
	GetCtrl()->SetPosition(x, 0, y);
}

void BecherObject::SetProperty(const char * prop, const char * value)
{

}










