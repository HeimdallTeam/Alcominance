
#include "StdAfx.h"
#include "becher.h"
#include "id.h"
#include "infos.h"

static THoeObjectInfo is[EIS_Max] = {0};

static void SetInfoRing(EInfoStruct s, float tx, float ty, float tz, float sx, float sy, float sz)
{
	assert(s >= 0 && s < EIS_Max);
	is[s].model = (IHoeModel*)GetResMgr()->ReqResource(ID_INFO_RING);
	is[s].position[0] = tx;
	is[s].position[1] = ty;
	is[s].position[2] = tz;
	is[s].scale[0] = sx;
	is[s].scale[1] = sy;
	is[s].scale[2] = sz;
}

static void SetInfoConst(EInfoStruct s, int idres)
{
	assert(s >= 0 && s < EIS_Max);
	IHoeModel* m = (IHoeModel*)GetResMgr()->ReqResource(idres);
	is[s].model = (IHoeModel*)GetResMgr()->ReqResource(ID_CONSTRUCT);
	THoeParameter pi,pm;
	if (m && m->GetParameter("boundbox",&pm) && is[s].model->GetParameter("boundbox",&pi))
	{
		is[s].position[0] = 0;
		is[s].position[1] = -5;
		is[s].position[2] = 0;
		is[s].scale[0] = (pm.box.right-pm.box.left+5)/(pi.box.right-pi.box.left)*1.05f;
		is[s].scale[1] = (pm.box.top-pm.box.bottom+10)/(pi.box.top-pi.box.bottom);
		is[s].scale[2] = (pm.box.back-pm.box.front+5)/(pi.box.back-pi.box.front)*1.05f;
	}
	else
	{
		is[s].position[0] = 0;
		is[s].position[1] = 0;
		is[s].position[2] = 0;
		is[s].scale[0] = 1;
		is[s].scale[1] = 1;
		is[s].scale[2] = 1;
	}

}

static void SetInfoConstD(EInfoStruct s, int idres)
{
	assert(s >= 0 && s < EIS_Max);
	IHoeModel* m = (IHoeModel*)GetResMgr()->ReqResource(idres);
	is[s].model = (IHoeModel*)GetResMgr()->ReqResource(ID_CONSTRUCT_D);
	THoeParameter pm,pi;
	if (m->GetParameter("boundbox",&pm) && is[s].model->GetParameter("boundbox",&pi))
	{
		is[s].position[0] = pm.box.left*1.06f;
		is[s].position[1] = 0;
		is[s].position[2] = pm.box.front*1.06f;
		is[s].scale[0] = (pm.box.right-pm.box.left+5)/(pi.box.right-pi.box.left)*0.3f;
		is[s].scale[1] = (pm.box.top-pm.box.bottom+10)/(pi.box.top-pi.box.bottom)*0.3f;
		is[s].scale[2] = (pm.box.back-pm.box.front+5)/(pi.box.back-pi.box.front)*0.3f;
	}
	else
	{
		is[s].position[0] = 0;
		is[s].position[1] = 0;
		is[s].position[2] = 0;
		is[s].scale[0] = 1;
		is[s].scale[1] = 1;
		is[s].scale[2] = 1;
	}

}

