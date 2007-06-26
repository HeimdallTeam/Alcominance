
#ifndef _BECHER_APP_H_
#define _BECHER_APP_H_

class BecherGame;
//#ifdef D_SIM
//class BechApp : public HoeGame::HoeFakeApp
//#else
class BechApp : public HoeGame::HoeApp
//#endif
{
	HoeGame::Scene * scene;
	BecherGame *m_game;
public:
	BechApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "BecherGame"; }
	bool InitGame();
	virtual void OnUpdate(float dtime);
	void SetScene(HoeGame::Scene * scn);
	int RunGame();
	inline BecherGame * GetGame() { return m_game; }
};

#endif // _BECHER_APP_H_

