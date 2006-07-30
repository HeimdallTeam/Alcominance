
#ifndef _BECHER_OBJECT_H_
#define _BECHER_OBJECT_H_

#include "id.h"
#include "infos.h"
#include <hoefs.h>

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
	unsigned int m_info;
public:
	unsigned long id;

	BecherObject(IHoeScene * scn);
	virtual EObjType GetType() = 0; 

	virtual bool Save(BecherGameSave &w);
	virtual bool Load(BecherGameLoad &r);

	void SetAngle(float angle);
	float GetAngle() { return m_angle; }

	/** Funkce pro nastavovani viditelnosti infa */
	void SetInfo(unsigned int info);

	/** Funkce pro odnastavovani viditelnosti infa */
	void UnsetInfo(unsigned int info);

	bool IsSelected() { return false; }
	bool IsCurActive() { return false; }

	virtual bool Select() { return false; }
	virtual void Unselect() { }

	void SetPosition(const float x, const float y, const float h);
	inline const float GetPosX() const { return posX; }  
	inline const float GetPosY() const { return posY; }  

	unsigned long GetID() { return id; }

	virtual void SetProperty(const char * prop, const char * value);
	virtual int GetNumInfos() { return 0; }
	virtual int GetInfo(int id, char * buff, size_t size) { return 0; }
};

#define DECLARE_BASEOBJECT(type) virtual EObjType GetType() { return type; }
#ifndef BECHER_EDITOR
#define DECLARE_BUILDING(type) virtual EObjType GetType() { return type; } \
	virtual void Update(const double t); \
	virtual bool Select(); \
	virtual bool Idiot(Troll * t); \
	virtual int AddSur(ESurType type, int s); \
	virtual int GetSur(ESurType type, int req, bool upln); \
	virtual void AddToWork(class Troll * t); \
	virtual int GetNumInfos(); \
	virtual int GetInfo(int id, char * buff, size_t size);
#else
#define DECLARE_BUILDING(type) virtual EObjType GetType() { return type; } \
	virtual bool Select();
#endif

#endif // _BECHER_OBJECT_H_



