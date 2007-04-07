
#include "StdAfx.h"
#include "becher.h"
#include "console.h"
#include "id.h"

ConsoleDialog::ConsoleDialog(HoeGame::Console & con)
	: HoeGame::GuiConsole(con)
{
}

bool ConsoleDialog::Load(IHoe3DEngine *eng)
{
	engine = eng;
	m_fontcolor = 0xffcf0000;

	font = (IHoeFont *)eng->Create("font 'resource/font.ttf' 12");
	if (!font)
		return false;
	background = (IHoePicture*)GetResMgr()->ReqResource(model_CONSOLE_BACK);
	if (!background)
		return false;

	return true;
}


