
#ifndef _BECHER_SYSTEM_OBJECTS_H_
#define _BECHER_SYSTEM_OBJECTS_H_

#include "object.h"

#ifdef BECHER_EDITOR
class BecherSystemObject : public BecherObject
{
protected:
	float m_x;
	float m_y;
public:
	BecherSystemObject(IHoeScene * scn) : BecherObject(scn) {}
	virtual bool Save(ChunkDictWrite &w) { return true; }
	virtual bool Load(ChunkDictRead &r) { return true; }
};
#else
class BecherSystemObject
{
protected:
	float m_x;
	float m_y;
public:
	BecherSystemObject(IHoeScene * scn) {}
	virtual ~BecherSystemObject() {}
	virtual EObjType GetType() = 0; 
	virtual bool Save(ChunkDictWrite &w) { return true; }
	virtual bool Load(ChunkDictRead &r) { return true; }
	float GetPosX() { return m_x; }
	float GetPosY() { return m_y; }
};
#endif


class SystemObjectSound : public BecherSystemObject
{
public:
	SystemObjectSound(IHoeScene * scn);
	virtual EObjType GetType() { return EBSys_Sound; }
	virtual bool Save(ChunkDictWrite &w);
	virtual bool Load(BecherGameLoad &r);
	static IHoeModel * GetModelForSound();
#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual bool Select();
	virtual void SetPosition(const float x, const float y, const float h);
#endif
};

class SystemObjectPlace : public BecherSystemObject
{
public:
	SystemObjectPlace(IHoeScene * scn);
	virtual EObjType GetType() { return EBSys_Place; }
	virtual bool Save(ChunkDictWrite &w);
	virtual bool Load(BecherGameLoad &r);
	static IHoeModel * GetModelForPlace();
#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual bool Select();
	virtual void SetPosition(const float x, const float y, const float h);
#endif
};

class SystemObjectSource : public BecherSystemObject
{
protected:
	float m_distance;
	uint m_source;
public:
	SystemObjectSource(IHoeScene * scn);
	virtual bool Save(ChunkDictWrite &w);
	virtual bool Load(ChunkDictRead &r);
	static IHoeModel * GetModelForSource();
	float GetDistance(float x, float y);
#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual bool Select();
	virtual void SetPosition(const float x, const float y, const float h);
#endif
};

class SystemObjectWater : public SystemObjectSource
{
public:
	SystemObjectWater(IHoeScene * scn) : SystemObjectSource(scn) {}
	virtual EObjType GetType() { return EBSys_Water; }
};

class SystemObjectCoal : public SystemObjectSource
{
public:
	SystemObjectCoal(IHoeScene * scn) : SystemObjectSource(scn) {}
	virtual EObjType GetType() { return EBSys_Coal; }
};

class SystemObjectStone : public SystemObjectSource
{
public:
	SystemObjectStone(IHoeScene * scn) : SystemObjectSource(scn) {}
	virtual EObjType GetType() { return EBSys_Stone; }
};




#endif // _BECHER_SYSTEM_OBJECTS_H_

