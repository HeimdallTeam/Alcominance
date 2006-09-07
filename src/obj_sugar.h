
#ifndef _BECHER_OBJECT_SUGAR_H_
#define _BECHER_OBJECT_SUGAR_H_

#include "buildings.h"

class Sugar;

class SugarStatic : public ObjectHud
{
protected:
	char m_sugarinfo[256];
	char m_trtinainfo[256];
	Sugar * m_act;
public:
	SugarStatic();
	void SetAct(Sugar * act);
	virtual void Draw(IHoe2D * h2d);
};

class Sugar : public FactoryBuilding, TrollList
{
	friend class BecherLevel;
	friend class SugarStatic;
protected:
	// panel
	static SugarStatic m_userhud;
	// trolls
	ResourceImp m_cane;
	ResourceExp m_sugar;

	THoeSub_Particle m_part;
	//WorkMode m_mode;
	TrollList m_worked;
	HoeGame::StopWatch m_exitdelay;
	inline const int GetMiniStoreCount() const { return m_cane.GetNum() + m_sugar.GetNum(); }
public:
	Sugar(IHoeScene * scn);
	virtual ~Sugar();

	DECLARE_BUILDING(EBO_Sugar)
};

#endif // _BECHER_OBJECT_SUGAR_H_
