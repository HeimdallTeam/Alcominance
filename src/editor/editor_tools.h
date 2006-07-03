
#ifndef _BECHER_EDITOR_TOOLS_H_
#define _BECHER_EDITOR_TOOLS_H_

class BecherObject;

class EditorTool
{
public:
	EditorTool() {}
	virtual ~EditorTool() {}

	virtual void LeftDown(const int x, const int y, wxMouseEvent & e) {}
	virtual void LeftUp(const int x, const int y, wxMouseEvent & e) {}
	virtual void RightDown(const int x, const int y, wxMouseEvent & e) {}
	virtual void RightUp(const int x, const int y, wxMouseEvent & e) {}
	virtual void Wheel(wxMouseEvent & e) {}
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
	virtual void LeftDown(const int x, const int y, wxMouseEvent & e);
};

class ToolCreateObject : public EditorTool
{
protected:
	BecherObject * m_obj;
	long m_type;
public:
	ToolCreateObject(long type);
	virtual ~ToolCreateObject();
	void SetPos(int absX, int absY);

	virtual void LeftDown(const int x, const int y, wxMouseEvent & e);
	virtual void LeftUp(const int x, const int y, wxMouseEvent & e);
	virtual void RightDown(const int x, const int y, wxMouseEvent & e);
	virtual void RightUp(const int x, const int y, wxMouseEvent & e) {};
	virtual void Wheel(wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);
	virtual void Enter(int absX, int absY);
	virtual void Leave();
};

class ToolTerrain : public EditorTool
{
public:
	virtual void LeftDown(const int x, const int y, wxMouseEvent & e);
	virtual void RightDown(const int x, const int y, wxMouseEvent & e);

};

#endif // _BECHER_EDITOR_TOOLS_H_

