
#ifndef _BG_CONSOLE_H_
#define _BG_CONSOLE_H_

#ifdef BECHER_PLUGIN

class BGConsole : public HoeGame::BaseConsole
{
public:
	virtual void HOEAPI Con_Print(const char *);
};

#else

class ConsoleDialog : public HoeGame::GuiConsole
{
public:
	ConsoleDialog(HoeGame::Console & con);
	virtual bool Load(IHoe3DEngine *);
};

#endif

#endif // _BG_CONSOLE_H_

