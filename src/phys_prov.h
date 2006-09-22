
#ifndef _PHYS_PROVISORIUM_H_
#define _PHYS_PROVISORIUM_H_

/*

Provizorni fyzika

seznam oblasti, kudy se muze chodit
seznam cvercu a kruhu, kde se nemue stavet a kde se nemuze chodit


*/

class Obdelnik
{
public:
	float width;
	float height;
	float radius;
	float angle;
	float x,y; // centr
};

struct Kruh
{
	float radius;
	float x,y;
};

class Phys
{
	HoeGame::PtrSet<Obdelnik*> m_obd;
	HoeGame::PtrSet<Kruh*> m_kruh;
	static Phys m_instance;
public:
	static Phys * Get() { return &m_instance; }
	bool IsOk(const Obdelnik * o)
	{
		for (size_t i=0;i < m_kruh.Count();i++)
		{
			const float lx = m_kruh[i]->x - o->x;
			const float ly = m_kruh[i]->y - o->y;
			const float r = m_kruh[i]->radius+o->radius;
			if (((lx*lx)+(ly*ly)) > (r*r))
				continue;

			// hodne blizko
		}
	}
	void ClearAll();
};

#endif // _PHYS_PROVISORIUM_H_







