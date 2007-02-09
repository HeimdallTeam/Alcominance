
#ifndef _BECHER_WATERHOLE_H_
#define _BECHER_WATERHOLE_H_

#include "buildings.h"

class WaterHole : public SourceBuilding
{
protected:
	// panel
	ResourceExp m_water;
	float m_kap;
	HoeCore::Set<TTrollWorkSlot> m_worked;
public:
	WaterHole(IHoeScene * scn);

#ifndef BECHER_EDITOR
	virtual bool SetToGet(Troll * t, uint num);
#endif
    virtual void Update(const float t);

	const char * BuildPlace(float x, float y);
	DECLARE_BUILDING(EBO_WaterHole)
};


#endif // _BECHER_WATERHOLE_H_

