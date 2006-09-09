
#include "StdAfx.h"
#include "becher.h"
#include "obj_stonemine.h"

#ifndef BECHER_EDITOR
StoneMineStatic StoneMine::m_userhud;

StoneMineStatic::StoneMineStatic()
{
	m_act = NULL;
}

void StoneMineStatic::SetAct(StoneMine * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetPtr(m_trtinainfo);
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetPtr(m_sugarinfo);
}

void StoneMineStatic::Draw(IHoe2D * h2d)
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
StoneMine::StoneMine(IHoeScene * scn) : SourceBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_STONEMINE));
}

#ifndef BECHER_EDITOR

void StoneMine::Update(const float dtime)
{
}

bool StoneMine::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_stonemine");
	return true;
}

bool StoneMine::Idiot(Job *t)
{
	return false;
}

bool StoneMine::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool StoneMine::SetToWork(Troll * t)
{
	return false;
}

void StoneMine::UnsetFromWork(Troll * t)
{
}

#else // BECHER_OBJECT

bool StoneMine::Select()
{
	SourceBuilding::Select();
	return true;
}

void StoneMine::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



