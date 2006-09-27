
#include "StdAfx.h"
#include "becher.h"
#include "dialogs.h"

using namespace HoeGame::Gui;

bool DialogBuild::SetTip(const char * str, IHoeFont * fnt, const char * tip)
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
	return true;
}

bool DialogBuild::Load(const char * fname)
{
	if (!HoeGame::Hoe2DFigure::Load(fname))
		return false;

	// set tt
	IHoeFont * fnt = (IHoeFont*)GetResMgr()->ReqResource(font_TOOLTIPS);
	SetTip("drevorubec", fnt, GetLang()->Get(1001));
	SetTip("duluhli", fnt, GetLang()->Get(1002));
	SetTip("dulkamen", fnt, GetLang()->Get(1003));
	SetTip("sklad", fnt, GetLang()->Get(1004));
	SetTip("studna", fnt, GetLang()->Get(1005));
	SetTip("farma", fnt, GetLang()->Get(1006));
	SetTip("obchod", fnt, GetLang()->Get(1007));
	SetTip("cukrovar", fnt, GetLang()->Get(1008));
	SetTip("lihovar", fnt, GetLang()->Get(1009));
	SetTip("tovarna", fnt, GetLang()->Get(1010));
	// 
	return true;
}



