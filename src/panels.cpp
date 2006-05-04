
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "panels.h"

using namespace HoeGame;

void TButton::Click()
{
	if (script[0] != 0)
		GetLua()->func(script);
}

ControlPanel::ControlPanel()
{
	m_numbuttons = 0;
	m_selbutton = -1;
}

bool ControlPanel::Init()
{
	return true;
}

int ControlPanel::Draw(IHoe2D * hoe2d)
{
	hoe2d->SetRect(800,600);
	for (int i=0;i < this->m_numbuttons;i++)
	{
		THoeRect * r = &this->m_buttons[i].rect;
		hoe2d->PaintRect(r->left-3,r->right+3,r->top-3,r->bottom+3,i==m_selbutton ? 0xffff0000:0x80ff0000,true);
        hoe2d->Blt(r,this->m_buttons[i].picture);
	}
	// uplne dole
	/*static IHoePicture * pic = (IHoePicture *)GetEngine()->Create("picture hud");
	hoe2d->SetRect(1024, 768);
	THoeRect r = { 0, 0, 1024, 768};
	hoe2d->SetAlpha(true);
	hoe2d->Blt(&r, pic);*/
	// tooltip
	if (m_selbutton != -1)
	{
		static IHoeFont * f= (IHoeFont*)GetResMgr()->GetResource(ID_INFO_FONT);
		f->DrawText(m_buttons[m_selbutton].rect.left, m_buttons[m_selbutton].rect.top+60,
			0xbbb0b0b0, m_buttons[m_selbutton].tooltip);
	}
	return 0;
}

int ControlPanel::l_AddButton(lua_State * L)
{
	LuaParam lp(L);
	if (lp.CheckPar(3,"s*s", "AddButton"))
	{
		const char * tt;
		if (!lp.IsNum(-2))
			tt = lp.GetString(-2);
		else
			tt = GetLang()->Get(lp.GetNum(-2));
		GetBecher()->GetControlPanel()->AddButton(lp.GetNum(-3),lp.GetString(-1),tt);
	}
	return 0;
}

int ControlPanel::l_ClearButtons(lua_State * L)
{
	LuaParam lp(L);
	if (lp.CheckPar(0,"","ClearButtons"))
	{
		ControlPanel * cp = GetBecher()->GetControlPanel();
		cp->m_selbutton = -1;
	}
	return 0;
}

int ControlPanel::GetButton(float X, float Y)
{
	for (int i=0;i < m_numbuttons;i++)
	{
		const TButton & b = m_buttons[i];
		if (X >= b.rect.left && X <= b.rect.right && Y >= b.rect.top && Y <= b.rect.bottom)
			return i;
	}
	return -1;
}

bool ControlPanel::MouseMove(float X, float Y)
{
	this->m_selbutton = GetButton(X,Y);
	return (this->m_selbutton != -1);
}

TButton * ControlPanel::AddButton(int idres, const char * func, const char * tooltip)
{
	m_buttons[m_numbuttons].picture = (IHoePicture*)GetResMgr()->ReqResource(idres);
	if (func)
        strncpy(m_buttons[m_numbuttons].script,func,sizeof(m_buttons[m_numbuttons].script)-1);
	else
		m_buttons[m_numbuttons].script[0] = '\0';
	if (tooltip)
        strncpy(m_buttons[m_numbuttons].tooltip,tooltip,sizeof(m_buttons[m_numbuttons].tooltip)-1);
	else
		m_buttons[m_numbuttons].tooltip[0] = '\0';

	m_buttons[m_numbuttons].rect.top = 50;
	m_buttons[m_numbuttons].rect.bottom = m_buttons[m_numbuttons].rect.top + 32 + 16;
	m_buttons[m_numbuttons].rect.left = (m_numbuttons == 0) ? 10:m_buttons[m_numbuttons-1].rect.right + 10;
	m_buttons[m_numbuttons].rect.right = m_buttons[m_numbuttons].rect.left + 32 + 16;
	
	return &m_buttons[m_numbuttons++];
}

/////////////////////////////////////

