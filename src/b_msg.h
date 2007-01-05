

#pragma once

// BMSG_Go - prikaz jdi
// float x, float y

// BMSG_BuildPlace
// BMSG_Build
struct PAR_BuildPlace
{
	float x;
	float y;
	IHoeModel * m;
	float height;
	float dobj;
};

#ifndef BECHER_EDITOR
// funkce pro zpravy
int SendGameMsg(BecherObject * o, int msg, int par1=0, void * par2=NULL, uint nump=0);
int SendGameMsgId(unsigned long id, int msg, int par1=0, void * par2=NULL, uint nump=0);
#else
static int SendGameMsg(BecherObject * o, int msg, int par1=0, void * par2=NULL, uint nump=0) { return 0; }
static int SendGameMsgId(unsigned long id, int msg, int par1=0, void * par2=NULL, uint nump=0) { return 0; }
#endif

#define IDMSG_ALL 0xffffffff


