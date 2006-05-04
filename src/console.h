
#ifndef _BG_CONSOLE_H_
#define _BG_CONSOLE_H_

#ifdef BECHER_PLUGIN

class BGConsole : public HoeGame::BaseConsole
{
public:
	virtual void HOEAPI Con_Print(const char *);
};

#else

class BGConsole : public HoeGame::GuiConsole
{
public:
	BGConsole();
	virtual bool Load(IHoe3DEngine *);
};

#endif

#endif // _BG_CONSOLE_H_

