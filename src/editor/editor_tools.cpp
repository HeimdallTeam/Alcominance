
#include "../StdAfx.h"
#include "editor_tools.h"
#include "../buildings.h"
#include "../becher.h"
#include "editor_map.h"
//#include "plugin_map.h"

enum {
	IDB_SUGAR = 1,
	IDB_BABA,
	IDB_STROM,
	IDB_FARM,
	IDB_BRIDGE,
	IDB_DESTILATE,
	IDB_FACTORY,
	IDB_WATERHOLE,
	IDB_STORE,
	IDB_SHOP,
	IDB_TROLL,
};

BEGIN_EVENT_TABLE(ToolObjects, wxChoicebook)
    EVT_BUTTON(IDB_SUGAR,ToolObjects::OnClick)
    EVT_BUTTON(IDB_BABA,ToolObjects::OnClick)
    EVT_BUTTON(IDB_FARM,ToolObjects::OnClick)
    EVT_BUTTON(IDB_BRIDGE,ToolObjects::OnClick)
    EVT_BUTTON(IDB_STROM,ToolObjects::OnClick)
    EVT_BUTTON(IDB_DESTILATE,ToolObjects::OnClick)
	EVT_BUTTON(IDB_FACTORY,ToolObjects::OnClick)
	EVT_BUTTON(IDB_WATERHOLE,ToolObjects::OnClick)
	EVT_BUTTON(IDB_STORE,ToolObjects::OnClick)
	EVT_BUTTON(IDB_SHOP,ToolObjects::OnClick)
	EVT_BUTTON(IDB_TROLL,ToolObjects::OnClick)
END_EVENT_TABLE()

#include "../../resource/maleikony/chaloupka.xpm"
#include "../../resource/maleikony/farma.xpm"
#include "../../resource/maleikony/lihovar.xpm"
#include "../../resource/maleikony/cukrovar.xpm"
#include "../../resource/maleikony/sklad.xpm"
#include "../../resource/maleikony/obchod.xpm"
#include "../../resource/maleikony/stromy.xpm"
#include "../../resource/maleikony/studna.xpm"
#include "../../resource/maleikony/tovarna.xpm"
#include "../../resource/maleikony/most.xpm"

#define BT_SIZE wxSize(28,28)
#define BT_P(x,y) wxPoint(28 * x + 6, 28 * y + 3)

ToolObjects::ToolObjects(wxWindow * parent,int width, int height)
	: wxChoicebook(parent,wxID_ANY,wxDefaultPosition, wxSize(width,height))
{
	wxWindow * p  = new wxWindow(this,-1);
	wxButton * b = new wxBitmapButton(p,IDB_FACTORY,wxBitmap(tovarna_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Factory") );
	b = new wxBitmapButton(p,IDB_BABA,wxBitmap(chaloupka_xpm),BT_P(1,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Chalupa") );
	b = new wxBitmapButton(p,IDB_FARM,wxBitmap(farma_xpm),BT_P(2,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Farma") );
	b = new wxBitmapButton(p,IDB_DESTILATE,wxBitmap(lihovar_xpm),BT_P(3,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Destilate") );
	b = new wxBitmapButton(p,IDB_SUGAR,wxBitmap(cukrovar_xpm),BT_P(4,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Sugar") );
	b = new wxBitmapButton(p,IDB_STORE,wxBitmap(sklad_xpm),BT_P(0,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Store") );
	b = new wxBitmapButton(p,IDB_SHOP,wxBitmap(obchod_xpm),BT_P(1,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Shop") );


	//b->SetForegroundColour(*wxBLUE);
	wxWindow * p2  = new wxWindow(this,-1);
	b = new wxBitmapButton(p2,IDB_STROM,wxBitmap(stromy_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Strom") );
	b = new wxBitmapButton(p2,IDB_BRIDGE,wxBitmap(most_xpm),BT_P(1,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Bridge") );
	b = new wxBitmapButton(p2,IDB_WATERHOLE,wxBitmap(studna_xpm),BT_P(2,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Water Hole") );
	b = new wxBitmapButton(p2,IDB_TROLL,wxBitmap(studna_xpm),BT_P(3,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Troll") );


	AddPage(p,_("Buildings"));
	AddPage(p2,_("Map elements"));

}

void ToolObjects::OnClick(wxCommandEvent& event)
{
	if (!EditorMap::Get())
	{
		wxMessageBox(_("First must create map."));
		return;
	}

	unsigned long type = 0;
	switch (event.GetId())
	{
	case IDB_SUGAR:
		type = EBO_Sugar;
		break;
	case IDB_BABA:
		type = EBO_HerbeWoman;
		break;
	case IDB_FARM:
		type = EBO_Farm;
		break;
	case IDB_BRIDGE:
		type = EBO_Bridge;
		break;
	case IDB_STROM:
		type = EBO_Tree;
		break;
	case IDB_DESTILATE:
		type = EBO_Destilate;
		break;
	case IDB_FACTORY:
		type = EBO_Factory;
		break;
	case IDB_WATERHOLE:
		type = EBO_WaterHole;
		break;
	case IDB_STORE:
		type = EBO_Store;
		break;
	case IDB_SHOP:
		type = EBO_Shop;
		break;
	case IDB_TROLL:
		type = EBO_Troll;
		break;
	};

	EditorMap::Get()->AddNewObject(type);
	//if (type != 0 && m_actmap)
	//	m_actmap->NewSelectObject(type);
	
}

////////////////////////////////////////////////////
// terrain

BEGIN_EVENT_TABLE(TerrainObject, wxPanel)
    EVT_BUTTON(IDB_FACTORY,TerrainObject::OnClick)
END_EVENT_TABLE()

TerrainObject::TerrainObject(wxWindow * parent,int width, int height)
	: wxPanel(parent,wxID_ANY,wxDefaultPosition, wxSize(width,height))
{
	wxButton * b = new wxBitmapButton(this,IDB_FACTORY,wxBitmap(tovarna_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Factory") );

}

void TerrainObject::OnClick(wxCommandEvent& event)
{
	if (!EditorMap::Get())
	{
		wxMessageBox(_("First must create map."));
		return;
	}
	EditorMap::Get()->GetTerrain()->MoveHeight(0,0,50,-5);
}

