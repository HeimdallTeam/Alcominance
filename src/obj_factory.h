
#ifndef _BECHER_OBJECT_FACTORY_H_
#define _BECHER_OBJECT_FACTORY_H_

#include "buildings.h"

class Factory;

#ifndef BECHER_EDITOR
class FactoryStatic : public ObjectHud
{
protected:
	char m_sugarinfo[256];
	char m_trtinainfo[256];
	Factory * m_act;
public:
	FactoryStatic();
	void SetAct(Factory * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class Factory : public FactoryBuilding, TrollList
{
	friend class BecherLevel;
	friend class FactoryStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static FactoryStatic m_userhud;
#endif // BECHER_EDITOR
	// trolls
	ResourceImp m_sugar;
	ResourceImp m_alco;
	ResourceImp m_water;
	ResourceImp m_herbe;
	ResourceExp m_becher;
	//WorkMode m_mode;
	TrollList m_worked;
	float m_progress;
	HoeGame::StopWatch m_exitdelay;
	inline const int GetMiniStoreCount() const 
	{ return m_sugar.GetNum()
		+ m_alco.GetNum()
		+ m_herbe.GetNum()
		+ m_water.GetNum()
		+ m_becher.GetNum(); }
public:
	Factory(IHoeScene * scn);
	virtual ~Factory();

	DECLARE_BUILDING(EBO_Factory)
};

#endif // _BECHER_OBJECT_FACTORY_H_
