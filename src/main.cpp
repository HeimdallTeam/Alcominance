
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
	g_con = &con;
	con.Con_Print("***********************");
	con.Con_Print("*     Becher Game!    *");
	con.Con_Print("***********************");

	if (!Configure(CONF_PARAM))
		return 0;

	BechApp app(hInstance,&con);

	try {

	if (!app.Init())
	{
		app.HandleError();
		return 0;
	}

	// init game
	if (!app.InitGame())
		throw;

	const char * lev = "data/maps/cukr.bm";
#ifdef _WIN32
	if (lpStr[0] != '\0')
		lev = lpStr;
#endif

	GetBecher()->LoadLevel(lev);
	
	app.Run();
	// music destroy
	GetBecher()->Destroy();
	app.DestroyEngine();

	} catch (const char *)
	{
		app.DestroyEngine();
		app.HandleError();
	}

	app.Destroy();

	return 0;
}






