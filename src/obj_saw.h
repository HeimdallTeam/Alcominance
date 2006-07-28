
#ifndef _BECHER_SAW_H_
#define _BECHER_SAW_H_

#include "buildings.h"

class Troll;

class Saw : public BecherBuilding
{
public:
	Saw(IHoeScene * scn);

	DECLARE_BUILDING(EBO_Saw)

};


#endif // _BECHER_SAW_H_

