
#ifndef _BECHERGAME_SCREENS_H_
#define _BECHERGAME_SCREENS_H_

#include "utils.h"

class BecherLogo : public HoeGame::Scene, public XHoe2DCallback
{
	IHoeFont * font;
	IHoe3DEngine * engine;
	IHoeScene * scene;
public:
	BecherLogo();
	~BecherLogo();
	virtual void OnSet();
	virtual void OnUnset();
	virtual void HOEAPI _Paint(IHoe2D * h2d);
};

class SlideShow : public HoeGame::Scene, public XHoe2DCallback, 
	public HoeGame::Hoe2DFigure,
	public HoeGame::KeyboardStdInput
{
protected:
	HoeGame::LuaScript m_lua;
	IHoeScene * scene;
	bool m_isend;
	static SlideShow * m_instance;
	float m_waittime;
	Fade m_fade;
	IHoeSound * m_sound;
public:
	SlideShow();
	~SlideShow();
	virtual void OnSet();
	virtual void OnUnset();
	virtual void HOEAPI _Paint(IHoe2D * h2d);
	virtual void Update(float dtime);
	virtual void OnKeyDown(int key);
	bool IsEnd() { return m_isend; }
	void Run();

	LUA_FUNCTION(l_wait);
	LUA_FUNCTION(l_settext);
	LUA_FUNCTION(l_loadimage);
	LUA_FUNCTION(l_fade);
};

#endif //_BECHERGAME_SCREENS_H_

