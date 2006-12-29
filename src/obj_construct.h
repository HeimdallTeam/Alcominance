
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

class Construct : public XHoeObject
{
	friend class BecherLevel;
protected:
	BecherBuilding * m_owner;
	uint m_reqwood;
	uint m_reqstone;
	uint m_actwood;
	uint m_actstone;
	float m_buildtime;
	float m_acttime;

	TrollList m_worked;
    int m_wrk_wood;
    int m_wrk_stone;

	HoeGame::StopWatch m_exitdelay;
public:
	Construct(BecherBuilding * owner);
	void SetBuildTime(float t) { m_buildtime = m_acttime = t; }
	void SetResources(uint wood, uint stone);
	bool Save(BecherGameSave &w);
	bool Load(BecherGameLoad &r);
#ifndef BECHER_EDITOR
	void Update(const float t);
	bool Select();
	bool Idiot(TJob * j);
	bool SetToWork(Troll * t);
	void UnsetFromWork(Troll * t);
	bool InsertSur(ESurType type, uint *s);
	float GetProgress() { return 1.f - (m_acttime / m_buildtime); }

	void Finish();
#endif
};

#endif // _CONSTRUCT_H_

