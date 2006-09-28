
#include "../StdAfx.h"
#include "../becher.h"
//#include "../../resource/logo.xpm"
#include "editor_tools.h"
//#include "plugin_map.h"
#include "editor_map.h"
#include "editor.h"
#include "help.h"
#include "../object.h"
#include "map_properties.h"
#include "../terrain.h"

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

BEGIN_EVENT_TABLE(BecherEdit, HoeEditor::BaseEditor)
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
	IDB_OBJECT = 1,
	IDB_OBJECT_MAX = IDB_OBJECT + EBSys_Max,
// system
// textures
	IDB_TEXTURE,
	IDB_TEXTURE_MAX  = IDB_TEXTURE + 50,
	IDB_TERRUP,
	IDB_TERRDOWN,
	IDB_TERRPREV,
};
#define EVT_BUTTON_RANGE(id1, id2, func) wx__DECLARE_EVT2(wxEVT_COMMAND_BUTTON_CLICKED, id1, id2, wxCommandEventHandler(func))


BEGIN_EVENT_TABLE(ToolObjects, wxChoicebook)
    /*EVT_BUTTON(IDB_SUGAR,ToolObjects::OnClick)
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
	EVT_BUTTON(IDB_SAW,ToolObjects::OnClick)*/
	//EVT_BUTTON_RANGE(
	EVT_BUTTON_RANGE(IDB_OBJECT, IDB_OBJECT_MAX, ToolObjects::OnClick)
	// system
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
#include "../../resource/maleikony/drevorubec.xpm"
// system
#include "../../resource/maleikony/sound.xpm"


#define BT_SIZE wxSize(28,28)
#define BT_P(x,y) wxPoint(28 * x + 6, 28 * y + 3)
#define IDB(obj) (IDB_OBJECT+obj)

