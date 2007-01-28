
#ifndef _JOBS_H_
#define _JOBS_H_

#include "b_msg.h"

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

// pole prikazu a parametru


struct TJob
{
	enum EType
	{
		EJT_None = 0,
		EJT_Go,
		EJT_Work, // pracovat
		EJT_WaitToRes, // cekat na suroviny
		EJT_Incoming,
		EJT_GetSur,
		EJT_InsertSur,

        EJP_Source,
        EJP_Owner,
		EJP_Point,
	};
    
	// parametry
	union {
		struct {
			BecherObject * owner;
			BecherObject * source;
		};
		PAR_Favour favour;
	};
	HoeMath::Vector2 point;
	
    bool Save(BecherGameSave &w);
	bool Load(BecherGameLoad &r);

	EType * Get();
	void Skip(int n) { npart+=n; }

	void JobRelease();
	// function for build job
	void Go(float x, float y);
	void Import(PAR_Favour * fav);

private:
	word njob;
	word npart;
};


#endif // _JOBS_H_

