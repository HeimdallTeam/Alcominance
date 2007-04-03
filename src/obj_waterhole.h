
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

	virtual int StatusPlace(float *pos);
public:
	WaterHole(IHoeScene * scn);

	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    virtual void Update(const float t);

	DECLARE_BUILDING(EBO_WaterHole)
};


#endif // _BECHER_WATERHOLE_H_

