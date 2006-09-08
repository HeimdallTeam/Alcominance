
#ifndef _BECHERGAME_ELEMENTS_H_
#define _BECHERGAME_ELEMENTS_H_

#include "object.h"

class Tree : public BecherObject
{
	struct ParamStruct
	{
		int type;
		float height;
		int wood;
	} param;

public:
	Tree(IHoeScene * scn);
	virtual ~Tree();
	virtual EObjType GetClass() { return EBO_Tree; }
	int GetWood() { return param.wood; }
	void SetWood(int w) { param.wood = w; }
	float GetHeight() { return param.height; }
	void SetHeight(float h);
	// nastaveni typu 1-5
	void SetTypeModel(int type);
	int GetTypeModel() { return param.type; }

#ifdef BECHER_EDITOR
	virtual bool Select();
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
#endif

	DECLARE_BASEOBJECT(EBO_Tree)
};

class Bridge : public BecherObject
{
	float m_height;
public:
	Bridge(IHoeScene * scn);
	virtual EObjType GetClass() { return EBO_Bridge; }
#ifdef BECHER_EDITOR
	virtual bool Select();
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
#endif

	DECLARE_BASEOBJECT(EBO_Bridge)
};


#endif // _BECHERGAME_ELEMENTS_H_


