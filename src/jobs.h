
#ifndef _JOBS_H_
#define _JOBS_H_

class BecherBuilding;
class Troll;
class ResourceExp;
class BecherGameSave;
class BecherGameLoad;

/**
* Jobs se stara o to aby vedeli tupouni co delat, je vsak udelanej
* Pole miniprogramu, pro tupouny
* Tupoun ma v sobe ukazatel na program, podle instrukci ukolu si bere parametry
* kazdy job ma sveho zadavatele
* a parametry
* napr.
* import:
*   jit k cilove budove
*   vzit suroviny/vytezit suroviny
*   jit zpatky
*   vlozit suroviny
*   ohlasit hotovo
*   cekat 
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

