
#ifndef _TUPON_H_
#define _TUPON_H_

#include "object.h"

/**
 Tupoun je zakladni pracovni sila, ve skutecnosti pracuje jen jako nosic
 prace v jednotlivych budovach je naprogramovana primo v budove

*/

struct TrollPath : public HoeGame::LandPath
{
	Point act;
	bool GetNextPos(float l,float &px, float &py);
	bool Step(Troll * t, const float time);
	//void SetPosTo(float X, float Y);
	// nastavi vnitrni promeny na chozeni, k urcenemu mistu
	bool Go(float tx, float ty);
	bool Go(BecherObject * to);
	bool Go(const HoeMath::Vector2 & pos) { return Go(pos.x,pos.y); }
};

class Troll : public BecherObject
{
	//TJob m_job; // co ma aktualne na praci
	TrollPath m_path; // jeho cesta
//	PAR_Load m_load; // naklad (co ma u sebe)
    // dict <- tabulka ktera preziva s tupounem
	float anim;
	HoeCore::Table m_data; // data
protected:
    void Finish();
    bool Step(float t);
	
    
    enum EAction {
		EA_None,
		EA_Wait,
		EA_Go,
        EA_Mining,
	} m_action;

   EWorkType m_vocation;
   
public:
	Troll(IHoeScene * scn);
	~Troll();

	virtual EObjType GetType() { return EBO_Troll; }

	virtual void Update(const float t);
	virtual bool Select();

	virtual bool Save(ChunkDictWrite &w);
	virtual bool Load(BecherGameLoad &r);

    //const TJob & GetJob() { return m_job; }


	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
};

extern CVar v_troll_num;

#endif // _TUPON_H_


