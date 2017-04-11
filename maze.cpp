#include <stdio.h>
#include "vector2.hpp"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "maze.hpp"
#include "vector3.hpp"
#include "graphiclib.hpp"

const vector2_t cdir[4] = {
  {0,1},
  {1,0},
  {0,-1},
  {-1,0},
};

const vector2_t cdoubledir[4] = {
  {0,2},
  {2,0},
  {0,-2},
  {-2,0},
};

static
void wait60thsec(int s60){
  for(int i=0;i<s60;i++){
    while(drawing);
    while(!drawing);
  }
}

inline
vector2 operator+(const vector2& v1,const vector2_t& v2){
  return vector2(v1.x+v2.x,v1.y+v2.y);
}

inline
vector2 operator-(const vector2& v1,const vector2_t& v2){
  return vector2(v1.x-v2.x,v1.y-v2.y);
}


inline
vector2 tovector2(vector2_t v){
  return vector2(v.x,v.y);
}

inline
vector2_t tovector2_t(const vector2& v){
  vector2_t r;
  r.x = v.x;
  r.y=v.y;
  return r;
}

unsigned long myRand(int seed=0){ 
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123; 
    unsigned long t;
    if(seed){
      x = seed<<10;
    }
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) ); 
} 

enum mape{
  WAY=0x0,
  WALL=0x1,
};


void map::printmap(graphiclib& g){
  vector2 p;
  vector2 sp;
  vector2 t;
  vector2 dp;
  
  for(p.y=0;p.y<size.y;p.y++){
    for(sp.y=-1;sp.y <= 1;sp.y++){
      for(p.x=0;p.x<size.x;p.x++){
	for(sp.x=-1;sp.x <= 1;sp.x++){
	  t = p+sp;
	  if(*loadmap(p)==WAY||sp.x*sp.x+sp.y*sp.y==2){
	    video::pset(dp.x,dp.y,15);
	  }
	  else if(isinmap(t)&&*loadmap(t)==WALL){
	    video::pset(dp.x,dp.y,0);
	  }else{
	    video::pset(dp.x,dp.y,15);
	  }
	  dp.x++;
	}
      }
      dp.y++;
      dp.x=0;
    }
  }
}

void map::dig(vector2& p,graphiclib& g){
  vector2 desti;
  int c;
  int r;
  int *pm;

  c = 0;
  r = myRand();
  while(c < 4){
    desti = p+cdoubledir[(c+r)&0x3];
    if(isinmap(desti)&&*(pm=loadmap(desti))==WALL){
      *pm=WAY;
      *loadmap(desti-cdir[(c+r)&0x3])=WAY;
      c=0;
      r=myRand();
      p = desti;
      printmap(g);
    }else{
      c++;
    }
  }
}

vector2 map::calcstart(vector2_t candi[CANDIDI_MAX]){
  vector2 p;
  vector2 pa;
  vector2 ans;
  int num=0;
  
  for(p.y=1;p.y<size.y;p.y+=2){
    for(p.x=1;p.x<size.x;p.x+=2){
      
      if(*loadmap(p)==WAY){
	for(int i=0;i<4;i++){
	  pa = p+cdoubledir[i];
	  if(isinmap(pa)&&*loadmap(pa)==WALL){
	    candi[num++] = tovector2_t(p);
	    break;
	  }
	}
      }
    }
  }

  if(num){
    ans = tovector2(candi[myRand()%num]);
  }
  else{
    //EOM(End Of Map)
    ans = vector2(0,0);
  }
  return ans;
}

void map::initmap(){
  for(int x=0;x<size.x;x++){
    for(int y=0;y<size.y;y++){
      //fast access :)
      map[y][x] = WALL;
    }
  }
}

// void map::printmap_s(){
//   vector2 p;
  
//   for(p.y=0;p.y<size.y;p.y++){
//     for(p.x=0;p.x<size.x;p.x++){
//       if(*loadmap(p)==WALL){
// 	printf("\033[41m  ");
//       }else{
// 	printf("\033[49m  ");
//       }
//     }
//     puts("");
//   }
// }

vector2_t candidi[CANDIDI_MAX];
void map::mazemake(int seed,const vector2& sm,graphiclib& g){
  vector2 start=vector2(1,1);

  size = sm;

  myRand(seed);
  initmap();
  printmap(g);
  dig(start,g);

  while(1){
    start = calcstart(candidi);
    if(start.x==0)break;
    dig(start,g);
    printmap(g);
  }
  wait60thsec(120);
  //  printmap_s(size,map);
}


