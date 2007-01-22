
#ifndef _BECHER_OBJECT_STORE_H_
#define _BECHER_OBJECT_STORE_H_

#include "buildings.h"
#include "panels.h"

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
protected:
	// panel
	// trolls
	StoreRes m_res[EBS_Max];
	THoeSub_Model m_info[16];
	static IHoeModel * g_models[EBS_Max];
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
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

    static void LoadModels();
	DECLARE_BUILDING(EBO_Store)
};


#endif // _BECHER_OBJECT_STORE_H_

