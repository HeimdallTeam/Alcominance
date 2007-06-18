
#ifndef _BECHERGAME_BUILDINGS_H_
#define _BECHERGAME_BUILDINGS_H_

#include "object.h"
#include "workspace.h"

class Troll;
struct TJob;

extern CVar v_idiottime;
/**
* Vsechno co ma spolecneho s funkcni budovou
*/
class BecherBuilding : public BecherObject
{
protected:
	EBuildingMode m_mode;

	/** Funkce pro zjisteni jestli je mozno postavit budovu */
	virtual int StatusPlace(float *pos);
	int BuildPlace(float *pos);
public:
	BecherBuilding(IHoeScene * scn);
	//bool IsBuildMode() { return m_mode == ; }
	bool StartBuilding(int gold, int wood, int stone);
	virtual void SetMode(EBuildingMode mode) { m_mode = mode; }
	virtual bool Save(ChunkDictWrite &w);
	virtual bool Load(const ChunkDictRead &r);
	virtual void OnUpdateSur() {}

	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
};

/**
 * Budova ktera se da stavet
 */
class ConstructBuilding : public BecherBuilding
{
protected:
	// staveni
	Workspace m_build;
	ResourceImp m_stone;
	ResourceImp m_wood;
	float m_buildprogress;
    // organizator prace
	Chief m_chief;
public:
	ConstructBuilding(IHoeScene * scn, CVar &v_build);

	inline bool InBuildProcess() { return m_buildprogress < 1.f; }
    void IdiotBuild();
	static uint ReqResource(const char * recept, float progress, char r);

	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    virtual void Update(const float t);
};

/**
 * Budova zdroje, pouzivaji ji doly a studna
 * obsahuje frontu do ktere se muzou tupouni zaregistrovat a tezit.
 */
class SourceBuilding : public ConstructBuilding
{
protected:
	LineUp m_line;
public:
	SourceBuilding(IHoeScene * scn, CVar &v_build);
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
};

/**
 * Pracovni budova
 * Ma funkce pro praci a funkci pro postaveni budovy
 */
class WorkBuilding : public ConstructBuilding
{
protected:
public:
    WorkBuilding(IHoeScene * scn, CVar &v_build);    
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    virtual void Update(const float t);
};

/**
 * Produkcni budova
 * budova pouze produkuje na zaklade nejake prace (farma, pila)
 */
class ProductionBuilding : public WorkBuilding
{
public:
    ProductionBuilding(IHoeScene * scn, CVar &v_build);        
};

/**
 * Tovarni budova
 * funkce pro tovarny, cukrovar, lihovar a tovarna
 */
class FactoryBuilding : public WorkBuilding
{
protected:
	// prace
    Workspace m_work;
	ResourceImp m_coal;
public:
	FactoryBuilding(IHoeScene * scn, CVar &v_recept, CVar &v_build);
	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
	virtual void Update(const float t);
};

#endif // _BECHERGAME_BUILDINGS_H_

