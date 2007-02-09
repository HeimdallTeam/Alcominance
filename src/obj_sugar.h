
#ifndef _BECHER_OBJECT_SUGAR_H_
#define _BECHER_OBJECT_SUGAR_H_

#include "buildings.h"

class Sugar : public FactoryBuilding
{
protected:
	// prace
	ResourceImp m_cane;
	ResourceExp m_sugar;
	inline const int GetMiniStoreCount() const { return m_cane.GetNum() + m_sugar.GetNum(); }
	
	HoeGame::CTimer m_it;
	THoeSub_Particle m_part;
	//WorkMode m_mode;
public:
	Sugar(IHoeScene * scn);
	virtual ~Sugar();

	virtual void SetMode(EBuildingMode mode);
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    void Idiot();
    virtual void Update(const float t);
	DECLARE_BUILDING(EBO_Sugar)
};

#endif // _BECHER_OBJECT_SUGAR_H_
