
#ifndef _BECHER_OBJECT_SUGAR_H_
#define _BECHER_OBJECT_SUGAR_H_

#include "buildings.h"

class Sugar;

#ifndef BECHER_EDITOR
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
#endif // BECHER_EDITOR

class Sugar : public FactoryBuilding, TrollList
{
	friend class BecherLevel;
	friend class SugarStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static SugarStatic m_userhud;
#endif // BECHER_EDITOR
	// trolls
	ResourceImp m_cane;
	ResourceExp m_sugar;
	float m_progress;

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
