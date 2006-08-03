
#ifndef _BECHER_EDITOR_TOOLS_H_
#define _BECHER_EDITOR_TOOLS_H_

class BecherObject;

////////////////////////////////////////////////////////////////////////
class BecherUndoAction : public HoeEditor::UndoAction
{
public:
};

class EditorTool
{
public:
	EditorTool() {}
	virtual ~EditorTool() {}

	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e) = 0;
	virtual void LeftUp(const int x, const int y, const wxMouseEvent & e) {}
	virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	virtual void RightUp(const int x, const int y, const wxMouseEvent & e) {}
	virtual void Wheel(const wxMouseEvent & e) {}
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev) {}
	virtual void Enter(int absX, int absY) {}
	virtual void Leave() {}

	/**
	* Funkce ktera zaridi aby se nastroj "vypnul" a automaticky smaze objekt
	*/
	virtual void Exit() { delete this; }
};

class ToolSelect : public EditorTool
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

class ToolCreateObject : public EditorTool
{
protected:
	BecherObject * m_obj;
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

class ToolTex : public EditorTool
{
	byte m_set;
public:
	ToolTex(byte set);
	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
	virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	virtual void Wheel( const wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);

};

class ToolTerrain : public EditorTool
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
	//virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	//virtual void Wheel( const wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);

};

class ToolTerrainExp : public EditorTool
{
public:
	ToolTerrainExp();
	virtual ~ToolTerrainExp();
	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
	virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	virtual void Wheel( const wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);

};

#endif // _BECHER_EDITOR_TOOLS_H_

