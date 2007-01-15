
#ifndef _BECHER_OBJECT_SUGAR_H_
#define _BECHER_OBJECT_SUGAR_H_

#include "buildings.h"

class Sugar : public BecherBuilding
{
protected:
	// staveni
	Workspace2 m_wbuild;
	ResourceImp m_stone;
	ResourceImp m_wood;
	float m_buildprogress;
	// prace
	ResourceImp m_cane;
	ResourceExp m_sugar;
	inline const int GetMiniStoreCount() const { return m_cane.GetNum() + m_sugar.GetNum(); }
	
	ResourceImp m_coal;
	HoeGame::CTimer m_it;

	THoeSub_Particle m_part;
	//WorkMode m_mode;
	TrollList m_worked;
    int m_wrk_cane;
    int m_wrk_coal;

	HoeGame::StopWatch m_exitdelay;
public:
	Sugar(IHoeScene * scn);
	virtual ~Sugar();

	virtual void SetMode(EBuildingMode mode);
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    void Idiot();
	DECLARE_BUILDING(EBO_Sugar)
};

#endif // _BECHER_OBJECT_SUGAR_H_
