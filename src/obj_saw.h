
#ifndef _BECHER_SAW_H_
#define _BECHER_SAW_H_

#include "buildings.h"

class Saw : public ProductionBuilding
{
protected:
	// panel
public:
	Saw(IHoeScene * scn);

    virtual void Update(const float t);

	DECLARE_BUILDING(EBO_Saw)
};


#endif // _BECHER_SAW_H_

