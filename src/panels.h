
#ifndef _PANELS_H_
#define _PANELS_H_

class MiniMap;

class ObjectHud : public HoeGame::Hoe2DFigure
{
public:

};

class BecherButton : public HoeGame::Gui::Button
{
protected:
	char tooltip[1024];
	char script[512];
public:
	virtual void OnClick();
	void SetButt(int idres, const char * func, const char * tooltip);
};

class HUD : public HoeGame::Hoe2DFigure
{
protected:
	HoeGame::Gui::InfoPanel * m_info;
	BecherButton * m_butt[16];
	int m_num;
	MiniMap * m_map;
public:
	HUD();
	virtual HoeGame::Gui::Base * CreateGUI(const char * type);
	void AddButton(int idres, const char * func, const char * tooltip);
	void ShowReset();
	void SetMap(MiniMap * map) { m_map = map; }
	MiniMap * GetActMap() { return m_map; }
	virtual void Draw(IHoe2D * hoe2d);
	/** zpracovani pohybu mysi, pokud se ma zpracovano,vraci true */
	//bool MouseMove(float X, float Y);
	bool Load(const char * fname);
	HoeGame::Gui::InfoPanel * GetInfo() { return m_info; }

	LUA_FUNCTION(l_ClearButtons);
	LUA_FUNCTION(l_AddButton);
	LUA_FUNCTION(l_info);

};

/*#define CPPS_SCRIPT_LENGTH	32
#define CP_MAXNUMSPRITES	30
#define CP_SPRITESINLIST	30
#define CP_NUMLIST			30


class ControlPanel : public Panel
{
	// for test
	struct PanelSprite
	{
		int id;
		IHoePicture * pic;
		HOE_RECT rect;
		const char * tooltip;
		char script[CPPS_SCRIPT_LENGTH];

		void CreateRect(int nx,int ny,int a);
		// klaves zkratka
	};

	struct PanelList
	{
		int id;
		PanelSprite * sprites[30];
		int numsprites;
	};

	// last source
	iHoePicture * m_source;
	int m_src_nx;
	int m_src_ny;

	// sprites
	PanelSprite m_sprites[CP_MAXNUMSPRITES];
	int m_nsprites;

	PanelList m_lists[CP_NUMLIST];
	int m_nlists;

	const char * m_tooltip;
	iHoeFont * m_tooltip_font;

	PanelSprite * GetSprite(float x, float y); 
	bool AddSpriteToList(PanelList * pl, int id);
public:
	bool AddToList(int idlist, int id);
	ControlPanel();
	~ControlPanel();
	
	bool Click(float,float);
	bool Move(float,float);
	
	int Draw(iHoe2D * hoe2d);

	bool Init();

	void SetSource(int idsrc,int nx,int ny); // nastavi aktualni zdroj
	void CreateSprite(int id,int source,int id_tooltip,const char * script); // vytvori tlacidlo

protected:
	PanelList * GetList(int id);
	PanelSprite * GetSprite(int id);
};*/

#endif // _PANELS_H_
