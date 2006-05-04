
#ifndef _BECHER_OBJECT_FACTORY_H_
#define _BECHER_OBJECT_FACTORY_H_

#include "buildings.h"

class Troll;

class Factory : public FactoryBuilding
{
protected:
	uint m_sugar;
	uint m_alco;
	uint m_herbe;
	uint m_water;
	uint m_becher;
public:
	Factory(IHoeScene * scn);
	virtual bool Save(HoeFileWriter &w);
	virtual bool Load(int ver, HoeFileReader &r);
	virtual void AdvPaint(IHoePaint3D * h3);

	DECLARE_BUILDING(EBO_Factory)
};

#endif // _BECHER_OBJECT_FACTORY_H_
