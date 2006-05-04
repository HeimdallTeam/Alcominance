
#include "../StdAfx.h"
#include "../becher.h"
//#include "console.h"

//////////////////////////////////////////////
class BecherEditor * g_app = NULL;
HoeGame::Lang * g_lang = NULL;
IHoe3DEngine * g_engine = NULL;
HoeGame::ResourceMgr * g_resmgr = NULL;
HoeGame::LuaScript * g_lua = NULL;
//HoeGame::Console * g_con = NULL;
//Becher * g_becher = NULL;
//BGConsole * g_con = NULL;

//void BGConsole::Con_Print(const char *)
//{
//}

/*bool InitBecherEdit(BecherEditor * be)
{
	g_app = be;

	g_con = new BGConsole();

	g_engine = HoeExtern::IEditor::Get()->GetEngine();

	//HoeGetInfo(GetEngine())->LoadFont("resource/font.ttf",12); 
	
	/*g_lang = new HoeGame::Lang();
	if (!GetLang()->Load("resource/czech.lng"))
	{
		GetCon()->Printf("Failed open lang file 'resource/czech.lng'");
		return false;
	}*/

	g_resmgr = new HoeGame::ResourceMgr();

	// init lua
	g_lua = new HoeGame::LuaScript(g_con);
	if (!GetLua()->Init())
		return false;

	if (!GetLua()->Connect(GetEngine()))
		return false;
	if (!GetLua()->Connect(GetResMgr()))
		return false;
	//if (!GetLua()->Connect(GetLang()))
	//	return false;

	// load skript
	if (!GetLua()->Load("init.lua"))
		return false;

	return true;
}




