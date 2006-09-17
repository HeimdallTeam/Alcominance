
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

enum EPhaseJob
{
	PhaseStart,
	GoToSource,
	GoToOwner,
	GoToEnd,
	Works,
};

enum EPhaseResult
{
	PhaseEnd,
	PhaseContinue,
	PhaseRepeat
};

class Troll : public BecherObject
{
	//bool MakeStep(Task * j, float step);
	Job m_job; // co ma aktualne na praci
	EPhaseJob m_phase; // faze vykonavane prace
	Path m_path; // jeho cesta

	ESurType m_surtype; // co ma u sebe
	uint m_numsur; // kolik ma u sebe

	EPhaseResult MakePhase(const double t);
public:
	Troll(IHoeScene * scn);
	~Troll();

	virtual EObjType GetType() { return EBO_Troll; }

	virtual void Update(const float t);
	virtual bool Select();

	void SetJob(const Job & j);
	void StopWork();
	
	//Job::Type GetActJob() { return job.type; }
	bool FindJob(BecherBuilding * prior);
};



#endif // _TUPON_H_


