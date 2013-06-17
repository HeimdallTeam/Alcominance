
#ifndef _BECHER_APP_H_
#define _BECHER_APP_H_

#include <HoeResource/resmgr.h>

class BecherGame;
//#ifdef D_SIM
//class BechApp : public HoeGame::HoeFakeApp
//#else
class BechApp : public HoeGame::HoeApp, public HoeRes::XResourceMgr
//#endif
{
	HoeGame::Scene * scene;
	BecherGame *m_game;
	HoeGame::HoeEngineDLL m_engine;
public:
	BechApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "BecherGame"; }
	bool InitGame();
	virtual void OnUpdate(float dtime);
	void SetScene(HoeGame::Scene * scn);
	int RunGame();
	inline BecherGame * GetGame() { return m_game; }

	virtual HoeCore::ReadStream * GetResource(const tchar* name, const tchar** ns = NULL);
};

#endif // _BECHER_APP_H_

