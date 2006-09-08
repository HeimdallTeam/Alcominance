
#include "StdAfx.h"
#include "becher.h"
#include "obj_coalmine.h"

#ifndef BECHER_EDITOR
CoalMineStatic CoalMine::m_userhud;

CoalMineStatic::CoalMineStatic()
{
	m_act = NULL;
}

void CoalMineStatic::SetAct(CoalMine * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Font*>(ReqItem("trtina"))->SetPtr(m_trtinainfo);
	//dynamic_cast<HoeGame::Font*>(ReqItem("cukr"))->SetPtr(m_sugarinfo);
}

void CoalMineStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		//sprintf(m_sugarinfo,"%d cukru.", m_act->m_sugar.GetNum());
		//sprintf(m_trtinainfo,"%d trtina.", m_act->m_cane.GetNum());
		ObjectHud::Draw(h2d);
	}
}
#endif // BECHER_EDITOR

////////////////////////////////////////////////////////////
CoalMine::CoalMine(IHoeScene * scn) : SourceBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_COALMINE));
}

#ifndef BECHER_EDITOR

void CoalMine::Update(const double dtime)
{
}

bool CoalMine::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_coalmine");
	return true;
}

bool CoalMine::Idiot(Job *t)
{
	return false;
}

bool CoalMine::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool CoalMine::SetToWork(Troll * t)
{
	return false;
}

void CoalMine::UnsetFromWork(Troll * t)
{
}

#else // BECHER_OBJECT

bool CoalMine::Select()
{
	SourceBuilding::Select();
	return true;
}

void CoalMine::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}

#endif // BECHER_OBJECT



