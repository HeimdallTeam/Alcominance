
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
	bool m_selected;
	bool m_curactive;
public:
	unsigned long id;

	BecherObject(IHoeScene * scn);
	virtual EObjType GetType() = 0; 

	virtual bool Save(ChunkDictWrite &w);
	virtual bool Load(const ChunkDictRead &r);

	void SetAngle(float angle);
	float GetAngle() { return m_angle; }

	bool IsSelected() { return m_selected; }
	bool IsCurActive() { return m_curactive; }

	virtual void Update(const float);

	virtual void SetPosition(const float x, const float y, const float h);
	inline const float GetPosX() const { return posX; }  
	inline const float GetPosY() const { return posY; } 
	inline const HoeMath::Vector2 GetPos() const { return HoeMath::Vector2(posX, posY); }

	unsigned long GetID() { return id; }

	void SetRingParam(float sx, float sy, float height);

	virtual int GetInfo(int type, char * str=NULL, size_t n=0) { return 0; }
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

	static int DefaultCustomInfo(const char * str);

#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
#endif
};

#define DECLARE_BASEOBJECT(type) virtual EObjType GetType() { return type; }
#ifndef BECHER_EDITOR
#define DECLARE_BUILDING(type) virtual EObjType GetType() { return type; } \
	virtual bool Select(); \
	virtual bool Save(ChunkDictWrite &w); \
	virtual bool Load(BecherGameLoad &r); \
	//virtual ResourceBase * GetResource(ESurType type);
#else
#define DECLARE_BUILDING(type) virtual EObjType GetType() { return type; } \
	virtual bool Select(); \
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi); \
	virtual bool Save(ChunkDictWrite &w); \
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



