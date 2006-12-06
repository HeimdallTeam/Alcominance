
#ifndef _BECHER_EDITOR_TOOLS_H_
#define _BECHER_EDITOR_TOOLS_H_

#include <he/undolist.h>


class BecherObject;

////////////////////////////////////////////////////////////////////////
class BecherUndoAction : public HoeEditor::UndoAction
{
public:
};

class ToolSelect : public HoeEditor::EditorTool
{
protected:
	BecherObject * m_obj;
public:
	ToolSelect();
	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
};

enum EObjType;

/* protiakce na vytvoreni objektu; undo - smazani; redo - znovuvytvoreni */
class UndoCreate : public BecherUndoAction
{
public:
};

class ToolCreateObject : public HoeEditor::EditorTool
{
protected:
	BecherObject * m_obj;
    XHoeObject * m_xobj;
	EObjType m_type;
	bool m_repeat;
	bool m_rand;
public:
	ToolCreateObject(EObjType type, bool repeat, bool randori);
	virtual ~ToolCreateObject();
	void SetPos(int absX, int absY);

	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
	//virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	//virtual void RightUp(const int x, const int y, const wxMouseEvent & e) {};
	virtual void Wheel(const wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);
	virtual void Enter(int absX, int absY);
	virtual void Leave();
};

class ToolTex : public HoeEditor::EditorTool
{
	byte m_tex;
	bool m_leftdown;
	int m_nx,m_ny;
public:
	ToolTex(byte tex);
	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
	virtual void LeftUp(const int x, const int y, const wxMouseEvent & e);
	virtual void Wheel( const wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);

};

class ToolTerrain : public HoeEditor::EditorTool
{
	bool lock;
	int lockx;
	int locky;
	float m_size;
	float m_radius;
public:
	ToolTerrain(float size, float radius);
	virtual ~ToolTerrain();
	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
	virtual void LeftUp(const int x, const int y, const wxMouseEvent & e);
	//virtual void Wheel( const wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);

};

class ToolTerrainExp : public HoeEditor::EditorTool
{
public:
	ToolTerrainExp();
	virtual ~ToolTerrainExp();
	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
	virtual void Wheel( const wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);

};

#endif // _BECHER_EDITOR_TOOLS_H_

