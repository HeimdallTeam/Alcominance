
#ifndef _BECHER_COALMINE_H_
#define _BECHER_COALMINE_H_

#include "buildings.h"

class CoalMine : public SourceBuilding
{
	friend class BecherLevel;
protected:
	// panel
	ResourceExp m_coal;
public:
	CoalMine(IHoeScene * scn);

	virtual EObjType GetType() { return EBO_CoalMine; } 
	virtual void Update(const float t); 
	virtual bool Save(ChunkDictWrite &w); 
	virtual bool Load(BecherGameLoad &r); 

	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);

#ifdef BECHER_EDITOR
	virtual bool Select();
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
#endif // BECHER_EDITOR
};


#endif // _BECHER_COALMINE_H_

