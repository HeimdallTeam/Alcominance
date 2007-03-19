
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

	virtual bool Save(ChunkDictWrite &w);
	virtual bool Load(const ChunkDictRead &r);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

	DECLARE_BASEOBJECT(EBO_Tree)
};

class Bridge : public BecherObject
{
	float m_height;
public:
	Bridge(IHoeScene * scn);
	virtual EObjType GetClass() { return EBO_Bridge; }
	virtual bool Save(ChunkDictWrite &w);
	virtual bool Load(const ChunkDictRead &r);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

	DECLARE_BASEOBJECT(EBO_Bridge)
};

// doplnky, ruzna id - sutriky, sutry, mraveniste ...
#ifdef BECHER_EDITOR

class Addon : public BecherObject
{
protected:
	EObjType m_type;
	float m_scale;
public:
    Addon(IHoeScene * scn, EObjType type);
	virtual EObjType GetType() { return m_type; }
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
	void SetScale(float s);
};

#else

class Addon : public HoeGame::BaseObject
{
public:
    Addon(IHoeScene * scn, EObjType type);
	void SetScale(float s);
};
#endif 

class AddonSmoke : public Addon
{
public:
    AddonSmoke(IHoeScene * scn);

};

#endif // _BECHERGAME_ELEMENTS_H_


