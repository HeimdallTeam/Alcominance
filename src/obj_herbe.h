
#ifndef _BECHER_HERBE_H_
#define _BECHER_HERBE_H_

#include "buildings.h"

class BabaJaga : public HoeGame::BaseObject
{
public:
	BabaJaga(IHoeScene * scn);
	void Update(const float dtime);
};

class HerbeWoman : public ProductionBuilding
{
protected:
	// panel
	THoeSub_Model m_info;
	ResourceExp m_herbe;
	float m_wait;
	bool m_ocup;
	BabaJaga m_jaga;
public:
	HerbeWoman(IHoeScene * scn);
	virtual void OnUpdateSur();

	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    virtual void Update(const float t);
	DECLARE_BUILDING(EBO_HerbeWoman)
};


#endif // _BECHER_HERBE_H_

