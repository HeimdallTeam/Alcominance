
#ifndef _BECHER_WATERHOLE_H_
#define _BECHER_WATERHOLE_H_

#include "buildings.h"

class WaterHole;

#ifndef BECHER_EDITOR
class WaterHoleStatic : public ObjectHud
{
protected:
	WaterHole * m_act;
public:
	WaterHoleStatic();
	void SetAct(WaterHole * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class WaterHole : public SourceBuilding
{
	friend class BecherLevel;
	friend class WaterHoleStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static WaterHoleStatic m_userhud;
#endif // BECHER_EDITOR
public:
	WaterHole(IHoeScene * scn);

	DECLARE_BUILDING(EBO_WaterHole)
};


#endif // _BECHER_WATERHOLE_H_

