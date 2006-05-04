
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "troll.h"
#include "obj_factory.h"
#include "obj_store.h"

////////////////////////////////////////////////////////

Factory::Factory(IHoeScene * scn) : FactoryBuilding(scn)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(ID_FACTORY));
	GetCtrl()->SetFlags(HOF_ADVSHOW);
}


bool Factory::Save(HoeFileWriter &w)
{
	return BecherBuilding::Save(w);
	//return 0;
}

bool Factory::Load(int ver, HoeFileReader &r)
{
	return BecherBuilding::Load(ver, r);
}

void Factory::AdvPaint(IHoePaint3D * h3)
{
	// info
	/*if (this->IsCurActive() || this->IsSelected())
	{
		static IHoeModel * mod_i = dynamic_cast<IHoeModel*>(GetEngine()->Create("model info"));
		h3->ResetPos();
		h3->Scale(2.5, 0, 2.5);
		h3->Move(0,2,0);
		h3->Paint(mod_i);
	}

	// velikost skladu
	const float r[] = { -48,-2, -48 , -10};
	const float rx = (r[1]-r[0])/5.f;
	const float ry = (r[3]-r[2])/4.f;

	static IHoeModel * mod[] = {dynamic_cast<IHoeModel*>(GetEngine()->Create("model miska")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model sud")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model kblok")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_becher")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_cukr")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model bedna_dul")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model kmen")),
		dynamic_cast<IHoeModel*>(GetEngine()->Create("model vedro"))};
	int m = 0;
	for (int i=0;i<5;i++)
		for(int j=0;j<4;j++)
		{
			h3->ResetPos();
			//h3->RotateY(GetEngine()->SysFloatTime()*2);
			
			h3->Move(rx*i+r[0]+rx/2, 1, ry*j+r[2]+ry/2);
			h3->Paint(mod[(m++)%8]);
		}
	*/
}



#ifndef BECHER_EDITOR

bool Factory::Select()
{
	if (!IsBuildMode())
        GetLua()->func("s_tovarna");

	return true;
}

void Factory::Update(const double dtime)
{
}

bool Factory::Idiot(Troll *t)
{
	return false;
}

int Factory::GetSur(ESurType type, int req, bool upln)
{
	return 0;
}

int Factory::AddSur(ESurType type, int s)
{
	return 0;
}

int Factory::GetNumInfos()
{
	return 0;
}

int Factory::GetInfo(int id, char * buff, size_t n)
{
	return 0;
}

void Factory::AddToWork(Troll *t)
{
}

#else
bool Factory::Select()
{
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Testing"));
	GetProp()->AppendLong(0, _("test"), 3);
	GetProp()->End();
	return true;
}

#endif
