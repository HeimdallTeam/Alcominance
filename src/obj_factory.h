
#ifndef _BECHER_OBJECT_FACTORY_H_
#define _BECHER_OBJECT_FACTORY_H_

#include "buildings.h"

class Factory;

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

class Factory : public FactoryBuilding, TrollList
{
	friend class BecherLevel;
	friend class FactoryStatic;
protected:
	// panel
	static FactoryStatic m_userhud;
	// trolls
	ResourceImp m_sugar;
	ResourceImp m_alco;
	ResourceExp m_becher;
	//WorkMode m_mode;
	TrollList m_worked;
	HoeGame::StopWatch m_exitdelay;
	inline const int GetMiniStoreCount() const 
	{ return m_sugar.GetNum() + m_alco.GetNum() + m_becher.GetNum(); }
public:
	Factory(IHoeScene * scn);
	virtual ~Factory();

	DECLARE_BUILDING(EBO_Factory)
};

#endif // _BECHER_OBJECT_FACTORY_H_
