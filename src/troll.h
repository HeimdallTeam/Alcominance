
#ifndef _TUPON_H_
#define _TUPON_H_

#include "object.h"
#include "jobs.h"

/**
 Tupoun je zakladni pracovni sila, ve skutecnosti pracuje jen jako nosic
 prace v jednotlivych budovach je naprogramovana primo v budove

*/

struct TrollPath : public HoeGame::LandPath
{
	Point act;
	bool GetNextPos(float l,float &px, float &py);
	bool Step(Troll * t, const float time);
	//void SetPosTo(float X, float Y);
	// nastavi vnitrni promeny na chozeni, k urcenemu mistu
	bool Go(float tx, float ty);
	bool Go(BecherObject * to);
};

struct TLoad
{
	ESurType surtype; // co ma u sebe
	uint numsur; // kolik ma u sebe
	bool locked;
	uint numlocked;
};

class Troll : public BecherObject
{
	TJob m_job; // co ma aktualne na praci
	TrollPath m_path; // jeho cesta
	TLoad m_load; // naklad (co ma u sebe)
	float anim;
	float m_nextfind;
public:
	Troll(IHoeScene * scn);
	~Troll();

	virtual EObjType GetType() { return EBO_Troll; }

	virtual void Update(const float t);
	virtual bool Select();

	virtual bool Save(BecherGameSave &w);
	virtual bool Load(BecherGameLoad &r);

	void SetJob(const TJob & j);
    const TJob & GetJob() { return m_job; }
	void SurIn(ESurType type, uint num);
	void Finish();

	// prace s tupounem
	void StopWork();
	bool FindJob(BecherBuilding * prior);

	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
};



#endif // _TUPON_H_


