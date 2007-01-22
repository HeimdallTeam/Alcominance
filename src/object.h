
#ifndef _BECHER_OBJECT_H_
#define _BECHER_OBJECT_H_

#include "id.h"
#include <hoefs.h>
#include "utils.h"

class Troll;

struct TObjectSaveStruct
{
	unsigned long id;
	unsigned long type;
	float x;
	float y;
	float angle;
	unsigned long reserved;
};



class BecherObject : public HoeGame::Strategy::StgObject
#ifdef BECHER_EDITOR
	, public HoeEditor::PropObject
#endif
{
protected:
	float m_angle;
	float posX;
	float posY;
	THoeSub_Model m_infoselect;
public:
	unsigned long id;

	BecherObject(IHoeScene * scn);
	virtual EObjType GetType() = 0; 

	virtual bool Save(BecherGameSave &w);
	virtual bool Load(BecherGameLoad &r);

	void SetAngle(float angle);
	float GetAngle() { return m_angle; }

	virtual void SetCurActive(bool active);

	bool IsSelected() { return false; }
	bool IsCurActive() { return false; }

	virtual void Update(const float);

	virtual bool Select();
	virtual void Unselect();

	virtual void SetPosition(const float x, const float y, const float h);
	inline const float GetPosX() const { return posX; }  
	inline const float GetPosY() const { return posY; } 
	inline const HoeMath::Vector2 GetPos() const { return HoeMath::Vector2(posX, posY); }

	unsigned long GetID() { return id; }

	void SetRingParam(float sx, float sy, float height);

	virtual int GetInfo(int type, char * str=NULL, size_t n=0) { return 0; }
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

	static int DefaultCustomInfo(const char * str);
};

#define DECLARE_BASEOBJECT(type) virtual EObjType GetType() { return type; }
#ifndef BECHER_EDITOR
#define DECLARE_BUILDING(type) virtual EObjType GetType() { return type; } \
	virtual void Update(const float t); \
	virtual bool Select(); \
	virtual bool Idiot(TJob * j); \
	virtual bool SetToWork(Troll * t); \
	virtual void UnsetFromWork(Troll * t); \
	virtual bool InsertSur(ESurType type, uint *s); \
	virtual bool Save(BecherGameSave &w); \
	virtual bool Load(BecherGameLoad &r); \
	//virtual ResourceBase * GetResource(ESurType type);
#else
#define DECLARE_BUILDING(type) virtual EObjType GetType() { return type; } \
	virtual bool Select(); \
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi); \
	virtual bool Save(BecherGameSave &w); \
	virtual bool Load(BecherGameLoad &r);
#endif

/*
bool Sugar::Save(BecherGameSave &w)
{
	BecherBuilding::Save(w);
	w.WriteReservedWords(3);
	return true;
}

bool Sugar::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	r.ReadReservedWords(3);
	OnUpdateSur();
	return true;
}
*/

#endif // _BECHER_OBJECT_H_



