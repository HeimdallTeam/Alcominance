
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

// pole prikazu a parametru


struct TJob
{
	enum EType
	{
		EJT_None = 0,
		EJT_Go,
		EJT_Work, // pracovat
		EJT_WaitToRes, // cekat na suroviny

        EJP_Source,
        EJP_Owner,
	} type;
    
	// parametry
	BecherBuilding * owner;
	BecherBuilding * source;
	
    
    
    bool Save(BecherGameSave &w);
	bool Load(BecherGameLoad &r);

	// function for build job
	void Go(float x, float y);
};


#endif // _JOBS_H_

