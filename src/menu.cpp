
#include "StdAfx.h"
#include "becher.h"
#include "menu.h"

void BecherMenuButton::OnActivate(bool active)
{
	if (active)
	{
		// play sound
		IHoeSound * s = (IHoeSound*)GetResMgr()->ReqResource(sound_CLICK);
		s->Play(false);
	}
}

BecherMenu::BecherMenu()
{
	scene = GetEngine()->CreateScene(HOETS_2D);
	scene->Set2DCallback(this);
	//font = (IHoeFont*)GetEngine()->Create("font 'resource/font.ttf' 24");
	//m_text[0] = 0;
	//m_image = NULL;
	//m_sound = NULL;
	cont = true;
}

BecherMenu::~BecherMenu()
{
	// delete scene
	//if (m_sound)
	{
	//	m_sound->Stop();
	//	m_sound->Delete();
	}
}

void BecherMenu::OnSet()
{
	Load("scripts/menu.menu");
	GetEngine()->SetActiveScene(scene);

	GetEngine()->GetInput()->RegisterMouse(IHoeInput::MT_Background, this);
	GetEngine()->GetInput()->RegisterKeyboard(this);
}

void BecherMenu::OnUnset()
{
	GetEngine()->SetActiveScene(NULL);
	GetEngine()->GetInput()->RegisterKeyboard(NULL);
	GetEngine()->GetInput()->RegisterMouse(IHoeInput::MT_Background, NULL);
}

void HOEAPI BecherMenu::_Paint(IHoe2D * h2d)
{
	/*h2d->SetRect(1,1);
	if (m_image)
		h2d->BltFast(0,1,0,1, m_image);
	h2d->SetRect(800,600);
	if (m_text[0])
		font->DrawText(535,507,0xffffffff,m_text); */
	Draw(h2d);
}

void BecherMenu::OnKeyDown(int key)
{
}

void BecherMenu::OnLeftButtonUp()
{
	Click(GetMouseX(), GetMouseY());
}

void BecherMenu::Update(float dtime)
{
}

void BecherMenu::OnMouseMove(float X, float Y)
{
	bool select = false;
	Move(X,Y,select);
}

HoeGame::Gui::Base * BecherMenu::CreateGUI(const char * type)
{
	BecherMenuButton * g = NULL;
#define IS(t) (strcmp(type,t)==0)
	if (IS("buttonusr"))
		g = new BecherMenuButton;
	else
		return Hoe2DFigure::CreateGUI(type);

	g = new (m_list.AddForNew(sizeof(BecherMenuButton))) BecherMenuButton();
	g->SetHandler(this, static_cast<HoeGame::Hoe2DControl::FUNC_CMD>(&BecherMenu::OnClick));
	return g;
}

void BecherMenu::OnClick(HoeGame::Gui::Base * sender)
{
	cont = false;
}


