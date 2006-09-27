
#ifndef _DIALOGS_H_
#define _DIALOGS_H_

class BaseDialog : public HoeGame::Hoe2DFigure
{
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
	bool SetTip(const char * str, IHoeFont * fnt, const char * tip);
public:
	bool Load(const char * fname);

};

#endif // _DIALOGS_H_

