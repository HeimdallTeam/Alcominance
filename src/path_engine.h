
#ifndef _PATH_ENGINE_H_
#define _PATH_ENGINE_H_

typedef struct{
	long x[40],y[40];
	long vzdalenost;
	int pocetkroku;
}cesta;


cesta &pathfinder(long startx,long starty,long endx,long endy);



#endif // _PATH_ENGINE_H_

