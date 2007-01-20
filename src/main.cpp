
#include "StdAfx.h"
#include "bechapp.h"
#include "console.h"
#include "becher.h"
#include "game.h"

using namespace HoeGame;

#ifdef _WIN32
bool Configure(HINSTANCE hInstance, LPSTR lpStr);
#define CONF_PARAM hInstance, lpStr

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR lpStr,int iCmdShow)
{
	// tests
	HoeGame::Mem::MemTest();

	SetRootFromInstance(hInstance); 
#else
bool Configure(int argc,char * argv[]);
#define CONF_PARAM argc, argv

int main(int argc,char * argv[])
{
	SetRootFromExe(argv[0]); 
	HOE_INSTANCE hInstance = NULL;
#endif

	BGConsole con;

	con.SetFileLogging("becher.log");

	con.Con_Print("****************************");
	con.Con_Print("*     Alcominance Game!    *");
	con.Con_Print("****************************");

	if (!Configure(CONF_PARAM))
		return 0;

	BechApp app(hInstance,&con);
	return app.RunGame();
}






