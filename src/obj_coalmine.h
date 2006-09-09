
#ifndef _BECHER_COALMINE_H_
#define _BECHER_COALMINE_H_

#include "buildings.h"

class CoalMine;

#ifndef BECHER_EDITOR
class CoalMineStatic : public ObjectHud
{
protected:
	CoalMine * m_act;
public:
	CoalMineStatic();
	void SetAct(CoalMine * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class CoalMine : public SourceBuilding
{
	friend class BecherLevel;
	friend class CoalMineStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static CoalMineStatic m_userhud;
#endif // BECHER_EDITOR
	ResourceExp m_coal;
public:
	CoalMine(IHoeScene * scn);

	DECLARE_BUILDING(EBO_CoalMine)
};


#endif // _BECHER_COALMINE_H_

