
#ifndef _BECHER_OBJECT_FARM_H_
#define _BECHER_OBJECT_FARM_H_

#include "buildings.h"

class Troll;
class Farm;

#ifndef BECHER_EDITOR
class FarmStatic : public ObjectHud
{
protected:
	//char m_sugarinfo[256];
	char m_produkce[256];
	Farm * m_act;
public:
	FarmStatic();
	void SetAct(Farm * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class Farm : public SourceBuilding
{
	friend class BecherLevel;
	friend class FarmStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static FarmStatic m_userhud;
#endif // BECHER_EDITOR
	THoeSub_Model m_growinfo;
	// parametry pro rust farmy
	float m_grow;
	ResourceExp m_cane;
	Troll * m_work;
public:
	Farm(IHoeScene * scn);
	DECLARE_BUILDING(EBO_Farm)

};


#endif // _BECHER_OBJECT_FARM_H_
