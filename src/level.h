
#ifndef _BECHERGAME_LEVEL_H_
#define _BECHERGAME_LEVEL_H_

#include "map.h"
#include "buildings.h"
#include "panels.h"
#include "crr.h"
#include "dialogs.h"
#include "ministry.h"
#include "minimap.h"

#define ID_BSAVE ('b' | 's' << 8 | 'a' << 16 | 'v' << 24)

class Store;
class BaseDialog;
struct TrollPath;

class BecherTime : public HoeGame::Timer
{
public:
	bool Save(HoeCore::WriteStream &w);
	bool Load(int ver, HoeFileReader &r);
};

class BecherCash 
{
	int m_limit;
	int m_cash;
public:
	BecherCash();
	void Set(int cash, int limit);
	bool Save(HoeCore::WriteStream &w);
	bool Load(int ver, HoeFileReader &r);
	bool Add(int m);
	int GetValue() { return m_cash; }
	int GetLimitCash() { return m_cash - m_limit; }
	void Link(HoeGame::Gui::DigiCounter * cnt) { cnt->Set(&m_cash); }
};

// objekt pro hleedani cesty atd..
class BecherLand
{
	HoeCore::WordTileMap m_tiles;
	HoeGame::Land m_land;
public:
	bool Create(BecherMap * map);
	bool FindPath(const HoeMath::Vector2 &from, const HoeMath::Vector2 &to, TrollPath &path);
};

class BecherLevel : public BecherMap, 
	public HoeGame::Scene, 
	public XHoe2DCallback, 
	public HoeGame::KeyboardStdInput, 
	public HoeGame::MouseStdInput
{
protected:
	char m_filename[512];
	// build object
	BecherBuilding * m_build;
	int m_buildgold;
	int m_buildstone;
	int m_buildwood;
	//BecherObject * m_buildinit;
	BecherTime m_timer;
	BecherCash m_cash; 
	MinistryOfJobs m_mjobs;
	WaterMiniMap m_watermap;
	CoalMiniMap m_coalmap;
	StoneMiniMap m_stonemap;
	TerrainMiniMap m_termap;
	BecherLand m_land;

	BaseDialog * m_dlg;
	BecherObject * m_select;
	CRR m_crr;
	BecherObject * m_mselect;
	HUD m_hud;

	// dlgs
	DialogBuild m_builddlg;
	//
public:
	BecherLevel();
	virtual void OnSet();
	void SelectObject(BecherObject* obj);
	BecherObject * GetSelectedObject() { return m_select; }
	void SetDialog(BaseDialog * dlg);

	bool IsPaused() { return m_timer.IsPaused(); }
	bool IsBuild() { return m_build != NULL; }
	virtual void Update(float time);
	virtual void HOEAPI _Paint(IHoe2D * h2d);

	void MouseUpdate(float x, float y);
	void MouseLeave();
	void MouseLeftDown(float x, float y);
	virtual void OnKeyDown(int key);
	virtual void OnMouseMove(float X, float Y);

	virtual void OnWheel(long);
	virtual void OnLeftButtonUp();
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnRightButtonDown();

	void SetBuildObject(BecherBuilding * bo, int gold, int wood, int stone);
	void LeaveBuild();
	void Start();
	bool SaveGame(const char * path);
	bool LoadGame(const char * path);

	BecherCash * GetCash() { return &m_cash; }
	MinistryOfJobs * GetMJobs() { return &m_mjobs; }

	HUD * GetPanel() { return &m_hud; }
	Troll * CreateTroll();
	void AddBuildObject(unsigned long id, int gold, int wood, int stone);

	void ShowWaterMap()
	{
		m_hud.SetMap(&m_watermap);
	}
	void ShowCoalMap()
	{
		m_hud.SetMap(&m_coalmap);
	}
	void ShowStoneMap()
	{
		m_hud.SetMap(&m_stonemap);
	}
	void HideMap()
	{
		m_hud.SetMap(NULL);
	}
	// hledani cest
	// vychytavka: vrati to cestu pokud byla nalezena, ale dokaze vratit 
	// i virtualni cestu, ktera prinut, zeptat se znova
	bool FindPath(const HoeMath::Vector2 &from, const HoeMath::Vector2 &to, TrollPath &path);
	static int l_SetBuilding(lua_State * L);
	static int l_AddCash(lua_State * L);
	static int l_GetCash(lua_State * L);
	static int l_Camera(lua_State * L);

};


#endif // _BECHERGAME_LEVEL_H

