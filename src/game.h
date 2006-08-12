
#ifndef _BECHER_GAME_H_
#define _BECHER_GAME_H_

#include "level.h"
#include "panels.h"
#include "crr.h"

class BecherGame : public HoeGame::Scene, public HoeGame::KeyboardStdInput, public HoeGame::MouseStdInput
{
	enum MouseMode
	{

	};

	InfoPanel m_info;
	ControlPanel m_controls;
	BecherObject * m_mselect;
	HoeGame::HoeMusic_Module m_music;
	BecherLevel m_level;
	CRR m_crr;
public:
	BecherGame();
	bool Init();
	void Destroy();
	HoeGame::StrategyView * GetView() { return m_level.GetView(); }
	BecherCash * GetCash() { return m_level.GetCash(); }
	// key
	virtual void Update(float dtime);
	virtual void OnPaint();
	virtual void OnSet();
	bool LoadLevel(const char * fpath);
	void CloseLevel();
	InfoPanel * GetInfoPanel() { return &m_info; }
	ControlPanel * GetControlPanel() { return &m_controls; }
	BecherLevel * GetLevel() { return &m_level; }
	virtual void OnKeyDown(int key);
	virtual void OnMouseMove(float X, float Y);
	virtual void OnSelectObject(EObjType type, BecherObject* obj);

	virtual void OnWheel(long);
	virtual void OnLeftButtonUp();
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnRightButtonDown();
	void AddTroll(float x, float y);
	void AddBuildObject(unsigned long id, int gold, int wood, int stone);

	static int l_AddTroll(lua_State * L);
	static int l_SetBuilding(lua_State * L);
	static int l_PlaySound(lua_State * L);
	static int l_AddCash(lua_State * L);
	static int l_GetCash(lua_State * L);

	static int c_map(int argc, const char * argv[], void * param);
};

#endif // _BECHER_GAME_H_

