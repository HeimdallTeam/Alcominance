
#ifndef _BECHER_WATERHOLE_H_
#define _BECHER_WATERHOLE_H_

#include "buildings.h"

/** Studna
*/
class WaterHole : public SourceBuilding
{
protected:
	/// voda
	ResourceExp m_water;
	float m_kap; ///< pritekajici voda
	HoeCore::Set<TTrollWorkSlot> m_worked;

	/** Callback funkce ktera kontroluje jestli je mozno stavet (pozice) */
	virtual int StatusPlace(float *pos);
	/** najde prislusne zdroje vody
	* @return kolik vody ma pribyt za minutu 
	*/
	int GetSourcePower();
public:
	WaterHole(IHoeScene * scn);

	virtual int GetInfo(int type, char * str=NULL, size_t n=0);
	virtual int GameMsg(int msg, int par1, void * par2, uint npar2);
    virtual void Update(const float t);

	DECLARE_BUILDING(EBO_WaterHole)
};


#endif // _BECHER_WATERHOLE_H_

