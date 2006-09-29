
#include "StdAfx.h"
#include "id.h"

#define REG_ID(id) id,#id,
#define ADDID(id) REG_ID(id)

HoeGame::LuaPreprocess::IDConst g_luaconst[] = {
#include "../scripts/id.txt"
	REG_ID(EBO_Tree)
	REG_ID(EBO_Sugar)
	REG_ID(EBO_HerbeWoman)
	REG_ID(EBO_Farm)
	REG_ID(EBO_Bridge)
	REG_ID(EBO_Destilate)
	REG_ID(EBO_Troll)
	REG_ID(EBO_Factory)
	REG_ID(EBO_WaterHole)
	REG_ID(EBO_CoalMine)
	REG_ID(EBO_StoneMine)
	REG_ID(EBO_Store)
	REG_ID(EBO_Shop)
	REG_ID(EBO_Saw)

	REG_ID(EBS_None)
	REG_ID(EBS_Becher)
	REG_ID(EBS_Stone)
	REG_ID(EBS_Wood)
	REG_ID(EBS_Water)
	REG_ID(EBS_Herbe)
	REG_ID(EBS_Sugar)
	REG_ID(EBS_Alco)
	REG_ID(EBS_Cane)
	REG_ID(EBS_Coal)
	0, NULL,
};

const char * FindIDString(int id)
{
	for (int i=0; g_luaconst[i].String != NULL;i++)
	{
		if (g_luaconst[i].ID == id)
			return g_luaconst[i].String;
	}
	return NULL;
}

