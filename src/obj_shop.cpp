
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_shop.h"
#include "obj_store.h"

/////////////////////////////////////////////////////////////
Shop::Shop(IHoeScene * scn) : BecherBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_SHOP));
}

#ifndef BECHER_EDITOR
void Shop::Update(const double t)
{
}

int Shop::AddSur(ESurType type, int s)
{
	
	//trtina += s;
	GetBecher()->GetCash()->Add(s);
	return 0;
}

bool Shop::Select()
{
	if (!IsBuildMode())
        GetLua()->func("s_shop");
	return true;
}

int Shop::GetSur(ESurType EBO_Shop, int req, bool upln)
{
	return 0;
}

void Shop::AddToWork(Troll *t)
{
}

bool Shop::Idiot(Troll * t)
{
	// vybrat idealni job
	// zjistit zda je trtina, pokud ne, je to jasny
	// bud podle modu nebo toho co je potreba

	//t->
	//if (trtina < 5)
	{
		// najit sklad
		Store * s = NULL;
		Job job;
		job.owner = this;

		for (int i=0;i < GetBecher()->GetLevel()->GetNumObj();i++)
		{
			BecherObject * bo = GetBecher()->GetLevel()->GetObj(i);
			if (bo->GetType() != EBO_Store)
				continue;
			Store * ts = dynamic_cast<Store*>(bo);
			// pokud je lepsi nez ten druhy
			s = ts;
		}

		if (s != NULL && s->GetStatus(EBS_Alco) > 0)
		{
			job.type = Job::jtPrines;
			job.store = s;
			job.num = 10;
			job.surtype = EBS_Alco;
			t->SetJob(job);
			return true;			
		}
		// zarezervovat

		// predat job a registrovat
		//t->job.Prine

	}
	return false;
}

int Shop::GetNumInfos()
{
	return 0;
}

int Shop::GetInfo(int id, char *buff, size_t size)
{
	return 0;
}

#else
bool Shop::Select()
{
	return true;
}
#endif


