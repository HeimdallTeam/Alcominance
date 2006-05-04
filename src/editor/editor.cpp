
#include "../StdAfx.h"
//#include "../../resource/logo.xpm"
#include "editor_tools.h"
//#include "plugin_map.h"
#include "../id.h"
#include "editor_map.h"
#include "editor.h"
#include "help.h"
#include "../object.h"
#include "../becher.h"

IMPLEMENT_APP(BecherEditApp)

const char * EditorName = "BecherEditor";

enum {
	ID_SHOWRES = HoeEditor::ID_CUSTOMMENU_FIRST,
	ID_OBJECT,
};

BEGIN_EVENT_TABLE(BecherEdit, HoeEditor::LevelEditor)
	EVT_MENU(ID_SHOWRES, BecherEdit::OnResMgr)
	EVT_MENU(HoeEditor::ID_NEW, BecherEdit::OnNewFile)
	EVT_MENU(HoeEditor::ID_OPEN, BecherEdit::OnOpenFile)
	EVT_MENU(HoeEditor::ID_SAVE, BecherEdit::OnSaveFile)
	EVT_MENU(HoeEditor::ID_SAVEAS, BecherEdit::OnSaveFile)
	EVT_MENU(HoeEditor::ID_ABOUT, BecherEdit::OnAbout)
	EVT_MENU_RANGE(ID_OBJECT, ID_OBJECT + EBO_Max, BecherEdit::OnNewObject)

END_EVENT_TABLE()


HoeEditor::BaseEditor * BecherEditApp::CreateEditor()
{
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

BecherEdit::BecherEdit()
{
	m_map = NULL;
}

BecherEdit::~BecherEdit()
{
	CloseMap();
}

bool BecherEdit::Create(const wxString & title)
{
	HoeEditor::LevelEditor::Create(title);

	wxSplitterWindow * split = new wxSplitterWindow(this,10, wxDefaultPosition, wxDefaultSize,0);
	split->SetSashGravity(0);
	
	// seradit do layoutu

	this->m_leftpanel.Create(split, -1);
	this->m_engview.Create(split, -1);

	split->SplitVertically(&m_leftpanel, &m_engview, 150);

	m_prop = new HoeEditor::PropertyGrid(GetPanelMgr());
	GetPanelMgr()->AddPanel(
		new ToolObjects(GetPanelMgr(), 80, 50), _("Tools"), true, true);
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
	menuView->Append(ID_VIEWCODE, _("&Code Editor"), _("Show code editor."));
	menuView->AppendSeparator();*/
	menuView->AppendCheckItem(HoeEditor::ID_VIEWFULLSCREEN, _("F&ull Screen\tF12"), _("Switch to fullscreen."));
	/*menuView->AppendSeparator();
	// advanced
	wxMenu * menuAdvanced = new wxMenu;
	menuAdvanced->Append(ID_ADV_WHITESPACE, _("View &White Space"), _("View white space in code editor."));
    menuView->Append(-1, _("Ad&vanced"), menuAdvanced);
	*/
	// tools
	wxMenu * m_menuTools = new wxMenu;
#ifndef HOE_STATIC
	m_menuTools->Append(HoeEditor::ID_ENGINE, _("&Load Engine...\tF10"), _("Load engine from library"));
	m_menuTools->AppendSeparator();
#endif
	m_menuTools->Append(ID_SHOWRES, _("&Resource Manager..."), _("Resource Manager"));

	wxMenu * menuHelp = new wxMenu;
	menuHelp->Append(HoeEditor::ID_ABOUT, _("&About..."), _("Show about"));

    // now append the freshly created menu to the menu bar...
    m_menu = new wxMenuBar(/*wxMB_DOCKABLE*/);
    m_menu->Append(m_menuFile, _("&File"));
	m_menu->Append(menuView, _("&View"));
	m_menu->Append(m_menuTools, _("&Tools"));
	m_menu->Append(menuHelp, _("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(m_menu); 


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

void BecherEdit::OnNewFile(wxCommandEvent &)
{
	CloseMap();
	m_map = new EditorMap();
	m_map->CreateNew();
	UpdateControls();
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
		HoeEditor::EditorFile file;
		HoeFileReader r(&file, 0);
		if (!file.OpenRead(dlg.GetPath()) || !m_map->Load( r, true))
		{
			wxLogMessage(_("Open map file %s failed."), dlg.GetPath().c_str());
			CloseMap();
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

void BecherEdit::OnAbout(wxCommandEvent &)
{
	AboutDlg dlg;
	dlg.Create(this,-1,"About");
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
	if (m_map && m_map->GetCreatedObject())
	{
		float sx,sy;
		m_map->GetCreatedObject()->Show(true);
		if (m_map->GetView()->GetPick(absX,absY,&sx,&sy))
				m_map->GetCreatedObject()->SetPosition(sx,sy); 
	}
}

void BecherEdit::MouseLeave()
{
	if (m_map && m_map->GetCreatedObject())
	{
		m_map->GetCreatedObject()->Show(false);
		//m_map set pos
	}
}

void BecherEdit::MouseMove(int relX, int relY, int absX, int absY, const wxMouseEvent & ev)
{
	float sx,sy;
	if (m_map && m_map->GetCreatedObject())
	{
		
		if (m_map->GetView()->GetPick(absX,absY,&sx,&sy))
			m_map->GetCreatedObject()->SetPosition(sx,sy); 
	}
	else if (m_map && m_map->m_lockobject)
	{
		if (m_map->GetView()->GetPick(absX,absY,&sx,&sy))
			m_map->m_lockobject->SetPosition(sx,sy); 

	}
}

void BecherEdit::MouseLeftDown(const int x, const int y, wxMouseEvent & e)
{
	if (m_map)
	{
		if (m_map->IsBuildMode())
			m_map->BuildObject();
		else if (e.ControlDown())
		{
			m_map->m_lockobject = m_map->GetObject(x,y);
		}
		else
		{
			m_map->SelectObject(x,y);
		}
	}
}

void BecherEdit::MouseLeftUp(const int x, const int y, wxMouseEvent & e)
{
	m_map->m_lockobject = NULL;
}

void BecherEdit::MouseRightDown(const int x, const int y, wxMouseEvent & e)
{
	if (m_map)
		m_map->LeaveObject();
}

void BecherEdit::MouseWheel(wxMouseEvent & e)
{
	if (m_map)
	{
		if (e.ControlDown())
		{
			BecherObject * o = m_map->GetObject(e.GetX(),e.GetY());
			if (o)
				o->SetAngle(o->GetAngle() + e.GetWheelRotation() / 500.f);
		}
		else if (e.ShiftDown())
			m_map->GetView()->Zoom(e.GetWheelRotation() / 250.f);
		else
			m_map->GetView()->Rotate(e.GetWheelRotation() / 500.f);
	}
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

