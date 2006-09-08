
#include "StdAfx.h"
#include "becher.h"
#include "obj_waterhole.h"

#ifndef BECHER_EDITOR
WaterHoleStatic WaterHole::m_userhud;

WaterHoleStatic::WaterHoleStatic()
{
	m_act = NULL;
}

void WaterHoleStatic::SetAct(WaterHole * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Font*>(ReqItem("trtina"))->SetPtr(m_trtinainfo);
	//dynamic_cast<HoeGame::Font*>(ReqItem("cukr"))->SetPtr(m_sugarinfo);
}

void WaterHoleStatic::Draw(IHoe2D * h2d)
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
WaterHole::WaterHole(IHoeScene * scn) : SourceBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_WATERHOLE));
	m_infoselect.s_x = 1.f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 1.f;}

#ifndef BECHER_EDITOR

void WaterHole::Update(const double dtime)
{
}

bool WaterHole::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_studna");
	return true;
}

bool WaterHole::Idiot(Job *t)
{
	return false;
}

bool WaterHole::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool WaterHole::SetToWork(Troll * t)
{
	return false;
}

void WaterHole::UnsetFromWork(Troll * t)
{
}

#else // BECHER_OBJECT

bool WaterHole::Select()
{
	SourceBuilding::Select();
	return true;
}

void WaterHole::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



