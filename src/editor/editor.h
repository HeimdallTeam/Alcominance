
/**
   @file    editor.h
   @date    Mar 2006
   @version 1.0
   @brief   Soubor s definici hlavnich trid editoru.
*/

#ifndef _BECHER_EDITOR_H_
#define _BECHER_EDITOR_H_

class EditorMap;

class BecherResources : public HoeEditor::Resources
{
public:
	BecherResources();
	virtual ~BecherResources();
	virtual IHoe3DEngine * GetEngine();
	virtual wxString FindId(int id);
};

class TerrainTexture
{
public:

};

class TerrainTextures
{
public:
};

class BecherEdit : public HoeEditor::LevelEditor
{
protected:
	TerrainTextures m_tex;
	BecherResources m_res;
	EditorMap *m_map;
	HoeEditor::PropertyGrid * m_prop;
	HoeEditor::EngineView m_engview;
	HoeEditor::PanelMgr m_leftpanel;
	wxHelpController m_help;
public:
	BecherEdit();
	virtual ~BecherEdit();
	virtual void OnInitMenu();
	virtual XHoeFS * GetFS() { return &m_res; }
	HoeEditor::PropertyGrid * GetProp() { return m_prop; }
	virtual HoeEditor::EngineView * GetEngineView() { return &m_engview; }
	virtual HoeEditor::PanelMgr * GetPanelMgr() { return &m_leftpanel; }

	bool Create(const wxString & title);
	void OnNewObject(wxCommandEvent &);
	void OnResMgr(wxCommandEvent &);
	void OnMapSettings(wxCommandEvent &);
	void OnNewFile(wxCommandEvent &);
	void OnOpenFile(wxCommandEvent &);
	void OnSaveFile(wxCommandEvent &);
	void OnHelp(wxCommandEvent &);
	void OnAbout(wxCommandEvent &);
	void OnTerrainWireFrame(wxCommandEvent &);
	void OnTerrainTextures(wxCommandEvent &);

	virtual void KeyDown(wxKeyEvent& event);
	virtual void KeyUp(wxKeyEvent& event);
	virtual void MouseLeftDown(const int x, const int y, wxMouseEvent & e);
	virtual void MouseLeftUp(const int x, const int y, wxMouseEvent & e);
	virtual void MouseRightDown(const int x, const int y, wxMouseEvent & e);
	virtual void MouseRightUp(const int x, const int y, wxMouseEvent & e) {};
	virtual void MouseWheel(wxMouseEvent & e);
	virtual void MouseMove(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);
	virtual void MouseEnter(int absX, int absY);
	virtual void MouseLeave();

	void CloseMap();
	void UpdateControls();

    DECLARE_EVENT_TABLE()
};

class BecherEditApp : public HoeEditor::App
{
	wxSplashScreen *m_splash;
	wxLocale m_locale;
public:
	virtual HoeEditor::BaseEditor * CreateEditor();
	virtual void OnPostInit();
};

#endif // _BECHER_EDITOR_H_

