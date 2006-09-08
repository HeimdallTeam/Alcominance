
#ifndef _BECHER_SAW_H_
#define _BECHER_SAW_H_

#include "buildings.h"

class Saw;

#ifndef BECHER_EDITOR
class SawStatic : public ObjectHud
{
protected:
	Saw * m_act;
public:
	SawStatic();
	void SetAct(Saw * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class Saw : public SourceBuilding
{
	friend class BecherLevel;
	friend class SawStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static SawStatic m_userhud;
#endif // BECHER_EDITOR
public:
	Saw(IHoeScene * scn);

	DECLARE_BUILDING(EBO_Saw)
};


#endif // _BECHER_SAW_H_

