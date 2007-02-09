
#ifndef _BECHER_OBJECT_DESTILATE_H_
#define _BECHER_OBJECT_DESTILATE_H_

#include "buildings.h"

class Troll;

class Destilate : public FactoryBuilding
{
protected:
	// prace
	Workspace2 m_build;
	ResourceImp m_sugar;
	ResourceExp m_alco;
	inline const int GetMiniStoreCount() const { return m_alco.GetNum() + m_sugar.GetNum(); }

	HoeGame::CTimer m_it;
public:
	Destilate(IHoeScene * scn);
	virtual ~Destilate();
	virtual void SetMode(EBuildingMode mode);

	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

    void Idiot();
	DECLARE_BUILDING(EBO_Destilate)
};

#endif // _BECHER_OBJECT_DESTILATE_H_
