
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
	m_infoselect.pos.Translate(0,3,0);
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
		GetCtrl()->SetScale(HoeMath::Vector3(1,h,1));
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

bool Tree::Save(ChunkDictWrite &w)
{
	BecherObject::Save(w);
	w.Key("model", m_type);
	w.Key("height", m_height);
	//w.Write<float>(m_height);
	return true;
}

bool Tree::Load(const ChunkDictRead &r)
{
	BecherObject::Load(r);
	SetTypeModel(r.KeyInt("model", m_type));
	SetHeight(r.KeyFloat("height", m_height));
	//m_height = r.Read<float>();
	//SetPosition(GetPosX(), GetPosY(), m_height);
	return true;
}

#ifndef BECHER_EDITOR
int Tree::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{

	}
	return BecherObject::GameMsg(msg, par1, par2, npar2);

}

#else // BECHER_OBJECT

int Tree::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Select:
		//BecherObject::Select();
		GetProp()->Begin(this);
		GetProp()->AppendCategory(_("Position"));
		GetProp()->AppendFloat(6, _("Height"), 1);
		GetProp()->End();
		break;
	case BMSG_PropertyChange:
		switch (par1)
		{
		case 6:
			SetHeight((float)reinterpret_cast<const HoeEditor::PropItem*>(par2)->GetFloat());
			return 0;
		};
		break;
	}
	return BecherObject::GameMsg(msg, par1, par2, npar2);

}

#endif // BECHER_OBJECT

////////////////////////////////////////////////////////////
Bridge::Bridge(IHoeScene * scn) : BecherObject(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_MOST));
	m_height = 0.f;
}

bool Bridge::Save(ChunkDictWrite &w)
{
	BecherObject::Save(w);
	w.Key("height", m_height);
	//w.Write<float>(m_height);
	return true;
}

bool Bridge::Load(const ChunkDictRead &r)
{
	BecherObject::Load(r);
	m_height = r.KeyFloat("height",0.f);
	SetPosition(GetPosX(), GetPosY(), m_height);
	return true;
}


#ifndef BECHER_EDITOR
int Bridge::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{

	}
	return BecherObject::GameMsg(msg, par1, par2, npar2);

}

#else // BECHER_OBJECT

int Bridge::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Select:
		//BecherObject::Select();
		GetProp()->Begin(this);
		GetProp()->AppendCategory(_("Position"));
		GetProp()->AppendFloat(6, _("Height"), m_height);
		GetProp()->End();
		break;
	case BMSG_PropertyChange:
		switch (par1)
		{
		case 6:
			m_height = (float)reinterpret_cast<const HoeEditor::PropItem*>(par2)->GetFloat();
			SetPosition(GetPosX(), GetPosY(), m_height);
			return 0;
		};
		break;
	}
	return BecherObject::GameMsg(msg, par1, par2, npar2);

}

#endif // BECHER_OBJECT

Addon::Addon(IHoeScene * scn, EObjType type)
#ifdef BECHER_EDITOR
 : BecherObject(scn), m_type(type), m_scale(1.f)
#else
: HoeGame::BaseObject(scn)
#endif
{
	IHoeModel * m = NULL;
	switch (type)
	{
	case EBAO_Stones1:
		m = (IHoeModel*)GetResMgr()->ReqResource(model_addon_STONE1);
        break;
    case EBAO_Stones2:
		m = (IHoeModel*)GetResMgr()->ReqResource(model_addon_STONE2);
        break;
    case EBAO_Anthill:
		m = (IHoeModel*)GetResMgr()->ReqResource(model_addon_ANTHILL);
        break;
    case EBAO_Row:
		m = (IHoeModel*)GetResMgr()->ReqResource(model_addon_ROW);
        break;
	};

	if (m)
		SetModel(m);
	//m_height = 0.f;
}

#ifdef BECHER_EDITOR

int Addon::GameMsg(int msg, int par1, void * par2, uint npar2)
{
	switch (msg)
	{
	case BMSG_Select:
		//BecherObject::Select();
		GetProp()->Begin(this);
		GetProp()->AppendCategory(_("Position"));
		GetProp()->AppendFloat(1, _("Scale"), m_scale);
		GetProp()->End();
		break;
	case BMSG_PropertyChange:
		switch (par1)
		{
		case 1:
			SetScale((float)reinterpret_cast<const HoeEditor::PropItem*>(par2)->GetFloat());
			return 0;
		};
		break;
	}
	return BecherObject::GameMsg(msg, par1, par2, npar2);
}

void Addon::SetScale(float s)
{
	m_scale = s;
	GetCtrl()->SetScale(HoeMath::Vector3(s,s,s));
}

#else
void Addon::SetScale(float s)
{
	GetCtrl()->SetScale(HoeMath::Vector3(s,s,s));
}

#endif // BECHER_OBJECT

AddonSmoke::AddonSmoke(IHoeScene *scn) : Addon(scn, EBAO_Smoke)
{
#ifdef BECHER_EDITOR
	static IHoeModel * mod = NULL;
	if (mod == NULL)
	{
		mod = (IHoeModel *)GetEngine()->Create("generate model box 1");
	}
	SetModel(mod);
#else
	SetModel((IHoeModel *)GetEngine()->Create("dummy model"));
#endif
	static THoeSub_Particle part;
	part.emitor = (IHoeParticleEmitor*)GetEngine()->Create("particle");
	part.pos.Set(0, 0, 0);
	GetCtrl()->Link(THoeSubObject::Particle, &part);
	part.emitor->Start();
}
