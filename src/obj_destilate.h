
#ifndef _BECHER_OBJECT_DESTILATE_H_
#define _BECHER_OBJECT_DESTILATE_H_

#include "buildings.h"

class Troll;

class Destilate : public BecherBuilding
{
	friend class BecherLevel;
protected:
	// trolls
	ResourceImp m_sugar;
    ResourceImp m_coal;
	ResourceExp m_alco;
	TrollList m_worked;
	float m_progress;
    int m_wrk_sugar;    
    int m_wrk_coal;
	THoeSub_Particle m_part;
	HoeGame::StopWatch m_exitdelay;
	inline const int GetMiniStoreCount() const { return m_alco.GetNum() + m_sugar.GetNum(); }
public:
	Destilate(IHoeScene * scn);
	virtual ~Destilate();
	virtual void SetMode(EBuildingMode mode);

	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

	DECLARE_BUILDING(EBO_Destilate)
};

#endif // _BECHER_OBJECT_DESTILATE_H_
