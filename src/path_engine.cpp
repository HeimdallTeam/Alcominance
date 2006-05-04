// pathfinder2.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "path_engine.h"


#define btdeep 7
#define maxpocetbodu 20 //pocetbodu pro pravy a levy body


typedef struct{
	long x[maxpocetbodu],y[maxpocetbodu],vzdalenost[maxpocetbodu];
	int pocet;
}bodv;
typedef struct{
	long x,y;
} bd;
typedef struct{
	long x,y,vzdalenost;
} bd2;
typedef struct{
	int pocetcar;
	bd bod[40];
} obj;
typedef struct{
	int pocetobjektu;
	obj objekt[1000];
} ob;
typedef struct{
	int objekt,pocetkolizi;
	long px,py;
}kol;
ob kmp; 
bd2 bt[128]; //btdeep = 7
long minvzdalenost;

int loadkmp(char *kmpfile){
FILE *soubor;
unsigned char bajt,bajt1,bajt2,bajt3;
int n,m;
if((soubor=fopen(kmpfile,"rb"))==NULL){
 printf("filewrk error");
 return -1;
}
bajt1=getc(soubor);
bajt2=getc(soubor);
kmp.pocetobjektu=bajt1*256+bajt2;
printf("pocetobjektu>>%d",kmp.pocetobjektu);
for(n=0;n<kmp.pocetobjektu;n++){
	bajt1=getc(soubor);
	bajt2=getc(soubor);
	kmp.objekt[n].pocetcar=bajt1*256+bajt2;
    printf("\nobjekt %d pocetcar %d ",n,kmp.objekt[n].pocetcar);
	for(m=0;m<=kmp.objekt[n].pocetcar;m++){
		bajt1=getc(soubor);
		bajt2=getc(soubor);
		bajt3=getc(soubor);
		kmp.objekt[n].bod[m].x=bajt1*65536+bajt2*256+bajt3;
		printf("%d,",kmp.objekt[n].bod[m].x);
		bajt1=getc(soubor);
		bajt2=getc(soubor);
		bajt3=getc(soubor);
		kmp.objekt[n].bod[m].y=bajt1*65536+bajt2*256+bajt3;
		printf("%d ",kmp.objekt[n].bod[m].y);
	}
}
fclose(soubor);
return 0;
}

long vzd(long xv1,long yv1,long xv2,long yv2){
return (long)sqrt((float)((xv1-xv2)*(xv1-xv2)+(yv1-yv2)*(yv1-yv2)));
}

