

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

// funkce pro zpravy
int SendGameMsg(BecherObject * o, int msg, void * param, uint nump);
int SendGameMsgId(unsigned long id, int msg, void * param, uint nump);

