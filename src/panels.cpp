
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

int ControlPanel::Draw(IHoe2D * hoe2d)
{
	hoe2d->SetRect(800,600);
	for (int i=0;i < this->m_numbuttons;i++)
	{
		THoeRect * r = &this->m_buttons[i].rect;
		hoe2d->PaintRect(r->left-3,r->right+3,r->top-3,r->bottom+3,i==m_selbutton ? 0xffff0000:0x80ff0000,true);
        hoe2d->Blt(r,this->m_buttons[i].picture);
	}
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
	/*LuaParam lp(L);
	if (lp.CheckPar(3,"s*s", "AddButton"))
	{
		const char * tt;
		if (!lp.IsNum(-2))
			tt = lp.GetString(-2);
		else
			tt = GetLang()->Get(lp.GetNum(-2));
		GetBecher()->GetControlPanel()->AddButton(lp.GetNum(-3),lp.GetString(-1),tt);
	}*/
	return 0;
}

int ControlPanel::l_ClearButtons(lua_State * L)
{
	/*LuaParam lp(L);
	if (lp.CheckPar(0,"","ClearButtons"))
	{
		ControlPanel * cp = GetBecher()->GetControlPanel();
		cp->m_selbutton = -1;
	}*/
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



