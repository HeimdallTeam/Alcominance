
#ifndef _BECHER_OBJECT_STORE_H_
#define _BECHER_OBJECT_STORE_H_

#include "buildings.h"

class Store : public BecherBuilding
{
protected:
	ResourceItem m_stone;
	ResourceItem m_wood;
	ResourceItem m_sugar;
	ResourceItem m_water;
	ResourceItem m_becher;
	ResourceItem m_alcohol;
	ResourceItem m_cane;
	ResourceItem * EBSToPointer(ESurType type);
public:
	Store(IHoeScene * scn);
	//virtual ~Store();
	virtual bool Save(BecherGameSave &w);
	virtual bool Load(BecherGameLoad &r);

	int GetStatus(ESurType type);

	virtual void AdvPaint(class IHoePaint3D * advpaint);
#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
#endif

	DECLARE_BUILDING(EBO_Store)
};


#endif // _BECHER_OBJECT_STORE_H_

