
#ifndef _BECHER_APP_H_
#define _BECHER_APP_H_

class BechApp : public HoeGame::HoeApp
{
	HoeGame::Scene * scene;
public:
	BechApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "BecherGame"; }
	bool InitGame();
	virtual void OnUpdate(float dtime);
	void SetScene(HoeGame::Scene * scn);
};

#endif // _BECHER_APP_H_