bool LoadInfos()
{
	// olaf
	SetInfoRing(EIS_TrollS, 0.f, 3.f, 0.f, 0.8f, 1.f, 0.8f); 
	SetInfoRing(EIS_TrollC, 0.f, 2.f, 0.f, 0.8f, 1.f, 0.8f); 
	SetInfoRing(EIS_TrollCS, 0.f, 5.f, 0.f, 0.8f, 1.f, 0.8f); 
	// strom
	SetInfoRing(EIS_TreeS, 0.f, 2.f, 0.f, 1.0f, 1.f, 1.0f); 
	SetInfoRing(EIS_TreeC, 0.f, 1.f, 0.f, 1.0f, 1.f, 1.0f); 
	SetInfoRing(EIS_TreeCS, 0.f, 3.f, 0.f, 1.0f, 1.f, 1.0f); 

	SetInfoRing(EIS_FarmS, 0.f, 2.f, 0.f, 4.5f, 1.f, 3.5f); 
	SetInfoRing(EIS_FarmC, 0.f, 1.f, 0.f, 4.5f, 1.f, 3.5f); 
	SetInfoRing(EIS_FarmCS, 0.f, 3.f, 0.f, 4.5f, 1.f, 3.5f); 

	SetInfoRing(EIS_DestilateS, 0.f, 2.f, 0.f, 4.5f, 1.f, 4.5f); 
	SetInfoRing(EIS_DestilateC, 0.f, 1.f, 0.f, 4.5f, 1.f, 4.5f); 
	SetInfoRing(EIS_DestilateCS, 0.f, 3.f, 0.f, 4.5f, 1.f, 4.5f); 

	SetInfoRing(EIS_SugarS, 0.f, 2.f, 0.f, 3.5f, 1.f, 3.5f); 
	SetInfoRing(EIS_SugarC, 0.f, 1.f, 0.f, 3.5f, 1.f, 3.5f); 
	SetInfoRing(EIS_SugarCS, 0.f, 3.f, 0.f, 3.5f, 1.f, 3.5f); 
	SetInfoConst(EIS_SugarConst, ID_SUGAR); 
	SetInfoConstD(EIS_SugarConstD, ID_SUGAR);

	SetInfoRing(EIS_StoreS, 0.f, 2.f, 0.f, 2.5f, 1.f, 2.5f); 
	SetInfoRing(EIS_StoreC, 0.f, 1.f, 0.f, 2.5f, 1.f, 2.5f); 
	SetInfoRing(EIS_StoreCS, 0.f, 3.f, 0.f, 2.5f, 1.f, 2.5f); 

	SetInfoRing(EIS_HerbeWomanS, 0.f, 2.f, 0.f, 1.4f, 1.f, 2.0f); 
	SetInfoRing(EIS_HerbeWomanC, 0.f, 1.f, 0.f, 1.4f, 1.f, 2.0f); 
	SetInfoRing(EIS_HerbeWomanCS, 0.f, 3.f, 0.f, 1.4f, 1.f, 2.0f); 

	SetInfoRing(EIS_FactoryS, 0.f, 2.f, 0.f, 6.7f, 1.f, 6.7f); 
	SetInfoRing(EIS_FactoryC, 0.f, 1.f, 0.f, 6.7f, 1.f, 6.7f); 
	SetInfoRing(EIS_FactoryCS, 0.f, 3.f, 0.f, 6.7f, 1.f, 6.7f); 
	SetInfoConst(EIS_FactoryConst, ID_FACTORY); 
	SetInfoConst(EIS_FactoryConstD, ID_FACTORY); 

	SetInfoRing(EIS_WaterHoleS, 0.f, 2.f, 0.f, 1.0f, 1.f, 1.0f); 
	SetInfoRing(EIS_WaterHoleC, 0.f, 1.f, 0.f, 1.0f, 1.f, 1.0f); 
	SetInfoRing(EIS_WaterHoleCS, 0.f, 3.f, 0.f, 1.0f, 1.f, 1.0f); 

	SetInfoRing(EIS_BridgeS, 0.f, 2.f, 0.f, 2.0f, 1.f, 4.0f); 
	SetInfoRing(EIS_BridgeC, 0.f, 1.f, 0.f, 2.0f, 1.f, 4.0f); 
	SetInfoRing(EIS_BridgeCS, 0.f, 3.f, 0.f, 2.0f, 1.f, 4.0f); 

	SetInfoRing(EIS_ShopS, 0.f, 2.f, 0.f, 1.5f, 1.f, 3.0f); 
	SetInfoRing(EIS_ShopC, 0.f, 1.f, 0.f, 1.5f, 1.f, 3.0f); 
	SetInfoRing(EIS_ShopCS, 0.f, 3.f, 0.f, 1.5f, 1.f, 3.0f); 

	return true;
}

THoeObjectInfo * GetInfoStruct(EInfoStruct str)
{
	if (is[str].model == NULL)
		LoadInfos();
	return &is[str];
}


