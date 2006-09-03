
#ifndef _BECHER_H_
#define _BECHER_H_

//#include "troll.h"
#include "console.h"
#include "utils.h"

/////////////////////////////////////////////
#ifndef BECHER_EDITOR

#include "bechapp.h"
#include "game.h"

inline BechApp * GetApp()
{
	assert(BechApp::GetApp<BechApp>());
	return BechApp::GetApp<BechApp>();
}

inline BecherGame * GetBecher()
{
	return GetApp()->GetGame();
}

inline BGConsole * GetCon()
{
	return dynamic_cast<BGConsole *>(GetApp()->GetConsole());
}

inline IHoe3DEngine * GetEngine()
{
	return GetApp()->GetEngine();
}


inline HoeGame::Lang * GetLang()
{
	return GetBecher()->GetLang();
}

inline HoeGame::LuaScript * GetLua()
{
	return GetBecher()->GetLua();
}

/*
inline BGConsole * GetGuiCon()
{
	assert(g_bcon);
	return g_bcon;
}*/

inline HoeGame::ResourceMgr * GetResMgr()
{
	return GetBecher()->GetResMgr();
}

#else // BECHER_PLUGIN

EditBaseConsole * GetCon();

inline IHoe3DEngine * GetEngine()
{
	return HoeEditor::App::Get()->GetEditor()->GetEngineView()->GetEngine();
}

inline BecherEditor * GetApp()
{
	assert(g_editor);
	return g_editor;
}

inline BecherLevel * GetLevel()
{
	return g_level;
}

HoeEditor::PropertyGrid * GetProp();


#endif // BECHER_PLUGIN

#endif // _BECHER_H_

