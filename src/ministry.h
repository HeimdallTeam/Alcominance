
#ifndef _MINISTRY_H_
#define _MINISTRY_H_

class MinistryOfJobs
{
	float m_paytime;
	float m_pay;
	IHoeSound * m_sound;

	void PayDay();
public:
	MinistryOfJobs();
	// nacteni zvuku atd..
	bool Init();
	void AddPay(float money) { m_pay += money; }
	void Update(const float dtime);
	bool Save(ChunkDictWrite &w) { return true; }
	bool Load(BecherGameLoad &r) { return true; }
};

#endif // _MINISTRY_H_

