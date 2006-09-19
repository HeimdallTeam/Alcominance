
#include "StdAfx.h"
#include "becher.h"
#include "obj_herbe.h"

#ifndef BECHER_EDITOR
HerbeWomanStatic HerbeWoman::m_userhud;

HerbeWomanStatic::HerbeWomanStatic()
{
	m_act = NULL;
}

void HerbeWomanStatic::SetAct(HerbeWoman * act)
{
	m_act = act;
	// pripojit 
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("trtina", HoeGame::Gui::EText))->SetPtr(m_trtinainfo);
	//dynamic_cast<HoeGame::Gui::Font*>(ReqItem("cukr", HoeGame::Gui::EText))->SetPtr(m_sugarinfo);
}

void HerbeWomanStatic::Draw(IHoe2D * h2d)
{
	if (m_act)
	{
		//sprintf(m_sugarinfo,"%d cukru.", m_act->m_sugar.GetNum());
		//sprintf(m_trtinainfo,"%d trtina.", m_act->m_cane.GetNum());
		ObjectHud::Draw(h2d);
	}
}
#endif // BECHER_EDITOR

float getheight(IHoeModel *);

////////////////////////////////////////////////////////////
HerbeWoman::HerbeWoman(IHoeScene * scn) : SourceBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_BABA));
	m_infoselect.s_x = 1.4f;
	m_infoselect.t_y = 2.f;
	m_infoselect.s_z = 2.f;
	static THoeSub_Model m;
	m.model = dynamic_cast<IHoeModel*>(GetEngine()->Create("model miska"));
	m.t_x = 0.f;
	m.t_y = getheight(this->GetModel()) + 3.f;
	m.t_z = 0.f;
	m.s_x = 2.f;
	m.s_y = 4.f;
	m.s_z = 2.f;
	m.rotate = true;
	GetCtrl()->Link(THoeSubObject::Object, &m);
}

#ifndef BECHER_EDITOR

void HerbeWoman::Update(const float dtime)
{
}

bool HerbeWoman::Select()
{
	SourceBuilding::Select();
	GetLevel()->SetObjectHud(&m_userhud);
	m_userhud.SetAct(this);
	if (!IsBuildMode())
        GetLua()->func("s_herbe");
	return true;
}

bool HerbeWoman::Idiot(TJob *t)
{
	return false;
}

bool HerbeWoman::InsertSur(ESurType type, uint *s)
{
	return false;
}

bool HerbeWoman::SetToWork(Troll * t)
{
	return false;
}

void HerbeWoman::UnsetFromWork(Troll * t)
{
}

#else // BECHER_OBJECT

bool HerbeWoman::Select()
{
	SourceBuilding::Select();
	return true;
}

void HerbeWoman::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}


#endif // BECHER_OBJECT



