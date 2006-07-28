
#ifndef _BECHER_SYSTEM_OBJECTS_H_
#define _BECHER_SYSTEM_OBJECTS_H_

#include "object.h"

#ifdef BECHER_EDITOR
class BecherSystemObject : public BecherObject
{
public:
	BecherSystemObject(IHoeScene * scn) : BecherObject(scn) {}
	virtual bool Save(BecherGameSave &w) { return true; }
	virtual bool Load(BecherGameLoad &r) { return true; }
};
#else
class BecherSystemObject
{
public:
	BecherSystemObject(IHoeScene * scn) {}
	virtual bool Save(BecherGameSave &w) { return true; }
	virtual bool Load(BecherGameLoad &r) { return true; }
};
#endif


class SystemObjectSound : public BecherSystemObject
{
public:
	SystemObjectSound(IHoeScene * scn);
	virtual EObjType GetType() { return EBS_Sound; }
	virtual bool Save(BecherGameSave &w);
	virtual bool Load(BecherGameLoad &r);
	static IHoeModel * GetModelForSound();
#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual bool Select();
#endif
};

#endif // _BECHER_SYSTEM_OBJECTS_H_

