
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

	//virtual EObjType GetType() { return EBO_StoneMine; } 
    virtual void Update(const float t);
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

	DECLARE_BUILDING(EBO_StoneMine)
};


#endif // _BECHER_STONEMINE_H_

