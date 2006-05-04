
#ifndef _BECHER_OBJECT_STORE_H_
#define _BECHER_OBJECT_STORE_H_

#include "buildings.h"

class Store : public BecherBuilding
{
protected:
	int m_stone;
	int m_wood;
	int m_sugar;
	int m_water;
	int m_becher;
	int m_alcohol;
	int m_cane;
	int * EBSToPointer(ESurType type);
public:
	Store(IHoeScene * scn);
	//virtual ~Store();
	virtual bool Save(HoeFileWriter &w);
	virtual bool Load(int ver, HoeFileReader &r);

	int GetStatus(ESurType type);

	virtual void AdvPaint(class IHoePaint3D * advpaint);
#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
#endif

	DECLARE_BUILDING(EBO_Store)
};


#endif // _BECHER_OBJECT_STORE_H_

