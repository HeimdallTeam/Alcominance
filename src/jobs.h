
#ifndef _JOBS_H_
#define _JOBS_H_

class BecherBuilding;
class Troll;
class ResourceExp;

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
	} type;
	// parametry
	BecherBuilding * owner;
	ResourceExp * from;
	BecherBuilding * to;
	ESurType surtype;
	int num;
	float percent;
};


#endif // _JOBS_H_

