
#ifndef _BECHER_OBJECT_STORE_H_
#define _BECHER_OBJECT_STORE_H_

#include "buildings.h"
#include "panels.h"

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
	char m_coalinfo[256];
	Store * m_act;
	IHoeModel * m_models[EBS_Max];
public:
	StoreStatic();
	void SetAct(Store * act);
	virtual void Draw(IHoe2D * h2d);
	IHoeModel * GetModel(int i) { return m_models[i]; }
	void LoadModels();
};

class StoreRes : public ResourceExp
{
	bool m_enable;
public:
	StoreRes() : ResourceExp(EBS_None) { m_enable = true; }
	void SetType(ESurType type) { m_type = type; }
	bool IsEnable() { return m_enable; }
	void Enable(bool enable) { m_enable = enable; }
};

class Store : public BecherBuilding
{
	friend class BecherLevel;
	friend class StoreStatic;
protected:
	// panel
	static StoreStatic m_storepref;
	// trolls
	StoreRes m_res[EBS_Max];
	THoeSub_Model m_info[16];
	//WorkMode m_mode;
	TrollList m_worked;
	HoeGame::StopWatch m_exitdelay;
	inline const int GetMiniStoreCount() const
	{
		int n = 0;for (int i=1;i<EBS_Max;i++) n+=m_res[i].GetNum();
		return n;
	}
public:
	Store(IHoeScene * scn);
	//virtual ~Store();
	int GetStatus(ESurType type);
	virtual void OnUpdateSur();
	virtual uint AcceptSur(ESurType type);
	virtual bool Load(BecherGameLoad &r);

	DECLARE_BUILDING(EBO_Store)
};


#endif // _BECHER_OBJECT_STORE_H_

