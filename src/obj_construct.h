
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

class Construct;

#ifndef BECHER_EDITOR
class ConstructStatic : public ObjectHud
{
protected:
	//char m_sugarinfo[256];
	//char m_trtinainfo[256];
	Construct * m_act;
public:
	ConstructStatic();
	void SetAct(Construct * act);
	virtual void Draw(IHoe2D * h2d);
};
#endif // BECHER_EDITOR

class Construct
{
	friend class BecherLevel;
	friend class SugarStatic;
protected:
#ifndef BECHER_EDITOR
	static ConstructStatic m_userhud;
#endif // BECHER_EDITOR
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

