
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

#define ADD_EDGEID(id,num) id=num,
#define ADD_ID(id) id,

enum {
#include "../scripts/id.txt"
};

#undef ADD_EDGEID
#undef ADD_ID

enum EObjType
{
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
	EBO_Max,

// class
	EBC_Building = 100,
	EBC_Max,

	// system
	EBS_Sound = 1000,
	EBS_Max
};

enum ESurType
{
	EBS_None = 0,
    EBS_Work,
	EBS_Becher,
	EBS_Stone,
	EBS_Wood,
	EBS_Water,
	EBS_Herbe,
	EBS_Sugar,
	EBS_Alco,
	EBS_Cane,
	EBS_Unknown,
};

enum ESurCriterion{
    EBSC_Distance = 0,
    EBSC_Priority,
};

extern HoeGame::LuaPreprocess::IDConst g_luaconst[];

const char * FindIDString(int id);

#endif // _ID_H_

