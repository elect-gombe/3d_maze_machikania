#ifndef __MAZE_H
#define __MAZE_H

#define MAX_X 39
#define MAX_Y 39

#define CANDIDI_MAX (((MAX_X-1))*((MAX_Y-1))/4)

#include "vector3.hpp"
#include "vector2.hpp"
#include "graphiclib.hpp"

struct vector2_t{
  int x;
  int y;
};

class map{
private:
  vector2 size;

  int isinmap(const vector2 &p){
    return p.x>=0&&p.y>=0&&p.x<size.x&&p.y<size.y;
  }
  int* loadmap(const vector2& p){
    assert(isinmap(p));
    return &map[p.y][p.x];
  }
  void dig(vector2& p,graphiclib& g);
  vector2 calcstart(vector2_t candi[CANDIDI_MAX]);
  void initmap();
public:
  int map[MAX_Y][MAX_X];
  int genepoly(int polyvec[][3],const vector3& lookat);
  int genepoints(vector3_t *pv,const vector3& lookat);

  void printmap(graphiclib& g);
  void printmap_s();
  void mazemake(int seed,const vector2& size,graphiclib& g);
};

#endif
