
#ifndef _BECHER_EDITOR_TOOLS_H_
#define _BECHER_EDITOR_TOOLS_H_

class EditorTool
{
public:
	virtual ~EditorTool() {}
	/**
	* Funkce ktera zaridi aby se nastroj "vypnul" a automaticky se smaze
	*/
	virtual void Exit() { delete this; }
};

class ToolSelect : public EditorTool
{
};

class ToolCreateObject : public EditorTool
{
public:
	ToolCreateObject(long type);
};

#endif // _BECHER_EDITOR_TOOLS_H_

