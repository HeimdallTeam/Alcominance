
#ifndef _BECHERGAME_BUILDINGS_H_
#define _BECHERGAME_BUILDINGS_H_

#include "object.h"
#include "workspace.h"

class Troll;
struct Job;

/**
* Vsechno co ma spolecneho s funkcni budovou
*/

class BecherBuilding : public BecherObject
{
protected:
public:
	BecherBuilding(IHoeScene * scn);
	bool IsBuildMode() { return false; }
	bool StartBuilding(int gold, int wood, int stone);
	//virtual bool Save(BecherGameSave &w);
	//virtual bool Load(BecherGameLoad &r);
	virtual bool InsertSur(ESurType type, uint *s) { assert(!"add surovina"); return false; }
	virtual bool SetToWork(Troll * t) { assert(!"add to work"); return false; }
	virtual void UnsetFromWork(Troll * t) { assert(!"add to work"); }
	virtual bool Idiot(Job * t) { return false; }

};

class FactoryBuilding : public BecherBuilding
{
protected:
	Workspace m_w;
public:
	FactoryBuilding(IHoeScene * scn) : BecherBuilding(scn) {}
};

class SourceBuilding : public BecherBuilding
{
public:
	SourceBuilding(IHoeScene * scn) : BecherBuilding(scn) {}
};

/**
* Objekt ktery se stara o tupouny
* Ma prehled o vsech tupounech kteri pro nej pracuji
* Dokaze jim davat rozkazy
* Prideluje praci
*/
class TrollList : public HoeGame::PtrSet<Troll *>
{
public:
	TrollList() { }
	void OneStopWork();
};

// neni cukr -> vyslany
// cukr -> 50% -> maka
// plny sklad
// trollove se sami ptaji po praci
// pokud najdou praci registruji se, pokud ne, odregistruji se
// idiot prideluje praci, objednava zakazky, atd
// jakmile trol, splni ukol, prijde zpet 


#endif // _BECHERGAME_BUILDINGS_H_

