
#ifndef _BECHER_H_
#define _BECHER_H_

//#include "troll.h"
#include "console.h"
#include "utils.h"

extern HoeGame::ResourceMgr * g_resmgr;
inline HoeGame::ResourceMgr * GetResMgr()
{
	assert(g_resmgr);
	return g_resmgr;
}

/////////////////////////////////////////////
#ifndef BECHER_EDITOR

extern BGConsole * g_con;
inline BGConsole * GetCon()
{
	assert(g_con);
	return g_con;
}

extern IHoe3DEngine * g_engine;
inline IHoe3DEngine * GetEngine()
{
	assert(g_engine);
	return g_engine;
}

extern class BechApp * g_app;
inline BechApp * GetApp()
{
	assert(g_app);
	return g_app;
}

extern HoeGame::Lang * g_lang;
inline HoeGame::Lang * GetLang()
{
	assert(g_lang);
	return g_lang;
}

extern class BecherGame * g_becher;
inline BecherGame * GetBecher()
{
	assert(g_becher);
	return g_becher;
}

extern HoeGame::LuaScript * g_lua;
inline HoeGame::LuaScript * GetLua()
{
	assert(g_lua);
	return g_lua;
}

/*extern class BGConsole * g_bcon;
inline BGConsole * GetGuiCon()
{
	assert(g_bcon);
	return g_bcon;
}*/

bool InitBecher(BechApp * application);

#else // BECHER_PLUGIN

EditBaseConsole * GetCon();

inline IHoe3DEngine * GetEngine()
{
	return HoeEditor::App::Get()->GetEditor()->GetEngineView()->GetEngine();
}

extern class BecherEditor * g_editor;
inline BecherEditor * GetApp()
{
	assert(g_editor);
	return g_editor;
}

extern class BecherLevel * g_level;
inline BecherLevel * GetLevel()
{
	return g_level;
}

HoeEditor::PropertyGrid * GetProp();


#endif // BECHER_PLUGIN

#endif // _BECHER_H_

