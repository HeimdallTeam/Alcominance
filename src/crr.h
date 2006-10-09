#ifndef _CRR_H_
#define _CRR_H_

#include "workspace.h"

class BecherObject;

/**
 * Central Register of Resource
 * Centralni registr zdroju (surovin) slouzi ...
 */
class CRR 
{
private:
    static CRR* this_;
	HoeCore::Set<ResourceExp*> m_items[EBS_Max];
public:
    CRR();
    ~CRR();
	static CRR * Get();
    void Register(ResourceExp* item);
    void Unregister(ResourceExp* item);
	/**
	* Najit podle priority, obsahu, vzdalenosti
	*/
	ResourceExp * Find(ESurType type, BecherObject * owner);
	BecherBuilding * FindAccept(ESurType type, BecherObject * owner);
};


#endif