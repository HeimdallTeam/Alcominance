#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#include "id.h"

class BecherBuilding;
class Troll;

class ResourceBase
{
protected:
	uint m_actual;
	BecherBuilding * m_owner;
public:
	ResourceBase();
	inline uint GetNum() const { return (int)m_actual; }
	void SetNum(uint num);
	void SetOwner(BecherBuilding * own) { m_owner = own; }
	inline BecherBuilding * GetOwner() { assert(m_owner); return m_owner; }
	bool Add(uint *s, int max);
	bool Save(BecherGameSave &w);
	bool Load(BecherGameLoad &r);

	/**
	* Vybiraci funkce
	* @param p Zda brat i jen cast
	*/
	uint Get(uint req, bool p);
};

class ResourceImp : public ResourceBase
{
public:
};

// zamykani surovin (get odemkne)

class ResourceExp : public ResourceBase
{
protected:
	ESurType m_type;
	uint m_max;
	ESurPriority m_priority;
	uint m_locked;
public:
	ResourceExp(ESurType type);
	inline ESurType GetType() { return m_type; }
	inline ESurPriority GetPriority() { return m_priority; }
	inline void SetPriority(ESurPriority p) { m_priority = p; }
	void Register();
	void Unregister();
	uint Lock(uint num); ///< zamknuti suroviny aby pro ni nesel nikdo jiny (tvari se jako by tam nebylo)
	void Unlock(uint num); ///< vraceni suroviny pri zruseni
	uint GetAvail() { return GetNum()-m_locked; } ///< vraceni kolik je k dispozici
	bool Save(BecherGameSave &w);
	bool Load(BecherGameLoad &r);

};

// 
class DrumEngine : public ResourceImp
{
public:
};

class Workspace : public DrumEngine
{
	CVar * m_recept;
	float m_progress;
	uint m_out;
	uint m_mout;
	// pro parsovani stringu
	int GetNumber(char type);
public:
	Workspace();
	void SetRecept(CVar * recept);
	void Update(const float t);
	bool In(ResourceBase * sur,char type, bool remove);
	uint Out(bool remove);
	bool CanIn() { return m_progress <= 0.f && m_out == 0 && m_mout == 0; }
	bool CanOut() { return m_out > 0; }
	bool InProcess() { return !m_out && m_mout; }
	void ToProcess();
};

/**
* Objekt ktery se stara o tupouny
* Ma prehled o vsech tupounech kteri pro nej pracuji
* Dokaze jim davat rozkazy
* Prideluje praci
*/
class TrollList : public HoeCore::Set<Troll *>
{
public:
	TrollList() { }
	void OneStopWork();
};

struct TTrollWorkSlot
{
	Troll * troll;
	float t;
	uint num;
	uint req;
	bool operator == (const TTrollWorkSlot& slot) { return troll == slot.troll; }
};

#endif // _WORKSPACE_H_


