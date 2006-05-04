
#ifndef _BECHER_OBJECT_FARM_H_
#define _BECHER_OBJECT_FARM_H_

#include "buildings.h"

class Troll;

class Farm : public BecherBuilding
{
	float rust;
	int skliz;
	Troll * worked;
public:
	Farm(IHoeScene * scn);

	DECLARE_BUILDING(EBO_Farm)

};


#endif // _BECHER_OBJECT_FARM_H_
