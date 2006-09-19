
#ifndef _TUPON_H_
#define _TUPON_H_

#include "object.h"
#include "jobs.h"

class Path
{
	float x,y;
public:
	bool GetNextPos(float l,float &px, float &py);
	bool Step(Troll * t, const float time);
	void SetPosTo(float X, float Y);
	void SetPosTo(BecherObject * bo);
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
	Path m_path; // jeho cesta
	TLoad m_load; // naklad (co ma u sebe)
	void ToBuilding();
	void LeaveBuilding();
public:
	Troll(IHoeScene * scn);
	~Troll();

	virtual EObjType GetType() { return EBO_Troll; }

	virtual void Update(const float t);
	virtual bool Select();

	//virtual bool Save(BecherGameSave &w);
	//virtual bool Load(BecherGameLoad &r);

	void SetJob(const TJob & j);
	void SurIn(ESurType type, uint num);
	void Finish();

	// prace s tupounem
	void StopWork();
	bool FindJob(BecherBuilding * prior);
};



#endif // _TUPON_H_


