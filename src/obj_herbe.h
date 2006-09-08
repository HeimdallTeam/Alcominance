
#ifndef _BECHER_HERBE_H_
#define _BECHER_HERBE_H_

#include "buildings.h"

class HerbeWoman;

#ifndef BECHER_EDITOR
class HerbeWomanStatic : public ObjectHud
{
protected:
	HerbeWoman * m_act;
public:
	HerbeWomanStatic();
	void SetAct(HerbeWoman * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class HerbeWoman : public SourceBuilding
{
	friend class BecherLevel;
	friend class HerbeWomanStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static HerbeWomanStatic m_userhud;
#endif // BECHER_EDITOR
public:
	HerbeWoman(IHoeScene * scn);

	DECLARE_BUILDING(EBO_HerbeWoman)
};


#endif // _BECHER_HERBE_H_