kol kolize(long xt1,long yt1,long xt2,long yt2){
 kol klz;
 int i,k,n,m,pocetkolizi;
 long x1,y1,x2,y2,xc1,yc1,xc2,yc2,hy1,hy2,hym1,hym2,pom,minpx,minpy;
 float atmp,btmp,a,b,px,py;

	x1=xt1;y1=yt1;x2=xt2;y2=yt2;
	if(y1>y2){hy1=y2;hy2=y1;} else {hy1=y1;hy2=y2;}
	if(x1>x2){
		pom=x1;x1=x2;x2=pom;
		pom=y1;y1=y2;y2=pom;
	}
	if(x2-x1>2){
		atmp=(float)(y2-y1)/(float)(x2-x1);
		btmp=y1-atmp*x1;
	}
	k=-1;i=0;minpx=32766;minpy=32766;
	pocetkolizi=0;
	for(n=0;n<kmp.pocetobjektu;n++){
		for(m=0;m<=kmp.objekt[n].pocetcar-1;m++){
		xc1=kmp.objekt[n].bod[m].x;
		yc1=kmp.objekt[n].bod[m].y;
		xc2=kmp.objekt[n].bod[m+1].x;
		yc2=kmp.objekt[n].bod[m+1].y;

		if(yc1>yc2){hym1=yc2;hym2=yc1;} else{hym1=yc1;hym2=yc2;}
		if(xc1>xc2){
			pom=xc1;xc1=xc2;xc2=pom;
			pom=yc1;yc1=yc2;yc2=pom;
		}

		if(hym2>hy1 && hym1<hy2)
			if(xc2>x1 && xc1<x2)
				if(!(x1==x2 && xc1==xc2)){
					if(xc2-xc1<=2){
						py=atmp*xc1+btmp;
						if(x1<xc1 && x2>xc1 && py>hym1 && py<hym2){
							if(abs(xc1-xt1)<minpx){minpx=abs(xc1-xt1);k=n;}
							pocetkolizi++;
							if(pocetkolizi==1){
								klz.px=(py-btmp)/atmp;
								klz.py=py;
							}
						}
					}
					else{
						a=(float)(yc2-yc1)/(float)(xc2-xc1);
						b=yc1-a*xc1;

						if(x2-x1<=2){
							py=a*x1+b;
							if(xc1<x1 && xc2>x1 && py>hy1 && py<hy2){
								if(abs(py-yt1)<minpy){minpy=abs(py-yt1);k=n;}
								pocetkolizi++;
								if(pocetkolizi==1){
								    klz.px=(py-b)/a;
								    klz.py=py;
								}
							}
						}
						else{
							if(a!=atmp && b!=btmp){
								if(!(fabs(a-atmp)<.005)){
									px=(b-btmp)/(atmp-a);
									if((int)px>xc1 && (int)(px+.5)<xc2){
										if(px>x1 && px<x2){
											if(abs(px-xt1)<minpx){minpx=abs(px-xt1);k=n;}
											pocetkolizi++;
											if(pocetkolizi==1){
												klz.px=px;
												klz.py=atmp*px+btmp;
											}
										}
									}
								}
							}
						}
					}
				}
			
		 
		} // konec for m
	} //konec for n
	klz.objekt=k;
	klz.pocetkolizi=pocetkolizi;
return klz;
}

