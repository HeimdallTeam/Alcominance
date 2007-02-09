
#ifndef _BECHER_STONEMINE_H_
#define _BECHER_STONEMINE_H_

#include "buildings.h"

class StoneMine : public SourceBuilding
{
protected:
	// panel
	ResourceExp m_stone;
#ifndef BECHER_EDITOR
	HoeCore::Set<TTrollWorkSlot> m_worked;
#endif // BECHER_EDITOR
public:
	StoneMine(IHoeScene * scn);

#ifndef BECHER_EDITOR
	virtual bool SetToGet(Troll * t, uint num);
#endif // BECHER_EDITOR

    virtual void Update(const float t);
    
	DECLARE_BUILDING(EBO_StoneMine)
};


#endif // _BECHER_STONEMINE_H_

