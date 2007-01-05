
#include "../StdAfx.h"
#include "../bechapp.h"
#include "../console.h"
#include "../becher.h"
#include "../game.h"

using namespace HoeGame;

bool Configure(int argc,char * argv[]);

int main(int argc,char * argv[])
{
	SetRootFromExe(argv[0]); 
	HOE_INSTANCE hInstance = NULL;

	BGConsole con;

	con.SetFileLogging("becher.log");

	con.Con_Print("****************************");
	con.Con_Print("* Alcominance  Simulation! *");
	con.Con_Print("****************************");

	if (!Configure(argc,argv))
		return 0;

	BechApp app(hInstance,&con);
	return app.RunGame();
}






