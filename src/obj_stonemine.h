
#ifndef _BECHER_STONEMINE_H_
#define _BECHER_STONEMINE_H_

#include "buildings.h"

class StoneMine : public SourceBuilding
{
protected:
	// panel
	ResourceExp m_stone;
public:
	StoneMine(IHoeScene * scn);

    virtual void Update(const float t);
    
	DECLARE_BUILDING(EBO_StoneMine)
};


#endif // _BECHER_STONEMINE_H_

