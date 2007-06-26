
#ifndef _BECHER_H_
#define _BECHER_H_

//#include "troll.h"
#include "console.h"
#include "utils.h"
#include "crr.h"

/////////////////////////////////////////////

inline IHoe3DEngine * GetEngine()
{
	assert(HoeGame::GetHoeEngine());
	return HoeGame::GetHoeEngine();
}

#ifndef BECHER_EDITOR

#include "bechapp.h"
#include "game.h"

inline BechApp * GetApp()
{
	//assert(BechApp::GetApp<BechApp>());
	return BechApp::GetApp<BechApp>();
}

inline BecherGame * GetBecher()
{
	return GetApp()->GetGame();
}

inline BecherLevel * GetLevel()
{
	return GetBecher()->GetLevel();
}

inline HoeGame::Console * GetCon()
{
	return dynamic_cast<HoeGame::Console *>(GetApp()->GetConsole());
}

extern HoeGame::Lang g_lang;
inline HoeGame::Lang * GetLang()
{
	return &g_lang;
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

extern BecherResources g_resmgr;
inline HoeGame::ResourceMgr * GetResMgr()
{
	return &g_resmgr;
}

#else // BECHER_PLUGIN

#include "editor/editor.h"
#include "editor/editor_map.h"

EditBaseConsole * GetCon();

inline BecherEditApp * GetApp()
{
	return dynamic_cast<BecherEditApp*>(HoeEditor::App::Get());
}

/*inline BecherEdit * GetBecher()
{
	return BecherEdit::Get();
}*/

inline BecherEdit * GetEditor()
{
	return BecherEdit::Get();
}


inline HoeGame::ResourceMgr * GetResMgr()
{
	return GetEditor()->GetResMgr();
}

inline EditorMap * GetLevel()
{
	return GetEditor()->GetActMap();
}

inline HoeEditor::PropertyGrid * GetProp()
{
	return GetEditor()->GetProp();
}


#endif // BECHER_PLUGIN

extern HoeCore::StringPool g_pool;

#endif // _BECHER_H_

