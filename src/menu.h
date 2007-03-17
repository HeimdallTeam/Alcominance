
#ifndef _BECHERGAME_MENU_H_
#define _BECHERGAME_MENU_H_

#include "utils.h"

class BecherMenuButton : public HoeGame::Gui::ButtonUsr
{
public:
	virtual void OnActivate(bool active);
};

class BecherMenu : public HoeGame::Scene, public XHoe2DCallback, 
	public HoeGame::Hoe2DFigure,
	public HoeGame::KeyboardStdInput,
	public HoeGame::MouseStdInput
{
protected:
	IHoeScene * scene;
public:
	bool cont;

	BecherMenu();
	~BecherMenu();
	virtual void OnSet();
	virtual void OnUnset();
	virtual void HOEAPI _Paint(IHoe2D * h2d);
	virtual void Update(float dtime);
	virtual void OnKeyDown(int key);
	virtual void OnLeftButtonUp();
	virtual void OnMouseMove(float X, float Y);
	void Run();
	virtual HoeGame::Gui::Base * CreateGUI(const char * type);

	// events
	void OnClick(HoeGame::Gui::Base * sender);
};

#endif //_BECHERGAME_MENU_H_

