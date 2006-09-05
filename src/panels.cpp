
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "panels.h"

using namespace HoeGame;

void BecherButton::OnClick()
{
	if (script[0] != 0)
		GetLua()->func(script);
}

void BecherButton::SetButt(int idres, const char * func, const char * tp)
{
	if (idres != -1)
		SetPicture((IHoePicture*)GetResMgr()->ReqResource(idres));
	else
		SetPicture(NULL);
	if (func)
        strncpy(script,func,sizeof(script)-1);
	else
		script[0] = '\0';
	if (tp)
        strncpy(tooltip,tp,sizeof(tooltip)-1);
	else
		tooltip[0] = '\0';
}

HUD::HUD()
{
	m_info = NULL;
	memset(m_butt,0,sizeof(m_butt));
	m_num = 0;
}

HoeGame::BaseGui * HUD::CreateGUI(const char * type)
{
	GuiItem * g = NULL;
#define IS(t) (strcmp(type,t)==0)
	if (IS("button"))
		g = new BecherButton;
	else
		return Hoe2DFigure::CreateGUI(type);

	m_list.Add(g);
	return g;
}

bool HUD::Load(const char * fname)
{
	if (!Hoe2DFigure::Load(fname))
		return false;
	m_info = dynamic_cast<HoeGame::InfoPanel*>(ReqItem("info"));

	for (int i=0;i < 16;i++)
	{
		char str[15];
		sprintf(str,"butt_%02d",i);
		GuiItem * item = GetItem(str);
		if (item==NULL)
			break;
		m_butt[i] = dynamic_cast<BecherButton*>(item);
	}
	return true;
}



void HUD::AddButton(int idres, const char * func, const char * tooltip)
{
	if (m_num == 16 || m_butt[m_num] == NULL)
		return;
	m_butt[m_num]->SetButt(idres, func, tooltip);
	m_num++;
}

void HUD::ShowReset()
{
	for (int i=0;i<m_num;i++)
	{
		m_butt[i]->SetButt(-1,NULL,NULL);
	}
	m_num = 0;
}

BecherButton * HUD::GetButton(float X, float Y)
{
	for (int i=0;i < 16;i++)
	{
		if (!m_butt[i])
			return NULL;
		const THoeRect & r = m_butt[i]->GetRect();
		if (X >= r.left && X <= r.right && Y >= r.top && Y <= r.bottom)
			return m_butt[i];
	}
	return NULL;
}

/*int ControlPanel::Draw(IHoe2D * hoe2d)
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
}*/

int HUD::l_AddButton(lua_State * L)
{
	LuaParam lp(L);
	if (lp.CheckPar(3,"s*s", "AddButton"))
	{
		const char * tt;
		if (!lp.IsNum(-2))
			tt = lp.GetString(-2);
		else
			tt = GetLang()->Get(lp.GetNum(-2));
		GetLevel()->GetPanel()->AddButton(lp.GetNum(-3),lp.GetString(-1),tt);
	}
	return 0;
}

int HUD::l_ClearButtons(lua_State * L)
{
	LuaParam lp(L);
	if (lp.CheckPar(0,"","ClearButtons"))
	{
		GetLevel()->GetPanel()->ShowReset();
	}
	return 0;
}

int HUD::l_info(lua_State * L)
{
	LuaParam lp(L);
	if (lp.GetNumParam() != 1)
		return 0;

	if (lp.IsNum(-1))
		GetLevel()->GetPanel()->GetInfo()->Add(GetLang()->GetString(lp.GetNum(-1)));
	else if (lp.IsString(-1))
		GetLevel()->GetPanel()->GetInfo()->Add(lp.GetString(-1));
	return 0;
}

/*bool ControlPanel::MouseMove(float X, float Y)
{
	this->m_selbutton = GetButton(X,Y);
	return (this->m_selbutton != -1);
}*/

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



