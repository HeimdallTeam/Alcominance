
#ifndef _BECHER_OBJECT_STORE_H_
#define _BECHER_OBJECT_STORE_H_

#include "buildings.h"

class Store;

class StoreStatic : public ObjectHud
{
protected:
	char m_sugarinfo[256];
	char m_caneinfo[256];
	char m_woodinfo[256];
	char m_stoneinfo[256];
	char m_herbeinfo[256];
	char m_waterinfo[256];
	char m_alcoinfo[256];
	char m_becherinfo[256];
	Store * m_act;
public:
	StoreStatic();
	void SetAct(Store * act);
	virtual void Draw(IHoe2D * h2d);
};

class Store : public BecherBuilding
{
	friend class BecherLevel;
	friend class StoreStatic;
protected:
	// panel
	static StoreStatic m_userhud;
	// trolls
	ResourceExp m_stone;
	ResourceExp m_wood;
	ResourceExp m_sugar;
	ResourceExp m_water;
	ResourceExp m_becher;
	ResourceExp m_alcohol;
	ResourceExp m_cane;
	ResourceExp m_herbe;
	ResourceExp * EBSToPointer(ESurType type);

	//WorkMode m_mode;
	TrollList m_worked;
	HoeGame::StopWatch m_exitdelay;
	inline const int GetMiniStoreCount() const { return m_cane.GetNum() + m_sugar.GetNum(); }
public:
	Store(IHoeScene * scn);
	//virtual ~Store();
	virtual bool Save(BecherGameSave &w);
	virtual bool Load(BecherGameLoad &r);

	int GetStatus(ESurType type);

	virtual void AdvPaint(class IHoePaint3D * advpaint);

	DECLARE_BUILDING(EBO_Store)
};


#endif // _BECHER_OBJECT_STORE_H_

