
#ifndef _BECHER_OBJECT_SHOP_H_
#define _BECHER_OBJECT_SHOP_H_

#include "buildings.h"

class Troll;

class Shop : public BecherBuilding
{
public:
	Shop(IHoeScene * scn);
	//virtual ~Shop();
    virtual void Update(const float t);

	DECLARE_BUILDING(EBO_Shop)
};

#endif // _BECHER_OBJECT_SHOP_H_
