
#ifndef _BECHERGAME_ELEMENTS_H_
#define _BECHERGAME_ELEMENTS_H_

#include "object.h"

class Tree : public BecherObject
{
	int m_type;
	float m_height;
	int m_wood;
public:
	Tree(IHoeScene * scn);
	virtual ~Tree();
	virtual EObjType GetClass() { return EBO_Tree; }
	int GetWood() { return m_wood; }
	void SetWood(int w) { m_wood = w; }
	float GetHeight() { return m_height; }
	void SetHeight(float h);
	// nastaveni typu 1-5
	void SetTypeModel(int type);
	int GetTypeModel() { return m_type; }

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


