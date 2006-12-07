
#ifndef _JOBS_H_
#define _JOBS_H_

class BecherBuilding;
class Troll;
class ResourceExp;
class BecherGameSave;
class BecherGameLoad;

struct TJob
{
	enum Type
	{
		jtNone = 0,
		jtGotoRes,
		jtGotoOwnerWithRes,
		jtGotoWork,
		jtWork,
		jtWaitToRes,
		jtFindJob,
		jtFly,
		jtGoto,
	} type;
	// parametry
	BecherBuilding * owner;
	ResourceExp * from;
	BecherBuilding * to;
	ESurType surtype;
	int num;
	float percent;
	bool Save(BecherGameSave &w);
	bool Load(BecherGameLoad &r);

	// function for build job
	void Go(float x, float y);
};


#endif // _JOBS_H_

