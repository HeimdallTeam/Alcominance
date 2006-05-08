
#include "StdAfx.h"
#include "../../HoeGame/hoe_config.h"
#ifdef _WIN32
#include "resource.h"
#endif

#define CONFIG_FILE "becher.conf"

class BecherConfig : public HoeGame::ConfigVars
{
	bool m_continue;
	bool m_showdlg;
	static CVar m_config;
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

CVar BecherConfig::m_config("config_file", CONFIG_FILE, TVAR_SSTR);

#ifdef _WIN32

bool BecherConfig::ShowConfig(HINSTANCE hInst)
{
	//HWND wnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_SETTDLG), GetDesktopWindow(), NULL);
	if (DialogBox(hInst,MAKEINTRESOURCE(IDD_SETTDLG), GetDesktopWindow(), DialogProc) == 0)
		return false;
	// zapsat zmeny
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
		case IDOK:
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

	if (Load(m_config.GetString())!=0 || m_showdlg)
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
	Load(m_config.GetString());

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
	return 1;
}

