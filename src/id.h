
/**
   @file    id.h
   @date    Sep 2004
   @version $Revision: 1.6 $
   @brief   ID vsech objektu v BecherGame
*/
/*
	$Id: id.h,v 1.6 2004/09/22 22:24:26 root Exp $
	$Author: root $
	$Date: 2004/09/22 22:24:26 $
	$Revision: 1.6 $
*/ 

#ifndef _ID_H_
#define _ID_H_

#define ADDID(id) id,

enum {
#include "../scripts/id.txt"
};

#undef ADDID

enum EObjType
{
	// pridavat vzdy az na konec!!!
	EBO_None = 0,
	EBO_Tree = 1, // class
	EBO_Sugar,
	EBO_HerbeWoman,
	EBO_Farm,
	EBO_Bridge, // class
	EBO_Destilate,
	EBO_Troll,
	EBO_Factory,
	EBO_WaterHole,
	EBO_Store,
	EBO_Shop,
	EBO_Saw,
	EBO_CoalMine,
	EBO_StoneMine,
	EBO_Max,    

	// add on
    EBAO_Stones1 = 400,
    EBAO_Stones2,
    EBAO_Anthill,
    EBAO_Row,
	EBAO_Max,

	// system
	EBSys_Sound = 500,
	EBSys_Place,
	EBSys_Water,
	EBSys_Coal,
	EBSys_Stone,
	EBSys_Max,

// class
	EBC_Building = 1000,
    EBC_Addon, // class
	EBC_System,
	EBC_Max,

};

#define assert_obj(n) assert(n > EBO_None && n < EBO_Max)
#define assert_sysobj(n) assert(n >= EBS_Sound)

enum ESurType
{
	EBS_None = 0,
	EBS_Becher, // B
	EBS_Stone, // K
	EBS_Wood,  // D
	EBS_Water, // W
	EBS_Herbe, // H
	EBS_Sugar, // S
	EBS_Alco, // A
	EBS_Cane, // C
    EBS_Coal, // U
	EBS_Max,
};

static const char s_sur[] = "_BKDWHSACU_";

enum EBuildingMode
{
	EBM_Hide,
	EBM_Select,
	EBM_Build,
	EBM_Normal,
	EBM_None,
	EBM_ToDelete,
	EBM_Edit,
};

enum ESurPriority
{
	// nevydavat
	// spis zanechat
	// normalni
	// zbavit se
	EBSP_Lowest,
    EBSP_TimeWork,
	EBSP_Normal,
	EBSP_High,
	EBSP_Higher,
};

enum EWorkType
{
// job list
	EBW_Work,
	EBW_ImportStart,
	EBW_ExportStart=EBW_ImportStart+EBS_Max,
	EBW_Wait=EBW_ExportStart+EBS_Max,
	EBW_GoIn,
    EBW_Max,
};

#define EBW_Import(sur) ((EWorkType)(EBW_ImportStart+(sur)))
#define EBW_Export(sur) ((EWorkType)(EBW_ExportStart+(sur)))

extern HoeGame::LuaPreprocess::IDConst g_luaconst[];

const char * FindIDString(int id);

#endif // _ID_H_