/*void ControlPanel::PanelSprite::CreateRect(int nx,int ny,int a)
{
	rect.left = float(a % nx) / nx;
	rect.right = float(a % nx + 1) / nx;
	rect.top = float (a / ny) / nx;
	rect.bottom = float ((a / ny) + 1) / ny;
}
//////////////////////////////////////

ControlPanel::ControlPanel()
{
	m_source = NULL;
	m_nsprites = 0;
	m_nlists = 0;
	m_tooltip = NULL;
	m_tooltip_font = NULL;
}

ControlPanel::~ControlPanel()
{
}



void ControlPanel::SetSource(int idsrc,int nx,int ny)
{
	this->m_source = (iHoePicture *)Becher::GetResMgr()->ReqResource(idsrc);
	this->m_src_nx = nx;
	this->m_src_ny = ny;
}

void ControlPanel::CreateSprite(int id,int source,int id_tooltip,const char * script)
{
	memset(&m_sprites[m_nsprites],0,sizeof(PanelSprite));

	m_sprites[m_nsprites].id = id;
	m_sprites[m_nsprites].pic = m_source;
	if (id_tooltip)
		m_sprites[m_nsprites].tooltip = Becher::GetLang()->Get(id_tooltip);
	strncpy(m_sprites[m_nsprites].script,script,32);
	m_sprites[m_nsprites].CreateRect(m_src_nx,m_src_ny,source);
	
	m_nsprites++;
}

int ControlPanel::Draw(iHoe2D * hoe2d)
{
	HOE_RECT rect = {100,0,150,50};
	hoe2d->SetRect(0,0);

	// get list
	int idlist;
	if (Becher::GetBecher()->GetSelectedObject())
		idlist = Becher::GetBecher()->GetSelectedObject()->GetTypeID();
	else
		idlist = 0;

	PanelList * l = GetList(idlist);
	if (l)
	{
		for (int i=0;i < l->numsprites;i++)
		{
			hoe2d->Blt(&rect,l->sprites[i]->pic,&l->sprites[i]->rect);
			rect.left += 50;rect.right += 50;
		}
	}

	if (m_tooltip_font && m_tooltip)
		m_tooltip_font->DrawText(100,100,0xff0000ff,m_tooltip);

	return 0;
}

ControlPanel::PanelSprite * ControlPanel::GetSprite(float x, float y)
{
	HOE_RECT rect = {100,0,150,50};

	// get list
	int idlist;
	if (Becher::GetBecher()->GetSelectedObject())
		idlist = Becher::GetBecher()->GetSelectedObject()->GetTypeID();
	else
		idlist = 0;

	PanelList * l = GetList(idlist);
	if (l)
	{
		for (int i=0;i < l->numsprites;i++)
		{
			if (x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom)
			{
				return l->sprites[i];
			}
			rect.left += 50;rect.right += 50;
		}
	}

	return NULL;
}

bool ControlPanel::Move(float x,float y)
{
	const PanelSprite * ps = GetSprite(x,y);
	if (ps)
	{
		m_tooltip = ps->tooltip;
		return true;
	}

	m_tooltip = NULL;
	return false;
}

bool ControlPanel::Click(float x,float y)
{
	const PanelSprite * ps = GetSprite(x,y);
	if (ps && ps->script[0])
	{
		lua_run(ps->script);
		return true;
	}
	return false;
}

ControlPanel::PanelSprite * ControlPanel::GetSprite(int id)
{
	for (int i=0; i < this->m_nsprites;i++)
		if (this->m_sprites[i].id == id)
			return &this->m_sprites[i];
	return NULL;
}

ControlPanel::PanelList * ControlPanel::GetList(int id)
{
	for (int i=0; i < this->m_nlists;i++)
		if (this->m_lists[i].id == id)
			return &this->m_lists[i];
	return NULL;	
}

bool ControlPanel::AddSpriteToList(PanelList * pl, int id)
{
	// kontrola jestli uz tu davno neni
	for (int i=0;i < pl->numsprites;i++)
		if (pl->sprites[i]->id == id)
			return false;
	
	assert(pl->numsprites < CP_SPRITESINLIST);
	PanelSprite * ps = this->GetSprite(id);
	if (!ps)
		return false;

	pl->sprites[pl->numsprites++] = ps;
	return true;

}

bool ControlPanel::AddToList(int idlist, int id)
{
	PanelList * l = this->GetList(idlist);
	if (!l)
	{
		assert(this->m_nlists < CP_NUMLIST);
		m_lists[m_nlists].id = idlist;
		m_lists[m_nlists].numsprites = 0;
		l = &m_lists[m_nlists];
		m_nlists++;
	}
	return AddSpriteToList(l,id);
}
*/

