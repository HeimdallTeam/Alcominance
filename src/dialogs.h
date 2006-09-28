
#ifndef _DIALOGS_H_
#define _DIALOGS_H_

class BaseDialog : public HoeGame::Hoe2DFigure
{
public:
	virtual void OnShow() {};
	virtual void OnHide() {};
};

/*
budovy na staveni
drevorubec
dul uhli
dul kamen
sklad

studna
farma
baba
obchod

cukrovar
lihovar
tovarna


*/

class DialogBuild : public BaseDialog
{
protected:
	bool SetButton(const char * str, int id, IHoeFont * fnt, const char * tip);
public:
	bool Load(const char * fname);
	void OnClick(HoeGame::Gui::Base * sender);
	void OnExit(HoeGame::Gui::Base * sender);
};

#endif // _DIALOGS_H_

