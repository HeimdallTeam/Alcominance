
#ifndef _BECHER_OBJECT_DESTILATE_H_
#define _BECHER_OBJECT_DESTILATE_H_

#include "buildings.h"

class Troll;

class Destilate : public FactoryBuilding
{
protected:
	int m_sugar;
	int m_alco;
	WorkT<4> m_worked;
public:
	Destilate(IHoeScene * scn);
	// virtual ~Destilate();
	void SetWork(Troll *);
	void SetOut(Troll *t, Store *s );
	void SetIn(Troll *t, Store * s);

	DECLARE_BUILDING(EBO_Destilate)
};

#endif // _BECHER_OBJECT_DESTILATE_H_
