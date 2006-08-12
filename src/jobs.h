
#ifndef _JOBS_H_
#define _JOBS_H_

class BecherBuilding;
class Troll;

struct Job
{
	enum Type
	{
		jtNone = 0,
		jtPrines,
		jtWork,
	} type;
	BecherBuilding * owner;
	ResourceItem * ritem;
	ESurType surtype;
	int num;
	float percent;
};


#endif // _JOBS_H_