const int drawarea=8;
int map::genepoints(vector3_t *pv,const vector3& lookat){
  int i=0;
  vector2 s;

  s.x = lookat.z/65536/2+size.x/2-drawarea;
  s.y = lookat.x/65536/2+size.y/2-drawarea;
  //s.print();
  //puts("");

  for(int x=0;x<drawarea*2;x++){
    for(int y=0;y<drawarea*2;y++){
      pv[i].x   =  (x-(size.x+1)/2+s.y)*131072;
      pv[i].y   = -21845;
      pv[i++].z =  (y-size.y/2+s.x)*131072;
      //printf("%d,%d,%d\n",pv[i].x,pv[i].y,pv[i].z);
      pv[i].x   =  (x-(size.x+1)/2+s.y)*131072;
      pv[i].y   =  21845;
      pv[i++].z =  (y-size.y/2+s.x)*131072;
    }
  }

  return i;
}

int map::genepoly(int polyvec[][3],const vector3& lookat){
  vector2 v;
  vector2 vv;
  vector2 vd;
  int i=0;

  vector2 s;

  s.x = lookat.z/65536/2+size.x/2-drawarea;
  s.y = lookat.x/65536/2+size.y/2-drawarea;

  for(vv.x=0;vv.x<drawarea*2-1;vv.x++){
    for(vv.y=0;vv.y<drawarea*2-1;vv.y++){
      v = vv + s;
      if(!isinmap(v)||*loadmap(v)!=WALL)continue;
      vd = v + vector2(1,0);
      if(isinmap(vd)&&*loadmap(vd)==WALL){
	polyvec[i][0]=(vv.x+vv.y*(drawarea*2))*2+0;
	polyvec[i][1]=(vv.x+vv.y*(drawarea*2))*2+1;
	polyvec[i++][2]=(vv.x+vv.y*(drawarea*2))*2+2;
	polyvec[i][0]=(vv.x+vv.y*(drawarea*2))*2+1;
	polyvec[i][1]=(vv.x+vv.y*(drawarea*2))*2+3;
	polyvec[i++][2]=(vv.x+vv.y*(drawarea*2))*2+2;
      }
      vd = v + vector2(0,1);
      if(isinmap(vd)&&*loadmap(vd)==WALL){
	polyvec[i][0]=(vv.x+vv.y*(drawarea*2))*2+0;
	polyvec[i][1]=(vv.x+vv.y*(drawarea*2))*2+1;
	polyvec[i++][2]=(vv.x+(vv.y+1)*(drawarea*2))*2;
	polyvec[i][0]=(vv.x+(vv.y+1)*(drawarea*2))*2+1;
	polyvec[i][1]=(vv.x+vv.y*(drawarea*2))*2+1;
	polyvec[i++][2]=(vv.x+(vv.y+1)*(drawarea*2))*2;
      }
    }
  }
  return i;
}

// int map::genepoints(vector3_t *pv){
//   int i=0;
//   for(int x=0;x<size.x+1;x++){
//     for(int y=0;y<size.y+1;y++){
//       pv[i].x=(x-(size.x+1)/2)*131072;pv[i].y=-21845;pv[i++].z=(y-size.y/2)*131072;
//       pv[i].x=(x-(size.x+1)/2)*131072;pv[i].y=21845;pv[i++].z=(y-size.y/2)*131072;
//     }
//   }
//   return i;
// }

// int map::genepoly(int polyvec[][3]){
//   vector2 v;
//   vector2 vd;
//   int i=0;

//   for(v.x=0;v.x<size.x;v.x++){
//     for(v.y=0;v.y<size.y;v.y++){
//       if(*loadmap(v)!=WALL)continue;
//       vd = v + vector2(1,0);
//       if(isinmap(vd)&&*loadmap(vd)==WALL){
// 	polyvec[i][0]=(v.x+v.y*(size.x+1))*2+0;polyvec[i][1]=(v.x+v.y*(size.x+1))*2+1;polyvec[i++][2]=(v.x+v.y*(size.x+1))*2+2;
// 	polyvec[i][0]=(v.x+v.y*(size.x+1))*2+1;polyvec[i][1]=(v.x+v.y*(size.x+1))*2+3;polyvec[i++][2]=(v.x+v.y*(size.x+1))*2+2;
//       }
//       vd = v + vector2(0,1);
//       if(isinmap(vd)&&*loadmap(vd)==WALL){
// 	polyvec[i][0]=(v.x+v.y*(size.x+1))*2+0;polyvec[i][1]=(v.x+v.y*(size.x+1))*2+1;polyvec[i++][2]=(v.x+(v.y+1)*(size.x+1))*2;
// 	polyvec[i][0]=(v.x+(v.y+1)*(size.x+1))*2+1;polyvec[i][1]=(v.x+v.y*(size.x+1))*2+1;polyvec[i++][2]=(v.x+(v.y+1)*(size.x+1))*2;
//       }
//     }
//   }
//   return i;
// }
