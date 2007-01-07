
#ifndef _JOBS_H_
#define _JOBS_H_

class BecherBuilding;
class Troll;
class ResourceExp;
class BecherGameSave;
class BecherGameLoad;

/**
* Jobs se stara o to aby vedeli tupouni co delat, je vsak udelanej
* na kterejkoliv objekt
* obsahuje zasobnik ukolu
* funguje jako zasobnik
* funguji tzv. prepisovaci pravidla
* 
*/

struct TJob
{
	enum Type
	{
		jtNone = 0,
		jtGotoRes, // jit pro suroviny
		jtGotoOwnerWithRes, // odevzdat suroviny
		jtGotoWork, // jit pracovat
		jtWork, // pracovat
		jtWaitToRes, // cekat na suroviny
		jtFindJob, // najit praci (v budove)
		jtFly, // dostane padaka
		jtGoto, // nekam jit
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

