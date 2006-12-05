
#include "StdAfx.h"
#include "../../HoeGame/include/hoe_config.h"
#ifdef _WIN32
#include "resource.h"
#endif

#define CONFIG_FILE "becher.conf"

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
	virtual int Arg(int argc,char * argv[]);
	void ShowUsage(const char * usage);
#ifdef _WIN32
	bool Conf(HINSTANCE hInst);
	bool ShowConfig(HINSTANCE hInst);
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
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


	//HWND wnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_SETTDLG), GetDesktopWindow(), NULL);
	if (DialogBox(hInst,MAKEINTRESOURCE(IDD_SETTDLG), GetDesktopWindow(), DialogProc) == 0)
		return false;
	
	return true;
}

INT_PTR CALLBACK BecherConfig::DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_INITDIALOG:
		// set vars

		AnimateWindow(hwndDlg,200,AW_BLEND);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
//		case IDC_DROP:

//			break;
		case IDOK:
			//v_engine.Set(SendDlgItemMessage(hwndDlg, IDC_ENGINE, CB_GETCURSEL, 0, 0));
			//v_resolution.Set(SendDlgItemMessage(hwndDlg, IDC_RESOLUTION, CB_GETCURSEL, 0, 0));

			AnimateWindow(hwndDlg,200,AW_BLEND|AW_HIDE);
			EndDialog(hwndDlg,1);
			break;
		case IDCANCEL:
			AnimateWindow(hwndDlg,200,AW_SLIDE|AW_HIDE);
			EndDialog(hwndDlg,0);
			break;
		};
		return TRUE;
	};
	return FALSE;
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
		ShowUsage("Becher [-c <config>]");
		m_continue = false;
		return argc;
	}

	// posledni
	if (argc == 1)
	{
		v_level.Set(argv[0]);
	}
	return 1;
}

