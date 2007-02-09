
#ifndef _BECHER_OBJECT_FACTORY_H_
#define _BECHER_OBJECT_FACTORY_H_

#include "buildings.h"

class Factory : public FactoryBuilding
{
protected:
	// prace
	ResourceImp m_sugar;
	ResourceImp m_alco;
	ResourceImp m_water;
	ResourceImp m_herbe;
	ResourceExp m_becher;
	//WorkMode m_mode;
	inline const int GetMiniStoreCount() const 
	{ return m_sugar.GetNum()
		+ m_alco.GetNum()
		+ m_herbe.GetNum()
		+ m_water.GetNum()
		+ m_becher.GetNum(); }

    HoeGame::CTimer m_it;
public:
	Factory(IHoeScene * scn);
	virtual ~Factory();
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    void Idiot();
    virtual void Update(const float t);
	DECLARE_BUILDING(EBO_Factory)
};

#endif // _BECHER_OBJECT_FACTORY_H_
