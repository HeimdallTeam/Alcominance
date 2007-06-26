
#include "StdAfx.h"
#include "becher.h"
#include "dialogs.h"

using namespace HoeGame::Gui;

bool DialogBuild::SetButton(const char * str, int id, IHoeFont * fnt, const char * tip)
{
	Item * item = GetItem(str, HoeGame::Gui::EButton);
	if (item==NULL)
		return false;
	Button * b = dynamic_cast<Button*>(item);
	if (b==NULL)
		return false;
	if (fnt)
		b->SetToolTipFont(fnt);
	b->SetToolTip(tip);
	b->SetHandler(this, static_cast<HoeGame::Hoe2DControl::FUNC_CMD>(&DialogBuild::OnClick));
	b->SetID(id);
	return true; 
}

bool DialogBuild::Load(const char * fname)
{
	if (!HoeGame::Hoe2DFigure::Load(fname))
		return false;

	// set tt
	IHoeFont * fnt = (IHoeFont*)GetResMgr()->ReqResource(font_TOOLTIPS);
	SetButton("drevorubec", EBO_Saw, fnt, GetLang()->Get(1001));
	SetButton("duluhli", EBO_CoalMine, fnt, GetLang()->Get(1002));
	SetButton("dulkamen", EBO_StoneMine, fnt, GetLang()->Get(1003));
	SetButton("sklad", EBO_Store, fnt, GetLang()->Get(1004));
	SetButton("studna", EBO_WaterHole, fnt, GetLang()->Get(1005));
	SetButton("farma", EBO_Farm, fnt, GetLang()->Get(1006));
	SetButton("obchod", EBO_Shop, fnt, GetLang()->Get(1007));
	SetButton("cukrovar", EBO_Sugar, fnt, GetLang()->Get(1008));
	SetButton("lihovar", EBO_Destilate, fnt, GetLang()->Get(1009));
	SetButton("tovarna", EBO_Factory, fnt, GetLang()->Get(1010));

	Item * item = GetItem("exit", HoeGame::Gui::EButton);
	if (item==NULL)
		return false;
	Button * b = dynamic_cast<Button*>(item);
	if (b==NULL)
		return false;
	b->SetHandler(this, static_cast<HoeGame::Hoe2DControl::FUNC_CMD>(&DialogBuild::OnExit));
	// 
	return true;
}

void DialogBuild::OnClick(HoeGame::Gui::Base * sender)
{
	// click
	//GetLevel()->SetDialog(NULL);
	switch (dynamic_cast<HoeGame::Gui::Button*>(sender)->GetID())
	{
	case EBO_Sugar:
		GetLua()->func("c_sugar");
		break;
	case EBO_WaterHole:
		GetLua()->func("c_stavstudna");
		// zobrazit minimapu
		if (GetLevel()->IsBuild())
			GetLevel()->ShowWaterMap();
		break;
	case EBO_CoalMine:
		GetLua()->func("c_stavuhli");
		// zobrazit minimapu
		if (GetLevel()->IsBuild())
			GetLevel()->ShowCoalMap();
		break;
	case EBO_StoneMine:
		GetLua()->func("c_stavkamen");
		// zobrazit minimapu
		if (GetLevel()->IsBuild())
			GetLevel()->ShowStoneMap();
		break;
	case EBO_Farm:
		GetLua()->func("c_farm");
		break;
	case EBO_Factory:
		GetLua()->func("c_stavtovarna");
		break;
	case EBO_Store:
		GetLua()->func("c_stavsklad");
		break;
	case EBO_Destilate:
		GetLua()->func("c_stavlihovar");
		break;
	case EBO_Shop:
		GetLua()->func("c_stavobchod");
		break;
	case EBO_Saw:
		GetLua()->func("c_stavdrevorubec");
		break;
	default:
		assert(!"Button not implemented.");
		break;
	};
	GetLevel()->SetDialog(NULL);
}

void DialogBuild::OnExit(HoeGame::Gui::Base * sender)
{
	GetLevel()->SetDialog(NULL);
}


