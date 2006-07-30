
#ifndef _BECHER_EDITOR_TOOLS_H_
#define _BECHER_EDITOR_TOOLS_H_

class BecherObject;

class EditorTool
{
public:
	EditorTool() {}
	virtual ~EditorTool() {}

	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e) = 0;
	virtual void LeftUp(const int x, const int y, const wxMouseEvent & e) {}
	virtual void RightDown(const int x, const int y, const wxMouseEvent & e) {}
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
public:
	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
};

enum EObjType;

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
	virtual void LeftUp(const int x, const int y, const wxMouseEvent & e);
	virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	virtual void RightUp(const int x, const int y, const wxMouseEvent & e) {};
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
	byte m_set;
public:
	ToolTerrain(byte set);
	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
	//virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	//virtual void Wheel( const wxMouseEvent & e);
	//virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);

};

#endif // _BECHER_EDITOR_TOOLS_H_

