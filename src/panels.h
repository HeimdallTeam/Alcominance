
#ifndef _PANELS_H_
#define _PANELS_H_

class Panel
{
public:
};

struct TButton
{
	bool enable;
	IHoePicture * picture;
	THoeRect rect;
	char tooltip[1024];
	char script[512];
	void Click();
};

class ControlPanel : public Panel
{
	//IHoePicture * m_back;
	TButton m_buttons[64];
	int m_numbuttons;
	int m_selbutton; //< vybrane tlacidlo
protected:
public:
	ControlPanel();
	bool Init();
	int Draw(IHoe2D * hoe2d);
	/** zpracovani pohybu mysi, pokud se ma zpracovano,vraci true */
	bool MouseMove(float X, float Y);
	int GetButton(float X, float Y);
	TButton * GetButton(int index) { return &m_buttons[index]; }
	int GetNumButtons() { return m_numbuttons; }
	TButton * AddButton(int idres, const char * func, const char * tooltip);
	void ShowReset() { m_numbuttons = 0; }

	LUA_FUNCTION(l_ClearButtons);
	LUA_FUNCTION(l_AddButton);
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

#define MAX_INFOS 20

class InfoPanel : public Panel
{
	struct Info
	{
		bool visible;
		float y;
		float totime;
		char info[512];
		static int comp(const void *,const void *);
	};

	Info m_infos[MAX_INFOS];
	int act;
	float startheight;
	float minheight;
	float stepsize;

	IHoeFont * font;

	float infoStartX,infoStartY;
	float infoX,infoY;

public:
	InfoPanel();
	~InfoPanel();

	bool Init(float min, float bottom, float left);
	int Draw(IHoe2D * hoe2d);

	void Add(int id);
	void Add(const char *);
	void Addf(const char *, ...);

	LUA_FUNCTION(l_info);
};

#endif // _PANELS_H_
