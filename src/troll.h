
#ifndef _TUPON_H_
#define _TUPON_H_

#include "object.h"
#include "jobs.h"

class Path
{
	float x,y;
public:
	bool GetNextPos(float l,float &px, float &py);
	void SetPosTo(float X, float Y);
	void SetPosTo(BecherObject * bo);
};

struct JobEx : public Job
{
	Path path;
	enum Phase
	{
		GoTo,
		GoFrom,
		GoToNew,
	} phase;
	JobEx();
	int sur;

	const Job & operator = (const Job & j);
	void SetNone();
	void Go(float x, float y);
	void Work();
	bool Step(Troll * t, const float time);
};


class Troll : public BecherObject
{
	//bool MakeStep(Task * j, float step);
	JobEx job;
public:
	Troll(IHoeScene * scn);
	~Troll();

	virtual EObjType GetType() { return EBO_Troll; }

    virtual void Update(const double t);
	virtual bool Select();
	void StopWork();

	void SetJob(const Job & j);
	void NewJob();

	Job::Type GetActJob() { return job.type; }
}
;


#endif // _TUPON_H_


