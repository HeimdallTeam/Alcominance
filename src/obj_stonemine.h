
#ifndef _BECHER_STONEMINE_H_
#define _BECHER_STONEMINE_H_

#include "buildings.h"

class StoneMine;

#ifndef BECHER_EDITOR
class StoneMineStatic : public ObjectHud
{
protected:
	StoneMine * m_act;
public:
	StoneMineStatic();
	void SetAct(StoneMine * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class StoneMine : public SourceBuilding
{
	friend class BecherLevel;
	friend class StoneMineStatic;
protected:
	// panel
	ResourceExp m_stone;
#ifndef BECHER_EDITOR
	static StoneMineStatic m_userhud;
	HoeGame::PtrSet<TTrollWorkSlot> m_worked;
#endif // BECHER_EDITOR
public:
	StoneMine(IHoeScene * scn);

#ifndef BECHER_EDITOR
	virtual bool SetToGet(Troll * t, uint num);
#endif // BECHER_EDITOR

	DECLARE_BUILDING(EBO_StoneMine)
};


#endif // _BECHER_STONEMINE_H_

