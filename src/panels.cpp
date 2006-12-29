
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "panels.h"
#include "minimap.h"

using namespace HoeGame;

void BecherButton::OnClick()
{
	if (script[0] != 0)
		GetLua()->func(script);
}

void BecherButton::SetButt(int idres, const char * func, const char * tp)
{
	if (idres == -1)
	{
		Hide();
		return;
	}

	SetPicture((IHoePicture*)GetResMgr()->ReqResource(idres));
	
	if (func)
        strncpy(script,func,sizeof(script)-1);
	else
		script[0] = '\0';
	if (tp)
        strncpy(tooltip,tp,sizeof(tooltip)-1);
	else
		tooltip[0] = '\0';
	SetToolTip(tooltip);
	Show();
}

HUD::HUD()
{
	m_info = NULL;
	memset(m_butt,0,sizeof(m_butt));
	m_num = 0;
	m_map = NULL;
	m_selhud = NULL;
	m_selobj = NULL;
}

HoeGame::Gui::Base * HUD::CreateGUI(const char * type)
{
	Gui::Item * g = NULL;
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
	m_info = dynamic_cast<HoeGame::Gui::InfoPanel*>(ReqItem("info", HoeGame::Gui::EInfoPanel));

	for (int i=0;i < 16;i++)
	{
		char str[15];
		sprintf(str,"butt_%02d",i);
		Gui::Item * item = GetItem(str, HoeGame::Gui::EButton);
		if (item==NULL)
			break;
		m_butt[i] = dynamic_cast<BecherButton*>(item);
		m_butt[i]->Hide();
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
		m_butt[i]->Hide();
	}
	m_num = 0;
}

void HUD::Draw(IHoe2D * hoe2d)
{
	HoeGame::Hoe2DFigure::Draw(hoe2d);
	if (m_map)
		m_map->Draw(hoe2d);
	if (m_selhud)
		m_selhud->Draw(hoe2d);
}

bool HUD::SetObjectHud(const char * fname, BecherObject * obj)
{
	m_selobj = obj;
	if (!fname)
	{
		if (m_selhud)
		{
			delete m_selhud;
			m_selhud = NULL;
		}
		return true;
	}
		
	if (!m_selhud)
		m_selhud = new HoeGame::Hoe2DFigure();
	m_selhud->Clear();
	if (!m_selhud->Load(fname))
		return false;
	// najit vsechny texty a nastavit jim overlap
	const HoeCore::List<Gui::Item*> & list = m_selhud->GetItems();
	for (uint i=0;i < list.Count();i++)
		if (list.Get(i)->GetType()==HoeGame::Gui::EText)
		{
			dynamic_cast<HoeGame::Gui::Font*>(list.Get(i))->SetTextOverlaper(
				this, H2DC_TEXTOVERLAP(HUD::InfoOverlap));
		}
	return true;
}

const char * HUD::InfoOverlap(HoeGame::Gui::Base * sender, const char * text)
{
	//if (m_act)
	{
		//sprintf(m_sugarinfo,"%d cukru.", m_act->m_sugar.GetNum());
		//sprintf(m_produkce,"Produkce je z %d%% hotova.", int(farm->m_grow * 100.f));
		// prevorat podle toho puvodniho stringu
		const char * po = text;
		char * pp = m_olverlapedtext;
		while (*po) 
		{
			// preskoceni znaku lomitkem
			if (*po == '\\' && po[1] == '$')
			{
				po+=2; *pp++ = '$';
			}
			else if (*po == '$')
			{
				// nahrazeni promene
				po++;
				char *pv = pp;
				while (*po && *po != '$') *pv++ = *po++;
				*pv = '\0';
				if (*po != '$') // reject
				{
					sprintf(pp, "$unclosed");
					return m_olverlapedtext;
				}
				po++;
				ReplaceVar(pp, 256);
				while (*pp) pp++;
			}
			else
				*pp++ = *po++;
		}
		*pp = '\0';
		return m_olverlapedtext;
	}
	return text;
}

void HUD::ReplaceVar(char * text, size_t n)
{
	if (m_selobj)
	{
		m_selobj->GetInfo( BINFO_Custom, text, n);
	}
	else
	{
		text[0] = 'X';
		text[1] = 0;
	}
}

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

