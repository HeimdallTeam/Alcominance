
#include "StdAfx.h"
#include "becher.h"
#include "obj_stonemine.h"
#include "troll.h"

static CVar v_numzpr("stone_mine", 1.f, TVAR_SAVE); // rychlost zpracovani jedne davky (davek / vterina)
static CVar v_numworks("stone_maxwork", 2, TVAR_SAVE);
static CVar v_cena("stone_cost", 150, TVAR_SAVE); // cena za stavbu
static CVar v_build("stone_build", "1.4:K1+D1=0.011", TVAR_SAVE); // recept pro staveni

/*
static CVar v_cena("stone_cost_wood", 60, TVAR_SAVE); // pocet dreva potrebneho na stavbu
static CVar v_cena("stone_cost_stone", 60, TVAR_SAVE); // pocet kameni potrebneho na stavbu
*/

////////////////////////////////////////////////////////////
StoneMine::StoneMine(IHoeScene * scn) : SourceBuilding(scn, v_build), m_stone(EBS_Stone)
{
	SetModel((IHoeModel*)GetResMgr()->ReqResource(model_STONEMINE));
	m_stone.SetOwner(this); CRR::Get()->Register(&m_stone);
	m_stone.SetPriority(EBSP_TimeWork);
	m_stone.SetNum(101);
}

bool StoneMine::Save(ChunkDictWrite &w)
{
	BecherBuilding::Save(w);
	return true;
}

bool StoneMine::Load(const ChunkDictRead &r)
{
	SourceBuilding::Load(r);
	OnUpdateSur();
	return true;
}

int StoneMine::GetInfo(int type, char * str, size_t n)
{
	register int ret = 0;
	if (type==BINFO_Custom && str)
	{
		type = DefaultCustomInfo(str);
	}
	switch (type)
	{
	case BINFO_NumSur|EBS_Stone:
		ret = m_stone.GetNum();
		break;
	default:
		return SourceBuilding::GetInfo(type, str, n);
	};
	return 0;
	if (str)
		snprintf(str, n, "%d", ret);
	return ret;
}

int StoneMine::GameMsg(int msg, int par1, void * par2, uint npar2)
{
#ifndef BECHER_EDITOR
	switch (msg)
	{
	case BMSG_Select:
		GetLevel()->GetPanel()->SetObjectHud("scripts/mine.menu",this);
		GetLua()->func("s_stonemine");
		break;
	case BMSG_GetSur:
        return -1;
	}
#endif
	return SourceBuilding::GameMsg(msg, par1, par2, npar2);
}

#ifndef BECHER_EDITOR

void StoneMine::Update(const float dtime)
{
    m_line.Update(dtime * v_numzpr.GetFloat(), m_stone, v_numworks.GetInt());

}

bool StoneMine::Select()
{
	//SourceBuilding::Select();
	GetLua()->func("s_stonemine");
	return true;
}

#else // BECHER_OBJECT
void StoneMine::Update(const float t)
{
}

bool StoneMine::Select()
{
	//SourceBuilding::Select();
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Resources"));
	GetProp()->AppendLong(1, _("Stone"), m_stone.GetNum());
	GetProp()->End();
	return true;
}

/*void StoneMine::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 1:
		m_stone.SetNum((int)pi.GetLong());
		break;
	};
}*/


#endif // BECHER_OBJECT