ToolObjects::ToolObjects(wxWindow * parent)
	: wxChoicebook(parent,wxID_ANY,wxDefaultPosition, wxSize(80,50))
{
	wxWindow * p  = new wxWindow(this,-1);
	wxButton * b = new wxBitmapButton(p,IDB(EBO_Factory),wxBitmap(tovarna_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Factory") );
	b = new wxBitmapButton(p,IDB(EBO_HerbeWoman),wxBitmap(chaloupka_xpm),BT_P(1,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Chalupa") );
	b = new wxBitmapButton(p,IDB(EBO_Farm),wxBitmap(farma_xpm),BT_P(2,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Farm") );
	b = new wxBitmapButton(p,IDB(EBO_Destilate),wxBitmap(lihovar_xpm),BT_P(3,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Destilate") );
	b = new wxBitmapButton(p,IDB(EBO_Sugar),wxBitmap(cukrovar_xpm),BT_P(4,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Sugar") );
	b = new wxBitmapButton(p,IDB(EBO_Store),wxBitmap(sklad_xpm),BT_P(0,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Store") );
	b = new wxBitmapButton(p,IDB(EBO_Shop),wxBitmap(obchod_xpm),BT_P(1,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Shop") );
	b = new wxBitmapButton(p,IDB(EBO_Saw),wxBitmap(drevorubec_xpm),BT_P(2,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Saw") );


	//b->SetForegroundColour(*wxBLUE);
	wxWindow * p2  = new wxWindow(this,-1);
	b = new wxBitmapButton(p2,IDB(EBO_Tree),wxBitmap(stromy_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Tree") );
	b = new wxBitmapButton(p2,IDB(EBO_WaterHole),wxBitmap(studna_xpm),BT_P(1,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Water Hole") );
	b = new wxBitmapButton(p2,IDB(EBO_StoneMine),wxBitmap(studna_xpm),BT_P(2,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Stone Mine") );
	b = new wxBitmapButton(p2,IDB(EBO_CoalMine),wxBitmap(studna_xpm),BT_P(3,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Coan Mine") );
	b = new wxBitmapButton(p2,IDB(EBO_Troll),wxBitmap(studna_xpm),BT_P(0,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Troll") );
	b = new wxBitmapButton(p2,IDB(EBO_Bridge),wxBitmap(most_xpm),BT_P(1,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Bridge") );

	wxWindow * p3  = new wxWindow(this,-1);
	b = new wxBitmapButton(p3,IDB(EBSys_Sound),wxBitmap(Sound_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Sound instance") );
	b = new wxBitmapButton(p3,IDB(EBSys_Place),wxBitmap(Sound_xpm),BT_P(1,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Place instance") );
	b = new wxBitmapButton(p3,IDB(EBSys_Water),wxBitmap(Sound_xpm),BT_P(0,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Water instance") );
	b = new wxBitmapButton(p3,IDB(EBSys_Coal),wxBitmap(Sound_xpm),BT_P(1,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Coal instance") );
	b = new wxBitmapButton(p3,IDB(EBSys_Stone),wxBitmap(Sound_xpm),BT_P(2,1),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Stone instance") );

	AddPage(p2,_("Map elements"));
	AddPage(p,_("Buildings"));
	AddPage(p3,_("System tools"));

}

void ToolObjects::OnClick(wxCommandEvent& event)
{
	if (!BecherEdit::Get()->IsMapLoaded())
	{
		wxMessageBox(_("First must create map."));
		return;
	}

	EObjType type = (EObjType)(event.GetId()-IDB_OBJECT);
	bool rep = false;
	bool rnd = false;
	switch (type)
	{
	case EBO_Tree:
		rep = true;
		rnd = true;
		break;
	};

	ToolCreateObject * t = new ToolCreateObject(type,rep,rnd);
	BecherEdit::Get()->SetTool(t);
}

////////////////////////////////////////////////////
// terrain

BEGIN_EVENT_TABLE(TerrainObject, wxPanel)
	EVT_BUTTON_RANGE(IDB_TEXTURE, IDB_TEXTURE_MAX, TerrainObject::OnTexturesClick)
	// terrain

	EVT_BUTTON(IDB_TERRUP, TerrainObject::OnTerrainClick)
	EVT_BUTTON(IDB_TERRDOWN, TerrainObject::OnTerrainClick)
	EVT_BUTTON(IDB_TERRPREV, TerrainObject::OnTerrainClick)
END_EVENT_TABLE()

#include "../../resource/maleikony/e_terrup.xpm"
#include "../../resource/maleikony/e_terrdown.xpm"
#include "../../resource/maleikony/e_terrprevis.xpm"


TerrainObject::TerrainObject(wxWindow * parent)
	: wxPanel(parent,wxID_ANY,wxDefaultPosition, wxSize(80,250))
{
#undef BT_P
#define BT_P(x,y) wxPoint(28 * x + 12, 28 * y + 15)

	wxButton * b;
	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
	wxStaticBox * sb = new wxStaticBox(this, -1, _("Terrain"), wxPoint(10,10),
                wxSize(140,80));
	b = new wxBitmapButton(sb,IDB_TERRDOWN,wxBitmap(e_terrdown_xpm),BT_P(0,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Terrain down") );
	b = new wxBitmapButton(sb,IDB_TERRUP,wxBitmap(e_terrup_xpm),BT_P(1,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Terrain up") );
	b = new wxBitmapButton(sb,IDB_TERRPREV,wxBitmap(e_terrprevis_xpm),BT_P(2,0),BT_SIZE/*,BS_FLAT*/);
	b->SetToolTip( _("Terrain") );
	topsizer->Add( sb);

	sb = new wxStaticBox(this, -1, _("Textures"), wxPoint(10,100),
                wxSize(140,80));

	// texturyyyyyy
	// pridavat dokud se muze
	int posx = 0,posy = 0;
	for (int i=0;g_ter_surfaces[i].tex_name;i++)
	{
		if (g_ter_surfaces[i].icon == NULL)
			continue;
		b = new wxBitmapButton(sb,IDB_TEXTURE+i,
			wxBitmap(g_ter_surfaces[i].icon),BT_P(posx,posy),BT_SIZE/*,BS_FLAT*/);
		if (g_ter_surfaces[i].tt)
			b->SetToolTip( _(g_ter_surfaces[i].tt) );
		posx++;
		if (posx == 4)
		{
			posy++; posx = 0;
		}
	}
	topsizer->Add( sb);//,
                //long style = 0,
                //const wxString& name = wxStaticBoxNameStr)
}

void TerrainObject::OnTexturesClick(wxCommandEvent& e)
{
	if (!BecherEdit::Get()->IsMapLoaded())
	{
		wxMessageBox(_("First must create map."));
		return;
	}
	BecherEdit::Get()->SetTool(new ToolTex(e.GetId()-IDB_TEXTURE));
	//EditorMap::Get()->GetTerrain()->MoveHeight(0,0,50,-5);
}

void TerrainObject::OnTerrainClick(wxCommandEvent& e)
{
	if (!BecherEdit::Get()->IsMapLoaded())
	{
		wxMessageBox(_("First must create map."));
		return;
	}
	switch (e.GetId())
	{
	case IDB_TERRUP:
		BecherEdit::Get()->SetTool(new ToolTerrain(3.f,30.f));
		break;
	case IDB_TERRDOWN:
		BecherEdit::Get()->SetTool(new ToolTerrain(-3.f,30.f));
		break;
	case IDB_TERRPREV:
		BecherEdit::Get()->SetTool(new ToolTerrainExp());
		break;

	};
	// pokus o nasazeni modelu
	
	//BecherEdit::Get()->SetTool();
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

bool BecherEditApp::OnPostInit()
{
	//m_splash->
	//delete m_splash;
	return false;
}

///////////////////////////
 BecherEdit * BecherEdit::s_actinstance = NULL;

BecherEdit::BecherEdit()
{
	assert(s_actinstance == NULL);
	s_actinstance = this;
	m_map = NULL;
}

BecherEdit::~BecherEdit()
{
	CloseMap();
	assert(s_actinstance == this);
	s_actinstance = NULL;
}

bool BecherEdit::OnPostInit()
{
	wxString eng = 	wxConfigBase::Get()->Read(wxT("/engine"),wxT(""));
	if (eng != "")
	{
		LoadEngine(eng);
	}
	m_res.LoadMainDir(wxConfigBase::Get()->Read(wxT("/ResourceDir"),wxT("")));

	return false;
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
	HoeEditor::BaseEditor::Create(title);

    // file
    wxMenu * menuFile = new wxMenu;
	menuFile->Append(HoeEditor::ID_NEW, _("&New...\tCtrl+N"), _("Creates a new file or project."));
	menuFile->Append(HoeEditor::ID_OPEN, _("&Open...\tCtrl+O"), _("Open an existing file."));
	menuFile->AppendSeparator();
	menuFile->Append(HoeEditor::ID_SAVE, _("&Save\tCtrl+S"), _("Save file."));
	menuFile->Append(HoeEditor::ID_SAVEAS, _("Save &As..."), _("Save file."));
	menuFile->AppendSeparator();
	menuFile->Append(HoeEditor::ID_QUIT, _("E&xit\tAlt-F4"), _("Quit this program")); 

	// edit
    /*wxMenu * menuEdit = new wxMenu;
	menuEdit->Append(wxID_ANY, _("Undo\tCtrl+Z"), _("Undo last action."));
	menuEdit->Append(wxID_ANY, _("Redo\tCtrl+Y"), _("Redo last action."));
	*/

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
	wxMenu * menuTools = new wxMenu;
	menuTools->Append(ID_MAPSETTINGS, _("&Map Settings..."), _("Set map parameters"));
	menuTools->Append(ID_TERRAINTEX, _("&Terrain Textures..."), _("Terrain textures editor."));
	menuTools->Append(ID_SHOWRES, _("&Resource Manager..."), _("Resource Manager"));
	menuTools->AppendSeparator();
	menuTools->Append(ID_ENGINECON, _("&Show Console...\tF11"), _("Show Engine Console"));
#ifndef HOE_STATIC
	menuTools->Append(HoeEditor::ID_ENGINE, _("&Load Engine...\tF10"), _("Load engine from library"));
#endif

	wxMenu * menuHelp = new wxMenu;
	menuHelp->Append(ID_HELP, _("&Help...\tF1"), _("Show help"));
	menuHelp->AppendSeparator();
	menuHelp->Append(HoeEditor::ID_ABOUT, _("&About..."), _("Show about"));

    // now append the freshly created menu to the menu bar...
    m_menu = new wxMenuBar(/*wxMB_DOCKABLE*/);
    m_menu->Append(menuFile, _("&File"));
    //m_menu->Append(menuEdit, _("&Edit"));
	m_menu->Append(menuView, _("&View"));
	m_menu->Append(menuTools, _("&Tools"));
	m_menu->Append(menuHelp, _("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(m_menu); 

	wxSplitterWindow * split = new wxSplitterWindow(this,10, wxDefaultPosition, wxDefaultSize,0);
	split->SetSashGravity(0);
	
	// seradit do layoutu

	this->m_leftpanel.Create(split, -1);
	this->m_engview.Create(split, -1, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);

	split->SplitVertically(&m_leftpanel, &m_engview, 150);

	m_prop = new HoeEditor::PropertyGrid();
	m_prop->Create(GetPanelMgr());
	GetPanelMgr()->AddPanel(
		new ToolObjects(GetPanelMgr()), _("Tools"), true, true);
	GetPanelMgr()->AddPanel(
		new TerrainObject(GetPanelMgr()), _("Terrain"), true, true);
	GetPanelMgr()->AddPanel(
		m_prop, _("Properties"), false, true);


	UpdateControls();

	// create status bar
	wxStatusBar *statbar = CreateStatusBar();
	statbar->SetFieldsCount(2);

	UpdateControls();
	return true;
}

// updejt menu
void BecherEdit::UpdateControls()
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

wxString BecherResources::FindId(int id)
{
#define ADDID(id) case id: return #id;
	switch (id)
	{
#include "../../scripts/id.txt"
	default:
			return "";
	};
#undef ADDID
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
			UpdateControls();
		}
		else
		{
			//m_mapfilepath = dlg.GetPath();
			SetTitle(m_map->GetTitle());
			GetEngine()->SetActiveScene(m_map->GetScene());
			HoeGetRef(GetEngine())->SetBackgroundColor(0xffb060ff);
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

void BecherEdit::CloseMap()
{
	SAFE_DELETE(m_map);
	//m_mapfilepath = wxT("");
}

const float move_speed = 8.f;

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
				m_map->GetView()->Zoom(-3.7f);
				break;
			case WXK_DOWN:
				m_map->GetView()->Zoom(3.7f);
				break;

			};
		}
		else
		{
			switch (e.GetKeyCode())
			{
			case WXK_LEFT:
				m_map->GetView()->Move(0,-move_speed);
				break;
			case WXK_RIGHT:
				m_map->GetView()->Move(0,move_speed);
				break;
			case WXK_UP:
				m_map->GetView()->Move(move_speed,0);
				break;
			case WXK_DOWN:
				m_map->GetView()->Move(-move_speed,0);
				break;

			};
		}			
	}
}

void BecherEdit::KeyUp(wxKeyEvent& e)
{
	if (e.GetKeyCode() == WXK_DELETE)
		m_map->DeleteSelect();

}

void BecherEdit::SetStatus(const wxString &str)
{
	SetStatusText(str, 0);
}

void BecherEdit::OnDefaultTool()
{
	SetTool(new ToolSelect());
}

//////////////////////////////////
EditBaseConsole * GetCon()
{
	return HoeEditor::App::Get()->GetConsole();
}