void path(long startx,long starty,long endx,long endy,int bindex,int lprekazka){
	//if(startx==endx && starty==endy) return NULL;
	long vzdalenost;
	long sx,sy,ex,ey,x,y;
	kol klz;
	int blindex,brindex,d,m,n,i,k,kl,kr,prekazka,firstlk,firstrk,di,predchudce;
	int lfronta[20],rfronta[20];
	float a,b;
	bodv maxleft,maxright;
	bd levejbod,pravejbod;
	bool prava,leva;

	char c;

	if(startx==bt[0].x && starty==bt[0].y){
		//bindex=1;
		minvzdalenost=32766;
		//for(n=0;n<20;n++){lfronta[n]=0;rfronta[n]=0;}
		memset(&lfronta[0],0,sizeof(lfronta));
		memset(&rfronta[0],0,sizeof(rfronta));
	}
//while(!kbhit());c=getch();
	klz=kolize(startx,starty,endx,endy);
	prekazka=klz.objekt;
	sx=startx;sy=starty;ex=endx;ey=endy;
	while(prekazka!=-1){
		//printf("|||%d %d|",lprekazka,prekazka);
		
		//printf("\nbindex %d",bindex);
		//body nejvic vpravo a nejvic vlevo
				levejbod.x=-1;levejbod.y=-1;
				pravejbod.x=-1;pravejbod.y=-1;
				maxleft.pocet=0;maxright.pocet=0;
zpatky:
		if(sx>ex) di=-1; else di=1;
		if(sx!=ex){
			if(di==1) a=(float)(ey-sy)/(float)(ex-sx);
			if(di==-1) a=(float)(sy-ey)/(float)(sx-ex);
			b=sy-a*sx;
		}
		prava=true;
		leva=true;
		for(d=0;d<maxpocetbodu;d++){
			maxleft.x[d]=0;maxleft.y[d]=0;maxleft.vzdalenost[d]=-1;
			maxright.x[d]=0;maxright.y[d]=0;maxright.vzdalenost[d]=-1;
		}
		//maxleft.pocet=0;maxright.pocet=0;
		//memset(&maxleft,0,sizeof(maxleft));
		//memset(&maxright,0,sizeof(maxright));
		
		
		for(n=0;n<=kmp.objekt[prekazka].pocetcar;n++){
			x=kmp.objekt[prekazka].bod[n].x;
			y=kmp.objekt[prekazka].bod[n].y;
			if(!(n==kmp.objekt[prekazka].pocetcar && (x==kmp.objekt[prekazka].bod[0].x && y==kmp.objekt[prekazka].bod[0].y))){
			if(x!=startx || y!=starty){
				vzdalenost=di*(y-(a*x+b));
				if(abs(ex-sx)<=2) vzdalenost=ex-x;
				if(vzdalenost<=0){
					i=0;
					maxleft.pocet++;
					while(maxleft.vzdalenost[i]>=abs(vzdalenost)) i++;
						
					for(m=maxleft.pocet-1;m>i;m--){
						maxleft.x[m]=maxleft.x[m-1];
						maxleft.y[m]=maxleft.y[m-1];
						maxleft.vzdalenost[m]=maxleft.vzdalenost[m-1];
					}
					maxleft.x[i]=x;maxleft.y[i]=y;
					maxleft.vzdalenost[i]=abs(vzdalenost);
					
				}
				if(vzdalenost>0){
					i=0;
					maxright.pocet++;
					while(maxright.vzdalenost[i]>=vzdalenost) i++;

					for(m=maxright.pocet-1;m>i;m--){
						maxright.x[m]=maxright.x[m-1];
						maxright.y[m]=maxright.y[m-1];
						maxright.vzdalenost[m]=maxright.vzdalenost[m-1];
					}
					maxright.x[i]=x;maxright.y[i]=y;
					maxright.vzdalenost[i]=vzdalenost;
				}
			}
			}
		} //konec for n
		
		if(bindex!=1) predchudce=(int)bindex/2; else predchudce=1;

		n=-1;kl=-1;

		do{			
			n++;
			if(n>maxleft.pocet-1){leva=false;break;}
			kl=kolize(startx,starty,maxleft.x[n],maxleft.y[n]).objekt;
			if(n==0) firstlk=kl;			
		}while(kl!=-1 || (maxleft.x[n]==bt[predchudce-1].x && maxleft.y[n]==bt[predchudce-1].y));
		if(leva==true){levejbod.x=maxleft.x[n];levejbod.y=maxleft.y[n];}
		if(levejbod.x!=-1 && levejbod.y!=-1) leva=true;

		n=-1;kr=-1;

		do{
			n++;
			if(n>maxright.pocet-1){prava=false;break;}
			kr=kolize(startx,starty,maxright.x[n],maxright.y[n]).objekt;
			if(n==0) firstrk=kr;			
		}while(kr!=-1 || (maxright.x[n]==bt[predchudce-1].x && maxright.y[n]==bt[predchudce-1].y));
		if(prava==true){pravejbod.x=maxright.x[n];pravejbod.y=maxright.y[n];}
		if(pravejbod.x!=-1 && pravejbod.y!=-1) prava=true;
		//kdyz se sem vrati podruhy (treba kvuli nedostupnosti levyho), tak se prava
		//musi prenastavit z false (podminka v cyklu) na true

		if(leva==false && kl!=-1){
		 prekazka=firstlk;
		 sx=maxleft.x[0];
		 sy=maxleft.y[0];
		 ex=startx;
		 ey=starty;
		 maxright.pocet=-64; //nastaveno tak, aby rightsmycka uz podruhy neprobehla
		 maxleft.pocet=0;
		 goto zpatky;
		 // nemuze se dostat do zadneho nejvic vlevo bodu kvuli objektu
		}
		
		if(prava==false && kr!=-1){
		 prekazka=firstrk;
		 sx=maxright.x[0];
		 sy=maxright.y[0];
		 ex=startx;
		 ey=starty;
		 maxleft.pocet=-64; //nastaveno tak, aby rightsmycka uz podruhy neprobehla
		 maxright.pocet=0;
		 goto zpatky;
		 // nemuze se dostat do zadneho nejvic vlevo bodu kvuli objektu
		}
		

		if(leva==true){
			blindex=bindex*2;
			//printf("\npristupny levy bod %d %d %d",blindex,levejbod.x,levejbod.y);
			if(blindex<128){ //mimo strom
				vzdalenost=bt[bindex-1].vzdalenost+vzd(bt[bindex-1].x,bt[bindex-1].y,levejbod.x,levejbod.y);
				if(vzdalenost>minvzdalenost) leva=false;
				else{
					bt[blindex-1].x=levejbod.x;
					bt[blindex-1].y=levejbod.y;
					bt[blindex-1].vzdalenost=vzdalenost;
					for(n=btdeep;n>0;n--) lfronta[n]=lfronta[n-1];
					lfronta[0]=blindex;
				}
			}
		}

		
		if(prava==true){
			brindex=bindex*2+1;
			//printf("\npristupny pravy bod %d %d %d",brindex,pravejbod.x,pravejbod.y);
			if(brindex<128){ //mimo strom
				vzdalenost=bt[bindex-1].vzdalenost+vzd(bt[bindex-1].x,bt[bindex-1].y,pravejbod.x,pravejbod.y);
				if(vzdalenost>minvzdalenost) prava=false;
				else{
					bt[brindex-1].x=pravejbod.x;
					bt[brindex-1].y=pravejbod.y;
					bt[brindex-1].vzdalenost=vzdalenost;
					for(n=btdeep;n>0;n--) rfronta[n]=rfronta[n-1];
					rfronta[0]=brindex;
				}
			}
		}
		if(leva==true){
			blindex=lfronta[0];
			for(n=0;n<btdeep;n++) lfronta[n]=lfronta[n+1];
			lfronta[btdeep]=0;
			if(blindex!=0){
				bindex=blindex;
				printf("\njedu doleva %d %d  %d",bt[blindex-1].x,bt[blindex-1].y,bindex); 
				path(bt[blindex-1].x,bt[blindex-1].y,endx,endy,bindex,prekazka);
			}
		}
		if(prava==true){
			brindex=rfronta[0];
			for(n=0;n<btdeep;n++) rfronta[n]=rfronta[n+1];
			rfronta[btdeep]=0;
			if(brindex!=0){
				bindex=brindex;
				printf("\njedu doprava %d %d  %d",bt[brindex-1].x,bt[brindex-1].y,bindex); 
				path(bt[brindex-1].x,bt[brindex-1].y,endx,endy,bindex,prekazka);
			}
		}

    return;
	}//konec while prekazka
	printf("->prima cesta do %d %d",endx,endy);
	vzdalenost=bt[bindex-1].vzdalenost+vzd(startx,starty,endx,endy);
	if(vzdalenost<minvzdalenost){
		printf("nova minvzdalenost %d",vzdalenost);
		minvzdalenost=vzdalenost;
		bt[bindex-1].vzdalenost=vzdalenost;
	}	
	
}

