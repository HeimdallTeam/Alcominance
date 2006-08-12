
#ifndef _BECHERGAME_BUILDINGS_H_
#define _BECHERGAME_BUILDINGS_H_

#include "object.h"

class Troll;
struct Job;

template <int N> struct WorkT
{
	Troll * works[N];
	size_t num;
	bool Add(Troll * t)
	{
		if (num < N) works[num++]=t;
		else return false;
		return true;
	}
	size_t NumFree()
	{
		return N - num;
	}
};

class BecherBuilding : public BecherObject
{
protected:
public:
	BecherBuilding(IHoeScene * scn);
	bool IsBuildMode() { return false; }
	bool StartBuilding(int gold, int wood, int stone);
	virtual bool Save(BecherGameSave &w);
	virtual bool Load(BecherGameLoad &r);
	virtual int AddSur(ESurType type, int s) { assert(!"add surovina"); return 0; }
	virtual int GetSur(ESurType type, int req, bool upln) { assert(!"get surovina"); return 0; } 
	virtual void AddToWork(class Troll * t) { assert(!"add to work"); }
#ifndef BECHER_EDITOR
	virtual bool Idiot(Job * t) { return false; }
#endif
};

class FactoryBuilding : public BecherBuilding
{
protected:
	
public:
	FactoryBuilding(IHoeScene * scn) : BecherBuilding(scn) {}
};

/**
* Objekt ktery se stara o tupouny
* Ma prehled o vsech tupounech kteri pro nej pracuji
* Dokaze jim davat rozkazy
* Prideluje praci
*/
class TrollList
{
protected:
	Troll *  m_trolls[10];
	int m_numtrolls;	
public:
	TrollList() { m_numtrolls = 0; }
	void RegisterTroll(Troll * t);
	void UnregisterTroll(Troll * t);
};

// neni cukr -> vyslany
// cukr -> 50% -> maka
// plny sklad
// trollove se sami ptaji po praci
// pokud najdou praci registruji se, pokud ne, odregistruji se
// idiot prideluje praci, objednava zakazky, atd
// jakmile trol, splni ukol, prijde zpet 



class HerbeWoman : public BecherBuilding
{
public:
	HerbeWoman(IHoeScene * scn);
	virtual ~HerbeWoman();
	//virtual void Select();
	virtual void AdvPaint(IHoePaint3D * h3);

	DECLARE_BASEOBJECT(EBO_HerbeWoman)
};

#endif // _BECHERGAME_BUILDINGS_H_

