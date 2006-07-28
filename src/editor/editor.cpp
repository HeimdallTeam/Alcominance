
#include "../StdAfx.h"
#include "../becher.h"
//#include "../../resource/logo.xpm"
#include "editor_tools.h"
//#include "plugin_map.h"
#include "../id.h"
#include "editor_map.h"
#include "editor.h"
#include "help.h"
#include "../object.h"
#include "map_properties.h"

#include <wx/arrimpl.cpp>

IMPLEMENT_APP(BecherEditApp)

WX_DEFINE_OBJARRAY(ArrayTextures);

const char * EditorName = "BecherEditor";

// menu
enum {
	ID_SHOWRES = HoeEditor::ID_CUSTOMMENU_FIRST,
	ID_OBJECT,
	ID_MAPSETTINGS,
	ID_HELP,
	ID_ENGINECON,
	ID_TERRAINWIRE,
	ID_OBJECTS,
	ID_OBJECTSWIRE,
	ID_OBJECTSHIDE,
	ID_OBJECTSFULL,
	ID_SYSOBJECTS,
	ID_TERRAINTEX,
};

BEGIN_EVENT_TABLE(BecherEdit, HoeEditor::LevelEditor)
	EVT_MENU(ID_SHOWRES, BecherEdit::OnResMgr)
	EVT_MENU(ID_MAPSETTINGS, BecherEdit::OnMapSettings)
	EVT_MENU(HoeEditor::ID_NEW, BecherEdit::OnNewFile)
	EVT_MENU(HoeEditor::ID_OPEN, BecherEdit::OnOpenFile)
	EVT_MENU(HoeEditor::ID_SAVE, BecherEdit::OnSaveFile)
	EVT_MENU(HoeEditor::ID_SAVEAS, BecherEdit::OnSaveFile)
	EVT_MENU(HoeEditor::ID_ABOUT, BecherEdit::OnAbout)
	EVT_MENU(ID_TERRAINWIRE, BecherEdit::OnTypeShow)
	EVT_MENU(ID_OBJECTSWIRE, BecherEdit::OnTypeShow)
	EVT_MENU(ID_OBJECTSHIDE, BecherEdit::OnTypeShow)
	EVT_MENU(ID_OBJECTSFULL, BecherEdit::OnTypeShow)
	EVT_MENU(ID_SYSOBJECTS, BecherEdit::OnTypeShow)
	EVT_MENU(ID_HELP, BecherEdit::OnHelp)
	EVT_MENU(ID_TERRAINTEX, BecherEdit::OnTerrainTextures)
	//EVT_MENU(ID_SHOWINFO, BecherEdit::OnShowInfo)

	EVT_MENU_RANGE(ID_OBJECT, ID_OBJECT + EBO_Max, BecherEdit::OnNewObject)

END_EVENT_TABLE()

// tools
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
	IDB_SAW,
// system
	IDB_SOUND,
// textures
	IDB_NOTEX,
	IDB_TEX1,
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
	EVT_BUTTON(IDB_SAW,ToolObjects::OnClick)
	// system
	EVT_BUTTON(IDB_SOUND,ToolObjects::OnClick)
END_EVENT_TABLE()

// gfx

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
// system
#include "../../resource/maleikony/sound.xpm"


#define BT_SIZE wxSize(28,28)
#define BT_P(x,y) wxPoint(28 * x + 6, 28 * y + 3)

