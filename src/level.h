
#ifndef _BECHERGAME_LEVEL_H_
#define _BECHERGAME_LEVEL_H_

#include "map.h"
#include "buildings.h"

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
	void Paint(IHoe2D * d2);
};

class BecherLevel : public BecherMap
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

	//
	bool LoadGame(BecherMapLoader &r);

public:
	BecherLevel();
	void SelectObject(BecherObject* obj);
	BecherObject * GetSelectedObject() { return m_select; }
	bool IsPaused() { return m_timer.IsPaused(); }
	void Update(float time);
	void MouseUpdate(float x, float y);
	void MouseLeave();
	void MouseLeftDown(float x, float y);
	void SetBuildObject(BecherObject * bo, int gold, int wood, int stone);
	void Start();
	bool SaveGame(const char * path);
	bool LoadGame(const char * path);

	BecherCash * GetCash() { return &m_cash; }

	// najde nejidealnejsi sklad 
	Store * FindBestStore();

};


#endif // _BECHERGAME_LEVEL_H

