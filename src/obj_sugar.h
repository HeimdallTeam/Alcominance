
#ifndef _BECHER_OBJECT_SUGAR_H_
#define _BECHER_OBJECT_SUGAR_H_

#include "buildings.h"

class Sugar : public FactoryBuilding, TrollList
{
protected:
	// trolls
	int m_cane;
	int m_sugar;
	//WorkMode m_mode;
	WorkT<4> m_worked;
public:
	Sugar(IHoeScene * scn);
	virtual ~Sugar();
	void SetWork(Troll *);
	void SetOut(Troll *t, Store *s );
	void SetIn(Troll *t, Store * s);

#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
#endif

	DECLARE_BUILDING(EBO_Sugar)
};

#endif // _BECHER_OBJECT_SUGAR_H_
