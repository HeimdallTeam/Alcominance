
#ifndef _BECHER_WATERHOLE_H_
#define _BECHER_WATERHOLE_H_

#include "buildings.h"

class WaterHole;

#ifndef BECHER_EDITOR
class WaterHoleStatic : public ObjectHud
{
protected:
	char m_waterinfo[256];
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
	ResourceExp m_water;
	float m_kap;
	HoeGame::PtrSet<TTrollWorkSlot> m_worked;
public:
	WaterHole(IHoeScene * scn);

#ifndef BECHER_EDITOR
	virtual bool SetToGet(Troll * t, uint num);
#endif // BECHER_EDITOR

	DECLARE_BUILDING(EBO_WaterHole)
};


#endif // _BECHER_WATERHOLE_H_