//////////////////////////////////////////////////////////////////

int InfoPanel::Info::comp(const void * v1,const void *v2)
{
	const Info * p1 = (Info*)v1;
	const Info * p2 = (Info*)v2;

	if (!p1->visible)
	{
		if (p2->visible)
			return 1;
		else
			return 0;
	}
	
	if (!p2->visible)
		return -1;

	if (p1->y < p2->y)
		return -1;
	else
		return 1;
}

InfoPanel::InfoPanel()
{
	font = NULL;
	memset(m_infos,0,sizeof(m_infos));
}

InfoPanel::~InfoPanel()
{
}

bool InfoPanel::Init(float min, float bottom, float left)
{
	font = (IHoeFont*)GetResMgr()->ReqResource(ID_INFO_FONT);

	stepsize = font->GetTextHeight();
	minheight = min;
	startheight = bottom - stepsize;

	return true;
}

int InfoPanel::Draw(IHoe2D * hoe2d)
{
	hoe2d->SetRect(800,600);
	float t = GetEngine()->SysFloatTime();

	// vypsat info o objektu
	BecherObject * o = GetBecher()->GetLevel()->GetSelectedObject();
	if (o)
	{
		char buff[200];
		float top = 600.f-(o->GetNumInfos()+1)*20;
		for (int i=0; i < o->GetNumInfos();i++)
		{
			o->GetInfo(i, buff, sizeof(buff));
			font->DrawText(30, top, 0xff00cc00, buff);
			top += 20;
		}
	}

	// predelat cas by se mel odecitat 

	for (int i=0;i < MAX_INFOS;i++)
	{
		if (m_infos[i].visible)
		{
			const float tt = m_infos[i].totime - t;
			unsigned long alpha;
			if (tt < 0)
			{
				m_infos[i].visible = false;
				continue;
			}
			alpha = (unsigned long)(tt * float(0x80));
			if (alpha > 0xff) alpha = 0xff;

			font->DrawText(5,m_infos[i].y,((alpha & 0xff) << 24) | 0x00ffffff,m_infos[i].info);
		}
	}
	
	return 0;
}

void InfoPanel::Add(const char * str)
{
	int a = -1;

	for (int i=0;i < MAX_INFOS;i++)
	{
		if (m_infos[i].visible)
		{
			// move up
			m_infos[i].y -= stepsize;
			if (m_infos[i].y < minheight)
			{
				m_infos[i].visible = false;
			}
			else
				continue;
		}
		
		if (a == -1)
		{
			a = i;
		}
	}

	if (a == -1) // get top list
	{
		a = 0;
		for (int i=0;i < MAX_INFOS;i++)
		{
			if (m_infos[i].visible && m_infos[a].y > m_infos[i].y)
			{
				a = i;
			}
		}
	}

	assert(a != -1);

	strcpy(m_infos[a].info,str);
	m_infos[a].y = startheight;
	m_infos[a].totime = GetEngine()->SysFloatTime() + 5.f;
	m_infos[a].visible = true;

	//qsort(m_infos,MAX_INFOS,sizeof(Info),Info::comp);

	//GetConsole()->Con_Print(str);
}

void InfoPanel::Addf(const char * format, ...)
{
	static char szBuff[512];

	va_list args;

	va_start(args, format);
#ifdef _WIN32
	_vsnprintf( szBuff, 512, format, args );
#else
	vsnprintf( szBuff, 512, format, args );
#endif
	va_end(args);

	Add(szBuff);
}

void InfoPanel::Add(int id)
{
	Add(GetLang()->GetString(id));
}

int InfoPanel::l_info(lua_State * L)
{
	LuaParam lp(L);
	if (lp.GetNumParam() != 1)
		return 0;

	//if (lp.IsNum(-1))
	//	GetBecher()->GetInfoPanel()->Add(GetLang()->GetString(lp.GetNum(-1)));
	//if (lp.IsString(-1))
		GetBecher()->GetInfoPanel()->Add(lp.GetString(-1));
	return 0;
}



