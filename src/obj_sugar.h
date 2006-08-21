
#ifndef _BECHER_OBJECT_SUGAR_H_
#define _BECHER_OBJECT_SUGAR_H_

#include "buildings.h"

class Sugar : public FactoryBuilding, TrollList
{
protected:
	// trolls
	ResourceImp m_cane;
	ResourceExp m_sugar;
	//WorkMode m_mode;
	TrollList m_worked;
	HoeGame::StopWatch m_exitdelay;
	inline const int GetMiniStoreCount() const { return m_cane.GetNum() + m_sugar.GetNum(); }
public:
	Sugar(IHoeScene * scn);
	virtual ~Sugar();

#ifdef BECHER_EDITOR
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
#else
	virtual bool SetToWork(Troll * t);
	virtual void UnsetFromWork(Troll * t);
	virtual bool InsertSur(ESurType type, uint *s);
#endif

	DECLARE_BUILDING(EBO_Sugar)
};

#endif // _BECHER_OBJECT_SUGAR_H_