ToolObjects::ToolObjects(wxWindow * parent)
	: wxChoicebook(parent,wxID_ANY,wxDefaultPosition, wxSize(80,50))
{
	wxWindow * p  = new wxWindow(this,-1);
	wxButton * b = new wxBitmapButton(p,IDB_FACTORY,wxBitmap(tovarna_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Factory") );
	b = new wxBitmapButton(p,IDB_BABA,wxBitmap(chaloupka_xpm),BT_P(1,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Chalupa") );
	b = new wxBitmapButton(p,IDB_FARM,wxBitmap(farma_xpm),BT_P(2,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Farm") );
	b = new wxBitmapButton(p,IDB_DESTILATE,wxBitmap(lihovar_xpm),BT_P(3,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Destilate") );
	b = new wxBitmapButton(p,IDB_SUGAR,wxBitmap(cukrovar_xpm),BT_P(4,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Sugar") );
	b = new wxBitmapButton(p,IDB_STORE,wxBitmap(sklad_xpm),BT_P(0,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Store") );
	b = new wxBitmapButton(p,IDB_SHOP,wxBitmap(obchod_xpm),BT_P(1,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Shop") );
	b = new wxBitmapButton(p,IDB_SAW,wxBitmap(farma_xpm),BT_P(2,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Saw") );


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

	wxWindow * p3  = new wxWindow(this,-1);
	b = new wxBitmapButton(p3,IDB_SOUND,wxBitmap(Sound_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Sound instance") );

	AddPage(p,_("Buildings"));
	AddPage(p2,_("Map elements"));
	AddPage(p3,_("System tools"));

}

void ToolObjects::OnClick(wxCommandEvent& event)
{
	if (!BecherEdit::Get()->IsMapLoaded())
	{
		wxMessageBox(_("First must create map."));
		return;
	}

	EObjType type = EBO_Max;
	bool rep = false;
	bool rnd = false;
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
		rep = true;
		rnd = true;
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
	case IDB_SAW:
		type = EBO_Saw;
		break;
	case IDB_SOUND:
		type = EBS_Sound;
		break;
	};

	ToolCreateObject * t = new ToolCreateObject(type,rep,rnd);
	BecherEdit::Get()->SetTool(t);
}

////////////////////////////////////////////////////
// terrain

BEGIN_EVENT_TABLE(TerrainObject, wxPanel)
	EVT_BUTTON(IDB_NOTEX, TerrainObject::OnClick)
	EVT_BUTTON(IDB_TEX1, TerrainObject::OnClick)
	EVT_BUTTON(IDB_TEX1+1, TerrainObject::OnClick)
	EVT_BUTTON(IDB_TEX1+2, TerrainObject::OnClick)
	EVT_BUTTON(IDB_TEX1+3, TerrainObject::OnClick)
	EVT_BUTTON(IDB_TEX1+4, TerrainObject::OnClick)
	EVT_BUTTON(IDB_TEX1+5, TerrainObject::OnClick)
	EVT_BUTTON(IDB_TEX1+6, TerrainObject::OnClick)
END_EVENT_TABLE()

#include "../../resource/maleikony/trava.xpm"
#include "../../resource/maleikony/City_Dirt.xpm"
#include "../../resource/maleikony/City_SquareTiles.xpm"
#include "../../resource/maleikony/Cliff1.xpm"
#include "../../resource/maleikony/Concrete.xpm"
#include "../../resource/maleikony/MetalPlateCliff.xpm"
#include "../../resource/maleikony/MetalPlateTiles.xpm"

TerrainObject::TerrainObject(wxWindow * parent)
	: wxPanel(parent,wxID_ANY,wxDefaultPosition, wxSize(80,250))
{
	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
	wxStaticBox * sb = new wxStaticBox(this, -1, _("Terrain"), wxPoint(10,10),
                wxSize(140,80));
	topsizer->Add( sb);

	sb = new wxStaticBox(this, -1, _("Textures"), wxPoint(10,100),
                wxSize(140,80));

#undef BT_P
#define BT_P(x,y) wxPoint(28 * x + 12, 28 * y + 12)

	wxButton * b = new wxBitmapButton(sb,IDB_NOTEX,wxBitmap(tovarna_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Empty terrain") );
	b = new wxBitmapButton(sb,IDB_TEX1,wxBitmap(trava_xpm),BT_P(1,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Trava") );
	b = new wxBitmapButton(sb,IDB_TEX1+1,wxBitmap(City_Dirt_xpm),BT_P(2,0),BT_SIZE/*,BS_FLAT*/);
	b = new wxBitmapButton(sb,IDB_TEX1+2,wxBitmap(City_SquareTiles_xpm),BT_P(3,0),BT_SIZE/*,BS_FLAT*/);
	b = new wxBitmapButton(sb,IDB_TEX1+3,wxBitmap(Cliff1_xpm),BT_P(0,1),BT_SIZE/*,BS_FLAT*/);
	b = new wxBitmapButton(sb,IDB_TEX1+4,wxBitmap(Concrete_xpm),BT_P(1,1),BT_SIZE/*,BS_FLAT*/);
	b = new wxBitmapButton(sb,IDB_TEX1+5,wxBitmap(MetalPlateCliff_xpm),BT_P(2,1),BT_SIZE/*,BS_FLAT*/);
	b = new wxBitmapButton(sb,IDB_TEX1+6,wxBitmap(MetalPlateTiles_xpm),BT_P(3,1),BT_SIZE/*,BS_FLAT*/);
                //long style = 0,
                //const wxString& name = wxStaticBoxNameStr)
	//wxButton * b = new wxBitmapButton(this,IDB_FACTORY,wxBitmap(tovarna_xpm),wxPoint(0,0),BT_SIZE/*,BS_FLAT*/);
	//b->SetToolTip( _("Factory") );

	topsizer->Add( sb);//,
                //long style = 0,
                //const wxString& name = wxStaticBoxNameStr)

}

void TerrainObject::OnClick(wxCommandEvent& e)
{
	if (!BecherEdit::Get()->IsMapLoaded())
	{
		wxMessageBox(_("First must create map."));
		return;
	}
	byte id = 0xff;
	switch (e.GetId())
	{
	case IDB_TEX1:
		id = 1;
		break;
	case IDB_TEX1+1:
		id = 2;
		break;
	case IDB_TEX1+2:
		id = 3;
		break;
	case IDB_TEX1+3:
		id = 4;
		break;
	case IDB_TEX1+4:
		id = 5;
		break;
	case IDB_TEX1+5:
		id = 6;
		break;
	case IDB_TEX1+6:
		id = 7;
		break;

	};
	BecherEdit::Get()->SetTool(new ToolTerrain(id));
	//EditorMap::Get()->GetTerrain()->MoveHeight(0,0,50,-5);
}

////////////////////////////////////////////////////////////
TexturesDialog::TexturesDialog(wxWindow * parent)
{
	// zatim nefunkcni, je dulezity a nejdriv konfigu
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    Create(parent, wxID_ANY, _("Textures"), wxDefaultPosition, wxSize(400,400));
    //CreateButtons(wxOK|wxCANCEL);
	
	m_list = new wxListCtrl(this, wxID_ANY, wxPoint(10,10), wxSize(375,150), wxLC_REPORT);
    wxListItem itemCol;
    itemCol.SetText(_("Name"));
    m_list->InsertColumn(0, itemCol);

    itemCol.SetText(_("ID"));
    m_list->InsertColumn(1, itemCol);

    itemCol.SetText(_("Command"));
    m_list->InsertColumn(2, itemCol);
    m_list->SetColumnWidth( 0, 150 );
    m_list->SetColumnWidth( 1, 50 );
    m_list->SetColumnWidth( 2, 20 );

	// textura
	// tlacitko add a delete
	// tlacitko ok
	// misto pro upravu textury
	// picture
}




HoeEditor::BaseEditor * BecherEditApp::CreateEditor()
{
	// load lang
	m_locale.Init(wxLANGUAGE_CZECH);
	wxLocale::AddCatalogLookupPathPrefix("resource");
	wxLocale::AddCatalogLookupPathPrefix("../Hoe/HoeEditor/lang");

	m_locale.AddCatalog("editor_cs");
	m_locale.AddCatalog("cs_CZ");

	BecherEdit * e = new BecherEdit();
	e->Create("Becher Editor");

 //   wxBitmap bitmap;
 //   if (bitmap.LoadFile(_T("splash.bmp"), wxBITMAP_TYPE_BMP))

	//m_splash = new wxSplashScreen(bitmap,
 //           wxSPLASH_CENTRE_ON_PARENT/*|wxSPLASH_TIMEOUT*/,
 //           4000, e, wxID_ANY, wxDefaultPosition, wxDefaultSize,
 //           wxSIMPLE_BORDER|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
	return e;
}

void BecherEditApp::OnPostInit()
{
	//m_splash->
	//delete m_splash;
}

///////////////////////////
 BecherEdit * BecherEdit::s_actinstance = NULL;

BecherEdit::BecherEdit()
{
	assert(s_actinstance == NULL);
	s_actinstance = this;
	m_map = NULL;
	m_tool = NULL;
}

BecherEdit::~BecherEdit()
{
	CloseMap();
	assert(s_actinstance == this);
	s_actinstance = NULL;
}

bool BecherEdit::Create(const wxString & title)
{

	
/*	wxImage::AddHandler(new wxGIFHandler);
	// 
	wxHelpControllerHelpProvider* provider = new wxHelpControllerHelpProvider;
    wxHelpProvider::Set(provider);
	provider->SetHelpController(&m_help);
	if (!m_help.Initialize("BecherEditor"))
	{
		wxMessageBox(wxT("Error: Cannot initialize the help system."));
		return false;
	}
*/
	HoeEditor::LevelEditor::Create(title);

	wxSplitterWindow * split = new wxSplitterWindow(this,10, wxDefaultPosition, wxDefaultSize,0);
	split->SetSashGravity(0);
	
	// seradit do layoutu

	this->m_leftpanel.Create(split, -1);
	this->m_engview.Create(split, -1, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);

	split->SplitVertically(&m_leftpanel, &m_engview, 150);

	m_prop = new HoeEditor::PropertyGrid(GetPanelMgr());
	GetPanelMgr()->AddPanel(
		new ToolObjects(GetPanelMgr()), _("Tools"), true, true);
	GetPanelMgr()->AddPanel(
		new TerrainObject(GetPanelMgr()), _("Terrain"), true, true);
	GetPanelMgr()->AddPanel(
		m_prop, _("Properties"), false, true);


	UpdateControls();

	// create status bar
	wxStatusBar *statbar = GetStatusBar();
	statbar->SetFieldsCount(2);

	return true;
}

void BecherEdit::OnInitMenu()
{
    // file
    wxMenu * m_menuFile = new wxMenu;
	m_menuFile->Append(HoeEditor::ID_NEW, _("&New...\tCtrl+N"), _("Creates a new file or project."));
	m_menuFile->Append(HoeEditor::ID_OPEN, _("&Open...\tCtrl+O"), _("Open an existing file."));
	m_menuFile->AppendSeparator();
	m_menuFile->Append(HoeEditor::ID_SAVE, _("&Save\tCtrl+S"), _("Save file."));
	m_menuFile->Append(HoeEditor::ID_SAVEAS, _("Save &As..."), _("Save file."));
	m_menuFile->AppendSeparator();
	m_menuFile->Append(HoeEditor::ID_QUIT, _("E&xit\tAlt-F4"), _("Quit this program")); 

	// view
	wxMenu * menuView = new wxMenu;
	menuView->Append(HoeEditor::ID_SHOWLOG, _("Show &Log...\tF2"), _("Show log dialog."));
	/*menuView->Append(ID_VIEWENGINE, _("&Engine"), _("Show engine view."));
	menuView->Append(ID_VIEWCODE, _("&Code Editor"), _("Show code editor."));*/
	menuView->AppendCheckItem(HoeEditor::ID_VIEWFULLSCREEN, _("F&ull Screen\tF12"), _("Switch to fullscreen."));
	menuView->AppendSeparator();
	// menu voleb zobrazeni

	menuView->AppendCheckItem(ID_TERRAINWIRE, _("Terrain Wireframe\tCtrl+W"), _("Switch terrain in wireframe."));
	wxMenu * omenu = new wxMenu;
	omenu->AppendRadioItem(ID_OBJECTSWIRE, _("Wire"), _("Show wireframe objects."));
	omenu->AppendRadioItem(ID_OBJECTSHIDE, _("Hidden"), _("Hide objects."));
	omenu->AppendRadioItem(ID_OBJECTSFULL, _("Full"), _("Show normaly."));
	menuView->Append(ID_OBJECTS, _("Objects"), omenu, _("Show objects."));
	menuView->AppendCheckItem(ID_SYSOBJECTS, _("System objects"), _("Show system objects."));

	omenu->Check(ID_OBJECTSFULL, true);
	menuView->Check(ID_SYSOBJECTS, true);
	/*menuView->AppendSeparator();
	// advanced
	wxMenu * menuAdvanced = new wxMenu;
	menuAdvanced->Append(ID_ADV_WHITESPACE, _("View &White Space"), _("View white space in code editor."));
    menuView->Append(-1, _("Ad&vanced"), menuAdvanced);
	*/
	// tools
	wxMenu * m_menuTools = new wxMenu;
	m_menuTools->Append(ID_MAPSETTINGS, _("&Map Settings..."), _("Set map parameters"));
	m_menuTools->Append(ID_TERRAINTEX, _("&Terrain Textures..."), _("Terrain textures editor."));
	m_menuTools->Append(ID_SHOWRES, _("&Resource Manager..."), _("Resource Manager"));
	m_menuTools->AppendSeparator();
	m_menuTools->Append(ID_ENGINECON, _("&Show Console...\tF11"), _("Show Engine Console"));
#ifndef HOE_STATIC
	m_menuTools->Append(HoeEditor::ID_ENGINE, _("&Load Engine...\tF10"), _("Load engine from library"));
#endif

	wxMenu * menuHelp = new wxMenu;
	menuHelp->Append(ID_HELP, _("&Help...\tF1"), _("Show help"));
	menuHelp->AppendSeparator();
	menuHelp->Append(HoeEditor::ID_ABOUT, _("&About..."), _("Show about"));

    // now append the freshly created menu to the menu bar...
    m_menu = new wxMenuBar(/*wxMB_DOCKABLE*/);
    m_menu->Append(m_menuFile, _("&File"));
	m_menu->Append(menuView, _("&View"));
	m_menu->Append(m_menuTools, _("&Tools"));
	m_menu->Append(menuHelp, _("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(m_menu); 

	MenuUpdate();
}

// updejt menu
void BecherEdit::MenuUpdate()
{
	if (m_menu)
	{
		// engine
		m_menu->Enable(HoeEditor::ID_NEW, this->GetEngineView()->IsLoaded());
		m_menu->Enable(HoeEditor::ID_OPEN, this->GetEngineView()->IsLoaded());
		// map
		m_menu->Enable(HoeEditor::ID_SAVE, m_map != NULL);
		m_menu->Enable(HoeEditor::ID_SAVEAS, m_map != NULL);
		m_menu->Enable(ID_TERRAINWIRE, m_map != NULL);
		m_menu->Enable(ID_OBJECTS, m_map != NULL);
		m_menu->Enable(ID_SYSOBJECTS, m_map != NULL);
		m_menu->Enable(ID_MAPSETTINGS, m_map != NULL);
	}
}

HoeGame::ResourceMgr * g_resmgr = NULL;
BecherResources::BecherResources()
{
	assert(g_resmgr == NULL);
	g_resmgr = this;
	// load
	m_mainDir = wxConfigBase::Get()->Read(wxT("/BecherDir"),wxT(""));
	if (!m_mainDir.IsEmpty())
	{
		LoadConf(HoeEditor::FullPath(m_mainDir, wxT("files.txt")));
	}
}

BecherResources::~BecherResources()
{
	g_resmgr = NULL;
	// save
	if (!m_mainDir.IsEmpty())
	{
		wxConfigBase::Get()->Write(wxT("/BecherDir"),m_mainDir);
		//LoadConf(HoeEditor::FullPath(m_mainDir, wxT("files.txt")));
	}
}

IHoe3DEngine * BecherResources::GetEngine()
{
	return HoeEditor::App::Get()->GetEditor()->GetEngineView()->GetEngine();
}

wxString BecherResources::FindId(int id)
{
#define ADD_EDGEID(id,num) case id: return #id;
#define ADD_ID(id) case id: return #id;
	switch (id)
	{
#include "../../scripts/id.txt"
	default:
			return "";
	};
#undef ADD_ID
#undef ADD_EDGEID
}

void BecherEdit::OnResMgr(wxCommandEvent &)
{
	m_res.ShowDlg();
}

void BecherEdit::OnMapSettings(wxCommandEvent &)
{
	assert(m_map);
	MapSettingsDialog dlg(this, m_map);
	if (dlg.ShowModal() == wxID_OK)
	{
		dlg.ApplyChanges(m_map);
	}
}

void BecherEdit::OnNewFile(wxCommandEvent &)
{
	MapSettingsDialog dlg(this,NULL);
	if (dlg.ShowModal() == wxID_OK)
	{
        // zjisti z dlg velikost
           
		CloseMap();
		m_map = new EditorMap();
		m_map->CreateNew(dlg.m_width->GetValue(),dlg.m_height->GetValue());
		UpdateControls();
		MenuUpdate();
	}
}

void BecherEdit::OnOpenFile(wxCommandEvent &)
{
	wxFileDialog dlg(this,_("Choose a map file..."),
		_T(""), _T(""), _("Becher Map(*.bm)|*.bm|All Files(*.*)|*.*"), 
		wxOPEN | wxHIDE_READONLY | wxFILE_MUST_EXIST); 
	if (dlg.ShowModal() == wxID_OK)
	{
		if (m_map == NULL)
			m_map = new EditorMap();
		m_map->SetFilePath(dlg.GetPath());
		if (!m_map->LoadMap(dlg.GetPath()))
		{
			wxLogMessage(_("Open map file %s failed."), dlg.GetPath().c_str());
			CloseMap();
			MenuUpdate();
		}
		else
		{
			//m_mapfilepath = dlg.GetPath();
			SetTitle(m_map->GetTitle());
			GetEngine()->SetActiveScene(m_map->GetScene());
			HoeGetRef(GetEngine())->SetBackgroundColor(0xffb060ff);
			MenuUpdate();
		}
	}
	
	UpdateControls();
}

void BecherEdit::OnSaveFile(wxCommandEvent &e)
{
	if (!m_map)
		return;

	if (m_map->GetFilePath().IsEmpty() || e.GetId() == HoeEditor::ID_SAVEAS)
	{
		wxFileDialog dlg( this, _("Save map file..."),
			_T(""), _T(""), _("Becher Map(*.bm)|*.bm"), wxSAVE | wxHIDE_READONLY);
nq:
		if (dlg.ShowModal() == wxID_OK)
		{
			// todo: prepsat?
			if (wxFile::Exists(dlg.GetPath()))
			{
				wxString str;
				str = wxString::Format(_("File %s exist, overwrite?"),dlg.GetPath().c_str());
				int q = wxMessageBox(str, _("Question"),
                              wxICON_QUESTION | wxYES_NO | wxCANCEL);
				if (q == wxNO)
					goto nq;
				if (q == wxCANCEL)
					return;
			}

			m_map->SetFilePath(dlg.GetPath());
		}
		else
			return;
	}

	if (m_map->SaveMap())
		SetTitle(m_map->GetTitle());
}

void BecherEdit::OnHelp(wxCommandEvent &)
{
	// help
	//m_msHtmlHelp.KeywordSearch("neco");
	m_help.DisplaySection(1);
}

void BecherEdit::OnAbout(wxCommandEvent &)
{
	AboutDlg dlg(this);
	dlg.ShowModal();
}

void BecherEdit::OnTypeShow(wxCommandEvent &)
{
	if (m_map)
	{
		m_map->GetTerrain()->ShowWireframe(m_menu->IsChecked(ID_TERRAINWIRE));
		if (m_menu->IsChecked(ID_OBJECTSHIDE))
			m_map->ShowObjects(false, false);
		else if (m_menu->IsChecked(ID_OBJECTSWIRE))
			m_map->ShowObjects(true, true);
		else if (m_menu->IsChecked(ID_OBJECTSFULL))
			m_map->ShowObjects(true, false);
		// zobrazit, hiddnout systemove objekty
		m_map->ShowSystemObjects(m_menu->IsChecked(ID_SYSOBJECTS));

	}

}

void BecherEdit::OnTerrainTextures(wxCommandEvent &)
{
	// textures dialog
	TexturesDialog dlg(this);
	dlg.ShowModal();
}

void BecherEdit::OnNewObject(wxCommandEvent &)
{
}

/////////////////////////////////////
void BecherEdit::UpdateControls()
{
	
}

void BecherEdit::CloseMap()
{
	SAFE_DELETE(m_map);
	//m_mapfilepath = wxT("");
}

/////////////////////////////////////
void BecherEdit::MouseEnter(int absX, int absY)
{
	if (m_tool)
		m_tool->Enter(absX, absY);
}

void BecherEdit::MouseLeave()
{
	if (m_tool)
		m_tool->Leave();
}

void BecherEdit::MouseMove(int relX, int relY, int absX, int absY, const wxMouseEvent & ev)
{
	if (m_tool)
		m_tool->Move(relX, relY, absX, absY, ev);
}

void BecherEdit::MouseLeftDown(const int x, const int y, wxMouseEvent & e)
{
	if (m_tool)
		m_tool->LeftDown(x,y,e);
}

void BecherEdit::MouseLeftUp(const int x, const int y, wxMouseEvent & e)
{
	if (m_tool)
		m_tool->LeftUp(x,y,e);
}

void BecherEdit::MouseRightDown(const int x, const int y, wxMouseEvent & e)
{
	if (m_tool)
		m_tool->RightDown(x,y,e);
}

void BecherEdit::MouseWheel(wxMouseEvent & e)
{
	if (m_map)
	{
		if (e.ControlDown() && m_tool)
		{
			/*BecherObject * o = m_map->GetObject(e.GetX(),e.GetY());
			if (o)
				o->SetAngle(o->GetAngle() + e.GetWheelRotation() / 500.f);
				*/
			m_tool->Wheel(e);
		}
		else if (e.ShiftDown())
			m_map->GetView()->Zoom(e.GetWheelRotation() / 50.f);
		else
			m_map->GetView()->Rotate(e.GetWheelRotation() / 500.f);
	}
		
}

void BecherEdit::KeyDown(wxKeyEvent& e)
{
	if (m_map)
	{
		if (e.ControlDown())
		{
			switch (e.GetKeyCode())
			{
			case WXK_LEFT:
				m_map->GetView()->Rotate(0.1f);
				break;
			case WXK_RIGHT:
				m_map->GetView()->Rotate(-0.1f);
				break;
			case WXK_UP:
				m_map->GetView()->Zoom(-1.7f);
				break;
			case WXK_DOWN:
				m_map->GetView()->Zoom(1.7f);
				break;

			};
		}
		else
		{
			switch (e.GetKeyCode())
			{
			case WXK_LEFT:
				m_map->GetView()->Move(0,-1);
				break;
			case WXK_RIGHT:
				m_map->GetView()->Move(0,1);
				break;
			case WXK_UP:
				m_map->GetView()->Move(1,0);
				break;
			case WXK_DOWN:
				m_map->GetView()->Move(-1,0);
				break;

			};
		}			
	}
}

void BecherEdit::KeyUp(wxKeyEvent& e)
{

}

void BecherEdit::SetTool(EditorTool *tool)
{
	if (m_tool)
		m_tool->Exit();
	if (tool)
		m_tool = tool;
	else
		m_tool = new ToolSelect();
}

void BecherEdit::SetStatus(const wxString &str)
{
	SetStatusText(str, 0);
}

//////////////////////////////////
EditBaseConsole * GetCon()
{
	return HoeEditor::App::Get()->GetConsole();
}

HoeEditor::PropertyGrid * GetProp()
{
	return reinterpret_cast<BecherEdit*>(HoeEditor::App::Get()->GetEditor())->GetProp();
}

