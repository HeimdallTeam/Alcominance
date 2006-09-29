
#include "StdAfx.h"
#include "becher.h"
#include "elements.h"
#include "id.h"

//////////////////////////////////////////////////////////////////

Tree::Tree(IHoeScene * scn) : BecherObject(scn)
{
#ifdef BECHER_EDITO
	SetOrientation(0,1,0,(rand() % 628) * 0.01);
	SetHeight((50+rand() % 100)*0.01f);
#endif
	m_infoselect.s_x = 1.f;
	m_infoselect.t_y = 3.f;
	m_infoselect.s_z = 1.f;
	m_wood = 100;
	m_height = 1.f;
	SetTypeModel(rand() % 4);
}

Tree::~Tree()
{
}

void Tree::SetHeight(float h)
{
	m_height = h;
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
	m_type = type;
	int id = model_STROM1;
	switch (type)
	{
	case 1:
		id = model_STROM2; break;
	case 2:
		id = model_STROM3; break;
	case 3:
		id = model_STROM4; break;
	default:
		id = model_STROM1; break;
	};
	SetModel((IHoeModel*)GetResMgr()->ReqResource(id));
}

#ifndef BECHER_EDITOR


#else // BECHER_OBJECT

bool Tree::Select()
{
	BecherObject::Select();
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
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_MOST));
	m_height = 0.f;
}


#ifndef BECHER_EDITOR


#else // BECHER_OBJECT

bool Bridge::Select()
{
	BecherObject::Select();
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

Addon::Addon(IHoeScene * scn, int id)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(id));
	m_height = 0.f;
}

#ifndef BECHER_EDITOR


#else // BECHER_OBJECT



#endif // BECHER_OBJECT