
#ifndef _BECHER_OBJECT_FARM_H_
#define _BECHER_OBJECT_FARM_H_

#include "buildings.h"

class Troll;
class Farm;

class Farm : public ProductionBuilding
{
	friend class BecherLevel;
	friend class FarmStatic;
protected:
	THoeSub_Model m_growinfo;
	// parametry pro rust farmy
	float m_grow;
	ResourceExp m_cane;
	Troll * m_work;
public:
	Farm(IHoeScene * scn);
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    virtual void Update(const float t);

	DECLARE_BUILDING(EBO_Farm)

};


#endif // _BECHER_OBJECT_FARM_H_
