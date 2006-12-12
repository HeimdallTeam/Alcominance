
#pragma once

// BMSG_Go - prikaz jdi
// float x, float y

// funkce pro zpravy
int SendGameMsg(BecherObject * o, int msg, void * param, uint nump);
int SendGameMsg(unsigned long id, int msg, void * param, uint nump);

