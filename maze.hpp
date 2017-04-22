#ifndef __MAZE_H
#define __MAZE_H

#define MAX_X 59
#define MAX_Y 59

#define CANDIDI_MAX (((MAX_X-1))*((MAX_Y-1))/10)

const int size_grid = 65536*1.6;
const int size_height = 65536*0.5;
#include "vector3.hpp"
#include "vector2.hpp"
#include "graphiclib.hpp"
#include <stdint.h>

struct vector2_t{
  int16_t x;
  int16_t y;
};

enum mape{
  WAY=0x0,
  WALL=0x1,
};

class map{
private:
  vector2 size;

  uint8_t* loadmap(const vector2& p){
    assert(isinmap(p));
    return &map[p.y][p.x];
  }
  
  void dig(vector2& p,graphiclib& g);
  vector2 calcstart(vector2_t candi[CANDIDI_MAX]);
  void initmap();
  void keytask();
public:
  uint8_t map[MAX_Y][MAX_X];
  int isinmap(const vector2 &p){
    return p.x>=0&&p.y>=0&&p.x<size.x&&p.y<size.y;
  }
  int genepoly(int polyvec[][3],const vector3& lookat);
  int genepoints(vector3_t *pv,const vector3& lookat);

  vector2 calcpos(const vector3& lookat,int n);
  void printmap(graphiclib& g);
  void printmap(graphiclib& g,const vector2& pd);
  void printmap(graphiclib& g,const vector2& pd,int r);
  void printmap_s(const vector2& dp);
  void mazemake(int seed,const vector2& size,graphiclib& g);

  vector2 getsize(){return size;}
};

#endif
