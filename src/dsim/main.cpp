
#include "../StdAfx.h"
#include "../bechapp.h"
#include "../console.h"
#include "../becher.h"
#include "../game.h"

using namespace HoeGame;

//////////////////////////////////////
// Dummy Hoe

class DummyObjectController : public IHoeObjectController
{
public:
	/** Prirazeni modelu */
	virtual void HOEAPI SetModel(IHoeModel * model) {}

	/** Vrati aktualni model */
	virtual IHoeModel * HOEAPI GetModel() { return NULL; }

	/** Nahrani modelu */
	virtual bool HOEAPI LoadModel(const char * cmd) { return true; }

	/** Nastavi aktualni pozici objektu */
	virtual void HOEAPI SetPosition(const HoeMath::Vector3 &pos) {}

	/** vrati aktualni pozici objektu */
	virtual const HoeMath::Vector3 & HOEAPI GetPosition() const {}

	/** Nastavi oriantaci objektu */
	virtual void HOEAPI SetOrientation(const float x, const float y, const float z, const float angle) {}

	/** Vrati oriantaci objektu */
	//virtual void HOEAPI GetOrientation(float *x, float *y, float *z, float *angle) = 0;

	virtual void HOEAPI SetScale(const HoeMath::Vector3 & scale) {}

	virtual const HoeMath::Vector3 & HOEAPI GetScale() const {}

	/** nastaveni priznaku */
	virtual void HOEAPI SetFlags(unsigned long  flags) {}

	/** odnastaveni priznaku */
	virtual void HOEAPI UnsetFlags(unsigned long  flags) {}

	/** prilinkovani podobjektu */
	virtual void HOEAPI Link(THoeSubObject::Type type, THoeSubObject * obj) {}

	/** odlinkovani podobjektu */
	virtual void HOEAPI Unlink(THoeSubObject * obj) {}
	
	/** zobrazeni casu animace */
	virtual void HOEAPI SetAnimationTime(float t) {}

	virtual void HOEAPI SetOverColor(dword color) {}
};


class DummyScene : public IHoeScene
{
public:
	virtual void HOEAPI Set2DCallback(XHoe2DCallback * cb) {}
	/** Zaregistruje objekt */
	virtual int HOEAPI RegisterObject(XHoeObject *o, unsigned long flags = 0) 
    {
        static DummyObjectController d;
        o->SetCtrl(&d);
        return 0; 
    }
	/** Zaregistruje objekt */
	virtual void HOEAPI UnregisterObject(XHoeObject *) {}
	/** Vytvori particle emitor */
	virtual IHoeParticleEmitor * HOEAPI CreateParticleEmitor() { return NULL; }
	/** Vytvori svetlo */
	virtual IHoeLight * HOEAPI CreateLight(bool diRECT)  { return NULL; }
	/** Fyzika */
	virtual IHoeScenePhysics * HOEAPI GetScenePhysics()  { return NULL; }

	/** Prostredi sceny */
	virtual IHoeSceneEnv * HOEAPI GetSceneEnv()  { return NULL; }

	/** Kamera */
	virtual IHoeCamera * HOEAPI GetCamera()  { return NULL; }
	/** smazani sceny */
	virtual void Delete() {}
};



//////////////////////////////////


bool Configure(int argc,char * argv[]);

DummyScene scn;

class Dummy : public BecherObject
{
public:
    Dummy() : BecherObject(&scn) {}
    virtual EObjType GetType() { return EBO_None; }
};

int main(int argc,char * argv[])
{
	SetRootFromExe(argv[0]); 
	HOE_INSTANCE hInstance = NULL;

    HoeGame::Console con;

	con.SetFileLogging("becher.log");

	con.Con_Print("****************************");
	con.Con_Print("* Alcominance  Simulation! *");
	con.Con_Print("****************************");

	if (!Configure(argc,argv))
		return 0;

    // test LineUp
    LineUp lu;
    Dummy d;

    lu.Register(&d, 10);
    ResourceBase sur(EBS_Water);
    sur.SetOwner(&d);
    sur.SetNum(50);
    for (float t=0.f;t < 10.f;t += 0.1f)
    {
        con.Printf("%f: ",t);
        lu.Update(0.1f, sur, 1);
    }
    
    return 0;    
}






