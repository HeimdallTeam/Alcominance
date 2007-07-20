#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#include "id.h"

class BecherObject;
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
	BecherObject * m_owner;
	ESurType m_type;
public:
	ResourceBase(ESurType type);
	inline uint GetNum() const { return (int)m_actual; }
	inline ESurType GetType() { return m_type; }
	void SetNum(uint num);
	void SetOwner(BecherObject * own) { m_owner = own; }
	inline BecherObject * GetOwner() { assert(m_owner); return m_owner; }
	bool Add(uint *s, int max);
	bool Save(ChunkDictWrite &w);
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
	bool Save(ChunkDictWrite &w);
	bool Load(BecherGameLoad &r);

};

class Workspace
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
	Workspace(CVar * recept);
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
	const char * GetRecept() { return m_recept->GetString(); }
};

typedef HoeCore::Set<Troll*> TrollList;

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
	void SetJob(int t, EWorkType type);
	CVar *mv_max;
public:
	/** Kontruktor */
    Chief();
    /** Nastavi ukazatel na maximalni pocet tupounu co muze budova najmout 
    * @param max reference na var
    */
	void SetMax(CVar &max) { mv_max = &max; }
	/** Vykona prikaz, viz developer help 
	* @param owner Vlastnik objektu
	* @param cmd Prikaz (viz developer help)
	* @return Vraci pocet tupounu, kterym byl zmenen stav.
	*/
    int Make(BecherObject * owner, const char * cmd);
    /** Pocet tupounu
    * @param type Filtr pro typ prace, EBW_All pro vsechny
    * @return pocet
    */
    int GetNumWorkers(EWorkType type);
    /** Aktualni maximalni pocet tupounu co muze pracovat
    * @return pocet
    */
	int GetMaxWorkers() { return mv_max ? mv_max->GetInt():0; }
	/** Tupoun dokoncil praci */
	void Incoming(Troll *);
};

/**
 * LineUp - poradi v tezbe
 */
class LineUp
{
	struct QWorker
	{
		BecherObject * troll;
		uint req;
		uint num;
		bool work;
	};
	HoeCore::Queue<QWorker> m_queue; ///< fronta vsech tupounu
	float m_dest; ///< prebytek z posledniho odberu
public:
	LineUp();
	/** Zaregistrovani trolla do fronty
	* @param troll Ukazatel na trolla
	* @param req Kolik potrebuje surovin
	*/
	void Register(BecherObject* troll, uint req);
	/** Stopnuti trolla */
	void Cancel(BecherObject* troll);
	/** Update
	* @param d kolik natezit (cas * jednotka)
	* @param resource tezebni surovina
	* @param maxtroll kolik maximalne trollu muze tezit
	*/
	void Update(float d, ResourceBase & resource, uint maxtrol);
	/** Surovina vytezena, jiz neni co tezit
	*/
	void End();
};

/**
 * Kotel - zpracovani uhli 
 */
class Cooker
{
    // kotel je zavisly na surovine = resource
    // ma nastavitelnou rychlost spotreby = var
    // vraci vysledky pro particles
    // jeho spotreba je zavisla na rychlosti vyroby
	bool m_prev;
	bool m_start;
	bool m_end;
	float m_coalcache;
	CVar * m_usage;
public:
	Cooker();
    float Update(float dt, ResourceBase & coal);
	/** ridici funkce pro partikle */
	bool WarmStart()
	{
		if (m_start) { m_start = false; return true; }
		return false;
	}
	bool WarmEnd()
	{
		if (m_end) { m_end = false; return true; }
		return false;
	}
	/** nastaveni spotreby (bez toho kotel nejede) */
	void SetUsage(CVar * usage) { m_usage = usage; }
};



#endif // _WORKSPACE_H_


