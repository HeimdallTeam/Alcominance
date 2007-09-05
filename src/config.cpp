
#include "StdAfx.h"
#include "../../HoeGame/include/hoe_config.h"
#ifdef _WIN32
#include "resource.h"
#endif
#include "../../HoeGame/include/hoe_wx.h"

#define CONFIG_FILE "becher.conf"

#ifdef _WIN32
class BecherConfigDlg : public HoeGame::wx::Dialog
{
public:
	virtual INT_PTR OnInit() 
	{ 
		AnimateWindow(this->GetHWND(),500,AW_BLEND);
		return TRUE; 
	}
	virtual INT_PTR OnCommand(word cmd) 
	{
		switch (cmd)
		{
		case IDOK:
			//v_engine.Set(SendDlgItemMessage(hwndDlg, IDC_ENGINE, CB_GETCURSEL, 0, 0));
			//v_resolution.Set(SendDlgItemMessage(hwndDlg, IDC_RESOLUTION, CB_GETCURSEL, 0, 0));
			return End(1);
		case IDCANCEL:
			return End(0);
		};
		return FALSE; 
	}
	virtual INT_PTR OnEnd()
	{
		AnimateWindow(this->GetHWND(),700,AW_SLIDE|AW_HIDE);
		return TRUE;
	}
};
#endif

class BecherConfig : public HoeGame::ConfigVars
{
	bool m_continue;
	bool m_showdlg;
public:
	BecherConfig()
	{
		m_continue = true;
		m_showdlg = false;
	}
    /** Automaticke parsovani parametry, 
     * vraci pocet zpracovanych parametru (vola se do zpracovani vsech parametru) */
	virtual int Arg(int argc,char * argv[]);
	void ShowUsage(const char * usage);
#ifdef _WIN32
	bool Conf(HINSTANCE hInst);
	bool ShowConfig(HINSTANCE hInst);
#endif
#ifdef _LINUX
	bool Conf();
#endif
};

static CVar v_config("config_file", CONFIG_FILE, TVAR_SSTR);
static CVar v_lang("lang_file", "czech", TVAR_SSTR);
//CVar BecherConfig::m_engine("config_file", CONFIG_FILE, TVAR_SSTR);
//CVar BecherConfig::m_resolution("config_file", CONFIG_FILE, TVAR_SSTR);

extern HoeGame::CVar v_level;

#ifdef _WIN32

bool BecherConfig::ShowConfig(HINSTANCE hInst)
{
	//@todo zde se zjisti dostupne enginy
	BecherConfigDlg dlg;
	if (dlg.Show(hInst,MAKEINTRESOURCE(IDD_SETTDLG)) == 0)
		return false;

	return true;
}

void BecherConfig::ShowUsage(const char *usage)
{
	MessageBox(GetDesktopWindow(), usage, "Usage:", 0);
}

bool BecherConfig::Conf(HINSTANCE hInstance)
{
	if (!m_continue)
		return false;

	if (Load(v_config.GetString())!=0 || m_showdlg)
	{
		if (!ShowConfig(hInstance))
			return false;
	}

	return m_continue;
}

bool Configure(HINSTANCE hInstance, LPSTR lpStr)
{
	BecherConfig c;
	// dialog
	c.ParseArgs(hInstance, lpStr);
	return c.Conf(hInstance);
}

#endif
#ifdef _LINUX

bool Configure(int argc,char * argv[])
{
	BecherConfig c;
	// dialog
	c.ParseArgs(argc, argv);
	return c.Conf();
}

void BecherConfig::ShowUsage(const char *usage)
{
	printf("Usage %s\n", usage);
}

bool BecherConfig::Conf()
{
	if (!m_continue)
		return false;
	Load(v_config.GetString());

	return m_continue;
}

#endif

int BecherConfig::Arg(int argc, char *argv[])
{
	if (strcmp(argv[0],"-h") == 0)
	{
		ShowUsage("Becher [-conf] [-c <config>] [-d <root>]");
		m_continue = false;
		return argc;
	}

    if (strcmp(argv[0],"-conf") == 0)
    {
        m_showdlg = true;
        // nastaveni na konfig
        return 2;
    }

    if (strcmp(argv[0],"-c") == 0)
    {
        // nastaveni konfigu
        if (argc < 2)
        {
            HoeGame::BaseConsole::Printf("Config file requied.");
            return -1;
        }
        v_config.Set(argv[1]); 
        // nastaveni na konfig
        return 2;
    }

    if (strcmp(argv[0],"-d") == 0)
    {
        // nastaveni konfigu
        if (argc < 2)
        {
            HoeGame::BaseConsole::Printf("Directory requied.");
            return -1;
        }
        HoeGame::BaseConsole::Printf("Changing directory to '%s'", argv[1]);
        // nastaveni cesty
        HoeGame::SetRootDir(argv[1]);
        return 2;
    }
    
	// posledni
	if (argc == 1)
	{
		v_level.Set(argv[0]);
	}
	return 1;
}

