#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#include "id.h"

class BecherBuilding;
class Troll;

/**
* Workspace se stara o spravny chod budovy
* umi zpracovavat suroviny, a prevadet je na praci (podle urciteho receptu)
* postup:
* podle receptu
* chtelo byt to tabulku pro prevod resources
* statickou
* napr.
* aby to nejelo tak rychle, tak se to pousti jednou za x spusteni

while (m_w.BeginPass(int nwork, float time))
{
	m_w << m_cane;
	m_w << m_water;
	m_w >> m_out;
	m_w.Commit();
}

*/

class ResourceBase
{
protected:
	uint m_actual;
	BecherBuilding * m_owner;
	ESurType m_type;
public:
	ResourceBase(ESurType type);
	inline uint GetNum() const { return (int)m_actual; }
	inline ESurType GetType() { return m_type; }
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
	ResourceImp(ESurType type) : ResourceBase(type) {}
};

// zamykani surovin (get odemkne)

class ResourceExp : public ResourceBase
{
protected:
	uint m_max;
	ESurPriority m_priority;
	uint m_locked;
public:
	ResourceExp(ESurType type);
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

class Workspace
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

class Workspace2
{
// staticke prvky pro pocitani
	static const int c_max_in = 20;
	static const int c_max_out = 5;

	struct Request
	{
		ResourceBase * res;
		uint num;
	};
	struct RequestOut
	{
		union {
			ResourceBase * res;
			float * f;
		};
		float num;
		enum Type {
			EResource,
			EFloat,
		} type;
	};
	static int s_passes; // znasobeni
	static int s_num_in,s_num_out;

	static Request s_req_in[c_max_in];
	static RequestOut s_req_out[c_max_out];
// worspace
	CVar * m_recept;
	float m_desttime;
public:
	Workspace2(CVar * recept);
	/** Zacne process
	* @param workers Kolik tupounu pracovalo
	* @param time kolik casu ubehlo
	* @
	*/
	int BeginPass(float workers, float time);
	bool operator << (ResourceBase & in);
	bool operator >> (ResourceBase & out);
	bool operator >> (float & out);
	void Commit();
};

typedef HoeCore::Set<Troll*> TrollList;

struct TTrollWorkSlot
{
	Troll * troll;
	float t;
	uint num;
	uint req;
	bool operator == (const TTrollWorkSlot& slot) { return troll == slot.troll; }
};

class BecherObject;
/**
 * Chief se stara o tupouny, prideluje praci, propousti, najima
 */
class Chief
{
    struct TTroll
    {
        EWorkType type;
        Troll * troll;
    };
    HoeCore::Set<TTroll> m_list;
    // troll list s hodnotami prace
    // pocitani statistik
    static int s_stats[EBW_Max];
    static Chief * s_lastupdater;
    int m_worked;

    void ComputeStatistik();
	inline void ResetStat(bool recount)
	{
		if (s_lastupdater == this) s_lastupdater = NULL; 
		if (recount) ComputeStatistik();
	}
public:
    Chief();
    void Make(BecherObject * owner, const char * cmd);
    int GetNumWorkers(EWorkType type);
	void Incoming(Troll *);
};

#endif // _WORKSPACE_H_


