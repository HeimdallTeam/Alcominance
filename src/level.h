
#ifndef _BECHERGAME_LEVEL_H_
#define _BECHERGAME_LEVEL_H_

#include "map.h"
#include "buildings.h"
#include "panels.h"
#include "crr.h"

#define ID_BSAVE ('b' | 's' << 8 | 'a' << 16 | 'v' << 24)

class Store;



class BecherTime : public HoeGame::Timer
{
public:
	bool Save(HoeFileWriter &w);
	bool Load(int ver, HoeFileReader &r);
};

class BecherCash 
{
	int m_limit;
	int m_cash;
public:
	BecherCash();
	void Set(int cash, int limit);
	bool Save(HoeFileWriter &w);
	bool Load(int ver, HoeFileReader &r);
	bool Add(int m) { if (GetLimitCash() < -m) return false; m_cash += m; return true; };
	int GetValue() { return m_cash; }
	int GetLimitCash() { return m_cash - m_limit; }
};

class BecherLevel : public BecherMap, public HoeGame::Scene, public XHoe2DCallback, public HoeGame::KeyboardStdInput, public HoeGame::MouseStdInput
{
protected:
	char m_filename[512];
	// build object
	BecherObject * m_build;
	int m_buildgold;
	int m_buildstone;
	int m_buildwood;
	BecherObject * m_buildinit;
	BecherTime m_timer;
	BecherCash m_cash; 
	BecherObject * m_select;
	CRR m_crr;
	BecherObject * m_mselect;
	HUD m_hud;
	//
	bool LoadGame(BecherGameLoad &r);

public:
	BecherLevel();
	virtual void OnSet();
	void SelectObject(BecherObject* obj);
	BecherObject * GetSelectedObject() { return m_select; }
	bool IsPaused() { return m_timer.IsPaused(); }
	virtual void Update(float time);
	virtual void HOEAPI _Paint(IHoe2D * h2d);

	void MouseUpdate(float x, float y);
	void MouseLeave();
	void MouseLeftDown(float x, float y);
	virtual void OnKeyDown(int key);
	virtual void OnMouseMove(float X, float Y);
	virtual void OnSelectObject(EObjType type, BecherObject* obj);

	virtual void OnWheel(long);
	virtual void OnLeftButtonUp();
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnRightButtonDown();

	void SetBuildObject(BecherObject * bo, int gold, int wood, int stone);
	void Start();
	bool SaveGame(const char * path);
	bool LoadGame(const char * path);

	BecherCash * GetCash() { return &m_cash; }

	HUD * GetPanel() { return &m_hud; }
	void AddTroll(float x, float y);
	void AddBuildObject(unsigned long id, int gold, int wood, int stone);

	static int l_AddTroll(lua_State * L);
	static int l_SetBuilding(lua_State * L);
	static int l_AddCash(lua_State * L);
	static int l_GetCash(lua_State * L);

};


#endif // _BECHERGAME_LEVEL_H

