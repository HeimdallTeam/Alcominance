
#ifndef _BECHER_OBJECT_STORE_H_
#define _BECHER_OBJECT_STORE_H_

#include "buildings.h"

class Store : public BecherBuilding
{
protected:
	ResourceExp m_stone;
	ResourceExp m_wood;
	ResourceExp m_sugar;
	ResourceExp m_water;
	ResourceExp m_becher;
	ResourceExp m_alcohol;
	ResourceExp m_cane;
	ResourceExp * EBSToPointer(ESurType type);
public:
	Store(IHoeScene * scn);
	//virtual ~Store();
	virtual bool Save(BecherGameSave &w);
	virtual bool Load(BecherGameLoad &r);

	int GetStatus(ESurType type);

	virtual void AdvPaint(class IHoePaint3D * advpaint);

	DECLARE_BUILDING(EBO_Store)
};


#endif // _BECHER_OBJECT_STORE_H_

