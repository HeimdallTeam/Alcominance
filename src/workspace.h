#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#include "id.h"

class BecherObject;

class ResourceBase
{
protected:
	uint m_actual;
public:
	ResourceBase() { m_actual = 0; }
	inline uint GetNum() const { return (int)m_actual; }
	inline void SetNum(uint num) { m_actual = num; }
	bool Add(uint *s, int max);
	/**
	* Vybiraci funkce
	* @param p Zda brat i jen cast
	*/
	uint Get(uint req, bool p);
};

class ResourceImp : public ResourceBase
{
public:
};

enum ESurPriority
{
	// nevydavat
	// spis zanechat
	// normalni
	// zbavit se
    EBSP_None = 0,
    EBSP_TimeWork,
};

class ResourceExp : public ResourceBase
{
protected:
	ESurType m_type;
	uint m_max;
	ESurPriority m_priority;
	BecherObject * m_owner;
public:
	ResourceExp(ESurType type);
	void SetOwner(BecherObject * own) { m_owner = own; }
	inline BecherObject * GetOwner() { assert(m_owner); return m_owner; }
	inline ESurType GetType() { return m_type; }
	inline ESurPriority GetPriority() { return m_priority; }
	inline void SetPriority(ESurPriority p) { m_priority = p; }
	void Register();
	void Unregister();
};

// 
class DrumEngine : public ResourceImp
{
public:
};

class Workspace : public DrumEngine
{
	CVar * m_recept;
	float m_progress;
	uint m_out;
	uint m_mout;
	// pro parsovani stringu
	int GetNumber(char type);
public:
	Workspace();
	void SetRecept(CVar * recept);
	void Update(const float t);
	bool In(ResourceBase * sur,char type, bool remove);
	uint Out(bool remove);
	bool CanIn() { return m_progress <= 0.f && m_out == 0 && m_mout == 0; }
	bool CanOut() { return m_out > 0; }
	bool InProcess() { return !m_out && m_mout; }
	void ToProcess();
};

#endif // _WORKSPACE_H_