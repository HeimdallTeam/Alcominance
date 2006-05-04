
#include "StdAfx.h"
#include "becher.h"
#include "console.h"
#include "id.h"

BGConsole::BGConsole()// : HoeGame::GuiConsole()
{
}

bool BGConsole::Load(IHoe3DEngine *eng)
{
	engine = eng;
	m_fontcolor = 0xffcf0000;

	font = (IHoeFont *)eng->Create("font 'resource/font.ttf' 12");
	if (!font)
		return false;
	background = (IHoePicture*)GetResMgr()->ReqResource(ID_CONSOLE_BACK);
	if (!background)
		return false;

	return true;
}


