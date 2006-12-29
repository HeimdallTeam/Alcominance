
#ifndef _BECHER_HERBE_H_
#define _BECHER_HERBE_H_

#include "buildings.h"

class HerbeWoman;

#ifndef BECHER_EDITOR
class HerbeWomanStatic : public ObjectHud
{
protected:
	HerbeWoman * m_act;
public:
	HerbeWomanStatic();
	void SetAct(HerbeWoman * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class BabaJaga : public HoeGame::BaseObject
{
public:
	BabaJaga(IHoeScene * scn);
	void Update(const float dtime);
};

class HerbeWoman : public SourceBuilding
{
	friend class BecherLevel;
	friend class HerbeWomanStatic;
protected:
	// panel
#ifndef BECHER_EDITOR
	static HerbeWomanStatic m_userhud;
#endif // BECHER_EDITOR
	THoeSub_Model m_info;
	ResourceExp m_herbe;
	float m_wait;
	bool m_ocup;
	BabaJaga m_jaga;
public:
	HerbeWoman(IHoeScene * scn);
	virtual void OnUpdateSur();

	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
	DECLARE_BUILDING(EBO_HerbeWoman)
};


#endif // _BECHER_HERBE_H_

