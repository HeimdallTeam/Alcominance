
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
	virtual void LoadAdv(int ver, unsigned long size, HoeFileReader * stream);
	virtual int SaveAdv(void ** buff, unsigned long * size);

	DECLARE_BASEOBJECT(EBO_Tree)
};

class Bridge : public BecherObject
{
public:
	Bridge(IHoeScene * scn);
	virtual EObjType GetClass() { return EBO_Bridge; }

	DECLARE_BASEOBJECT(EBO_Bridge)
};

class WaterHole : public BecherObject
{
public:
	WaterHole(IHoeScene * scn);
	virtual EObjType GetClass() { return EBO_WaterHole; }

#ifndef BECHER_EDITOR
	virtual bool Select();
#endif

	DECLARE_BASEOBJECT(EBO_WaterHole)
};



#endif // _BECHERGAME_ELEMENTS_H_


