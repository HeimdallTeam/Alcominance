
#include "StdAfx.h"
#include "becher.h"
#include "elements.h"
#include "id.h"
#include "game.h"

//////////////////////////////////////////////////////////////////

Tree::Tree(IHoeScene * scn) : BecherObject(scn)
{
#ifdef BECHER_PLUGIN
	SetOrientation(0,1,0,(rand() % 628) * 0.01);
	SetHeight((50+rand() % 100)*0.01f);
#endif
	m_infoselect.s_x = 1.f;
	m_infoselect.t_y = 3.f;
	m_infoselect.s_z = 1.f;
	param.wood = 100;
	param.height = 1.f;
	SetTypeModel(1);
}

Tree::~Tree()
{
}

void Tree::SetHeight(float h)
{
	param.height = h;
	if (h != 1.f)
	{
		GetCtrl()->SetScale(1,h,1);
		GetCtrl()->SetFlags(HOF_SCALED);
	}
	else
		GetCtrl()->UnsetFlags(HOF_SCALED);
}

void Tree::SetTypeModel(int type)
{
	param.type = type;
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_STROM+type));
}

#ifndef BECHER_EDITOR


#else // BECHER_OBJECT

bool Tree::Select()
{
	FactoryBuilding::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Position"));
	GetProp()->AppendFloat(6, _("Height"), 1);
	GetProp()->End();	
	return true;
}

void Tree::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 6:
		SetHeight((float)pi.GetFloat());
		break;
	};
}

#endif // BECHER_OBJECT

////////////////////////////////////////////////////////////
Bridge::Bridge(IHoeScene * scn) : BecherObject(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_MOST));
	m_height = 0.f;
}


#ifndef BECHER_EDITOR


#else // BECHER_OBJECT

bool Bridge::Select()
{
	FactoryBuilding::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Position"));
	GetProp()->AppendFloat(6, _("Height"), m_height);
	GetProp()->End();	
	return true;
}

void Bridge::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 6:
		m_height = (float)pi.GetFloat();
		SetPosition(GetPosX(), GetPosY(), m_height);
		break;
	};
}

#endif // BECHER_OBJECT

////////////////////////////////////////////////////////////
WaterHole::WaterHole(IHoeScene * scn) : BecherObject(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_WATERHOLE));
	m_infoselect.s_x = 1.f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 1.f;
}

#ifndef BECHER_EDITOR

bool WaterHole::Select()
{
	BecherObject::Select();
	GetLua()->func("s_studna");
	return true;
}

#endif