cesta & pathfinder(long startx,long starty,long &endx,long &endy){
	int i,n;
	static cesta c;
	for(n=0;n<128;n++){
		bt[n].x=0;
		bt[n].y=0;
		bt[n].vzdalenost=0;
	}
	bt[0].x=startx;bt[0].y=starty;

	path(startx,starty,endx,endy,1,-1);
	printf("\n===================");
	printf("\n%d %d<-",endx,endy);
	i=0;
	for(n=127;n>=0;n--){
		if(bt[n].vzdalenost==minvzdalenost){
			//i=(int)log2(n);   // logaritmus se zakladem 2 !!!
			n++;
			while(n>0){
				c.x[i]=bt[n-1].x;
				c.y[i]=bt[n-1].y;
				printf("%d %d",c.x[i],c.y[i]);
				if(n>1)printf("<-");
				i++;
				n=n/2;
			}
			break;
		}
	}
	c.vzdalenost=minvzdalenost;
	c.pocetkroku=i;
	printf("\nvzdalenost %d pocetkroku %d",minvzdalenost,i);
	printf("\n===================");
	return c;	
}
/*
int main(int argc, char* argv[]){
	
	cesta pesinka;
	loadkmp("c:\\audio\\becher\\testkmp2.kmp");
	//printf("\nkolize s objektem %d",kolize(57,133,200,100));
	//printf("\nkolize s objektem %d",kolize(120,316,243,411));
	//pesinka=pathfinder(200,100,600,150);
	//pesinka=pathfinder(200,100,485,370);
	
	pesinka=pathfinder(200,100,173,420);
	
return 0;
}*/
