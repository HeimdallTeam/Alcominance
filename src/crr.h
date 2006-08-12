#ifndef _CRR_H_
#define _CRR_H_

#include "buildings.h"

class ResourceItem
{
protected:
	ESurType m_type;
	uint m_max;
	uint m_actual;
	int m_priority;
	BecherObject * m_owner;
public:
	ResourceItem(ESurType type);
	void SetOwner(BecherObject * own) { m_owner = own; }
	inline BecherObject * GetOwner() { assert(m_owner); return m_owner; }
	inline ESurType GetType() { return m_type; }
	inline uint GetNum() { return m_actual; }
	inline void SetNum(uint num) { m_actual = num; }
	inline int GetPriority() { return m_priority; }
	inline void SetPriority(int p) { m_priority = p; }
	void Register();
	void Unregister();
	void Add(uint s) {}
};

/**
 * Central Register of Resource
 * Centralni registr zdroju (surovin) slouzi ...
 */
class CRR 
{
private:
    static CRR* this_;
	HoeGame::PtrList<ResourceItem*> m_items[EBS_Max];
	ResourceItem * s;
public:
    CRR();
    ~CRR();
	static CRR * Get();
    void Register(ResourceItem* item);
	ResourceItem * Find(ESurType type);
};


#endif