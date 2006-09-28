
#ifndef _BECHER_OBJECT_H_
#define _BECHER_OBJECT_H_

#include "id.h"
#include <hoefs.h>

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

	virtual void Update(const float) {}

	virtual bool Select();
	virtual void Unselect();

	virtual const char * BuildPlace(float x, float y) { return "Budova neni hotova"; /*todo*/ }

	virtual void OnDeleteObject(unsigned long id) {}; 
#ifdef BECHER_EDITOR
	virtual 
#endif
		void SetPosition(const float x, const float y, const float h);
	inline const float GetPosX() const { return posX; }  
	inline const float GetPosY() const { return posY; }  

	unsigned long GetID() { return id; }

	// smazat
	void SetProperty(const char * prop, const char * value);
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
	virtual bool Load(BecherGameLoad &r);
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



