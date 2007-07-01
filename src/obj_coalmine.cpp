
#include "StdAfx.h"
#include "becher.h"
#include "obj_coalmine.h"
#include "troll.h"

static CVar v_numzpr("coal_mine", 0.5f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_cena("coal_cost" , 150, TVAR_SAVE);
/*
static CVar v_cena_drevo("coal_cost_wood" , 60, TVAR_SAVE);
static CVar v_cena_kamen("coal_cost_stone", 60, TVAR_SAVE);
*/
static CVar v_numworks("coal_maxwork", 3, TVAR_SAVE);
static CVar v_build("coal_build", "1.4:K1+D1=0.011", TVAR_SAVE); // recept pro staveni

////////////////////////////////////////////////////////////
CoalMine::CoalMine(IHoeScene * scn) : SourceBuilding(scn, v_build), m_coal(EBS_Coal)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_COALMINE));
	m_coal.SetOwner(this); CRR::Get()->Register(&m_coal);
	m_coal.SetPriority(EBSP_TimeWork);
	m_coal.SetNum(100);
}

bool CoalMine::Save(ChunkDictWrite &w)
{
	BecherBuilding::Save(w);
	m_coal.Save(w);
	return true;
}

bool CoalMine::Load(BecherGameLoad &r)
{
	BecherBuilding::Load(r);
	m_coal.Load(r);
	r.ReadReservedWords(9);
	return true;
}

void CoalMine::Update(const float dtime)
{
    m_line.Update(dtime * v_numzpr.GetFloat(), m_coal, v_numworks.GetInt());
}


int CoalMine::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
	case BINFO_NumSur|EBS_Coal:
		ret = m_coal.GetNum();
		if (str)
			snprintf(str, n, "%d", ret);
		return ret;
	default:
		return SourceBuilding::GetInfo(type, str, n);
	};
	return 0;
}

int CoalMine::GameMsg(int msg, int par1, void * par2, uint npar2)
{
#ifndef BECHER_EDITOR
	switch (msg)
	{
	case BMSG_Select:
		GetLevel()->GetPanel()->SetObjectHud("scripts/mine.menu",this);
		GetLua()->func("s_coalmine");
		break;
	case BMSG_GetSur:
        return -1;
	}
#endif
	return SourceBuilding::GameMsg(msg, par1, par2, npar2);
}

#ifndef BECHER_EDITOR

#else // BECHER_OBJECT

bool CoalMine::Select()
{
	//SourceBuilding::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Resources"));
	GetProp()->AppendLong(1, _("Coal"), m_coal.GetNum());
	GetProp()->End();	
	return true;
}

void CoalMine::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 1:
		m_coal.SetNum((int)pi.GetLong());
		break;
	};
}

#endif // BECHER_OBJECT



