
#ifndef _BECHER_OBJECT_DESTILATE_H_
#define _BECHER_OBJECT_DESTILATE_H_

#include "buildings.h"

class Troll;

class Destilate;

#ifndef BECHER_EDITOR
class DestilateStatic : public ObjectHud
{
protected:
	char m_sugarinfo[256];
	char m_alcoinfo[256];
	Destilate * m_act;
public:
	DestilateStatic();
	void SetAct(Destilate * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class Destilate : public FactoryBuilding
{
	friend class BecherLevel;
	friend class DestilateStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static DestilateStatic m_userhud;
#endif // BECHER_EDITOR
	// trolls
	ResourceImp m_sugar;
	ResourceExp m_alco;
	TrollList m_worked;
	THoeSub_Particle m_part;
	HoeGame::StopWatch m_exitdelay;
	inline const int GetMiniStoreCount() const { return m_alco.GetNum() + m_sugar.GetNum(); }
public:
	Destilate(IHoeScene * scn);
	virtual ~Destilate();

	DECLARE_BUILDING(EBO_Destilate)
};

#endif // _BECHER_OBJECT_DESTILATE_H_
