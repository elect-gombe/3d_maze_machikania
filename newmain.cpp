#define _SUPPRESS_PLIB_WARNING
// TETRIS（キャラクターベース） Rev.1.0 2013/8/22
// 　with テキストVRAMコンポジット出力システム for PIC32MX1xx/2xx by K.Tanaka
// rev.2 PS/2キーボードシステムに対応
#include <plib.h>
#include <stdlib.h>
#include "videoout.h"
#include "vector2.h"
#include "xprintf.h"
#include "graphiclib.hpp"
#include "maze.hpp"
#include <stdint.h>
#include <xc.h>

int main_3d(void);
void hint(graphiclib& g,map &m,const vector3& lookat,vector2 &np);


using namespace video;

void init(){
  OSCConfig(OSC_POSC_PLL, OSC_PLL_MULT_15, OSC_PLL_POST_1, 0);
  // 周辺機能ピン割り当て
  SDI2R = 2; //RPA4:SDI2
  RPB5R = 4; //RPB5:SDO2

  //ポートの初期設定
  TRISA = 0x0010; // RA4は入力
  CNPUA = 0x0010; // RA4をプルアップ
  ANSELA = 0x0000; // 全てデジタル
  TRISB = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT; // ボタン接続ポート入力設定
  CNPUBSET = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT; // プルアップ設定
  ANSELB = 0x0000; // 全てデジタル
  LATACLR = 2; // RA1=0（ボタンモード）

  init_composite();

  for(int i=0;i<16;i++){
    set_palette(i,i*15,i*15,i*15);
  }
}

int main(void){
  init();
  main_3d();
  for(int j=0;j<224;j++){
    for(int i=0;i<256;i++){
        pset(i,j,(volatile int)7);
    }
  }
  
  return 0;
}

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "matrix4.hpp"
#include "graphiclib.hpp"
#include "triangle.hpp"

#define POLYMAXNUM 300
#define POINTMAXNUM 600

vector3_t pointvec[POINTMAXNUM]={
};

int polyvec[POLYMAXNUM][3]={
};

static
int loadPower(const vector3 &light_pos,const vector3 &light_n,const vector3 obj[3]){
  vector3 light_obj;
  vector3 n;
  vector3 obj_pos;
  int cos_cross;

  obj_pos = obj[0]+obj[1]+obj[2];
  obj_pos.x = obj_pos.x / 3;
  obj_pos.y = obj_pos.y / 3;
  obj_pos.z = obj_pos.z / 3;
  
  n = calc_nv(obj);

  light_obj = light_pos-obj_pos;
  
  cos_cross = light_n * n;

  return (int64_t)cos_cross*5;// * light_obj_dist/65536;
}

map gmap;

// *draw poly* num max
#define ONSCREENNUM 50

int main_3d(void){
  graphiclib gc;
  Matrix4 m;
  Matrix4 projection;
  Matrix4 obj;

  vector3_t poly_transed[POINTMAXNUM];
  
  int zlinebuf[window_width];
  triangle t[ONSCREENNUM];

  int prevcount=0;
    
  //Matrix4 loadPerspective(int fovy, int aspect,int zNear, int zFar){
  // graphiclib gc(window_width,window_height);
  
  gc.screenclear();
  gc.setbrush(255);
  // {
  //   vector3 pv[3]={vector3(30,10,0),vector3(80,40,65536),vector3(60,110,0)};
  //   triangle t(pv,0);
  //   while(!t.draw(zlinebuf,gc)){
  //     for(int i=0;i<160;i++){
  // 	zlinebuf[i]=0x7FFFFFFF;
  //     }
  //     gc.flush();
  //     gc.delay(1);
  //   }
  // }
  // for(int i=0;i<300;i++){
  //   gc.flush();
  //   gc.delay(10);
  // }
  int polynum;
  int pointnum;

  gmap.mazemake(0,vector2(59,59),gc);

  clearscreen();

  projection=translation(vector3(32768,32768,0));
  projection=loadPerspective(15000,65536*window_height/window_width,65536/2,65536*30,0,0)*projection;

  vector3 viewdir;
  vector3 vlookat;
  vector2 mouse;
  vector2 np;
  
  vlookat = vector3(0,35000,0);
  while(1){
    pointnum=gmap.genepoints(pointvec,vlookat);
    polynum=gmap.genepoly(polyvec,vlookat);
    viewdir = vector3(cos(np.x/1000.)*cos(np.y/1000.)*65536,sin(np.y/1000.)*65536,sin(np.x/1000.)*cos(np.y/1000.)*65536);
    m=projection*lookat(vlookat-viewdir,vlookat+viewdir);

    //print(m);

    //    obj=obj*translation(vector3(0,0,65536*3));
    //    obj=rotatey(frame*5);
 // m=m*rotatez(5461);

    //  m=m*magnify(2);
    // print(m);
    
    vector3 vo[3];
    vector3 v[3];
    vector3 n;
    int tnum=0;

    for(int j=0;j<pointnum;j++){
      if(abs(vlookat.x-pointvec[j].x)+abs(vlookat.z-pointvec[j].z)<65536*15){
	poly_transed[j] = m.applyit(vector3(pointvec[j]));
      }else{
	poly_transed[j].z = -1;
      }
      //      std::cout<<"poly"<<pointvec[j].x/65536.<<","<<pointvec[j].y/65536.<<","<<pointvec[j].z/65536.<<std::endl;
    }
    for(int i=0;i<polynum;i++){
      for(int j=0;j<3;j++){
 	v[j] = poly_transed[polyvec[i][j]];
 	vo[j] = pointvec[polyvec[i][j]];
      }

      if(v[0].z &0x00FF0000)continue;
      if(v[1].z &0x00FF0000)continue;
      if(v[2].z &0x00FF0000)continue;

      //      if(v[0].z < 0) break;
      if(!(
	   !((v[0].x|v[0].y|v[0].z)&0xFFFF0000)||
	   !((v[1].x|v[1].y|v[1].z)&0xFFFF0000)||
	   !((v[2].x|v[2].y|v[2].z)&0xFFFF0000)
	   ))continue;

      v[0].x=v[0].x*window_width/65536;v[0].y=v[0].y*window_height/65536;
      v[1].x=v[1].x*window_width/65536;v[1].y=v[1].y*window_height/65536;
      v[2].x=v[2].x*window_width/65536;v[2].y=v[2].y*window_height/65536;

      // std::cout<<"v0 "<<v[0].x<<","<<v[0].y<<","<<v[0].z<<std::endl;
      // std::cout<<"v1 "<<v[1].x<<","<<v[1].y<<","<<v[1].z<<std::endl;
      // std::cout<<"v2 "<<v[2].x<<","<<v[2].y<<","<<v[2].z<<std::endl;
      //      std::cout<<v1.x
      //gc.setbrush();

      int light = abs(min(loadPower(vector3(),-viewdir,vo)/14000,25))+5;
      if(light+1){
 	if(t[tnum++].triangle_set(v,light)==-1)tnum--;
      }
      //int loadPower(const vector3 &light_pos,const vector3 &light_n,const vector3 obj[3]){
    }

    int draworder[ONSCREENNUM];
    int zdata[ONSCREENNUM];

    for(int i=0;i<tnum;i++){
      zdata[i] = t[i].p[1].z+t[i].p[2].z+t[i].p[0].z;
      draworder[i] = i;
    }

    int tmpdata;
    int tmpsubdata;
    int j;

    for (int i = 1; i < tnum; i++) {
      tmpdata = zdata[i];
      tmpsubdata = draworder[i];
      if (zdata[i-1] > zdata[i]) {
        j = i;
        do {
	  zdata[j] = zdata[j-1];
	  draworder[j] = draworder[j-1];
	  j--;
        } while (j > 0 && zdata[j - 1] > tmpdata);
        zdata[j] = tmpdata;
	draworder[j] = tmpsubdata;
      }
    }
    
    for(int y=0;y<window_height;y++){
      for(int i=0;i<window_width;i++){
	zlinebuf[i]=65536*250;
      }
      for(int i=0;i<tnum;i++){
	if(t[draworder[i]].ymin < y&&t[draworder[i]].ymax >= y){
	  t[draworder[i]].draw(zlinebuf,gc);
	}
      }
      for(int i=0;i<window_width;i++){
	if(zlinebuf[i]==65536*250){
	  fpover(i,y);
	}
      }
    }

    vector3 vp;
    np.x -= 300;
    np.y -= 200;
    vp.x = -cos(np.x/1000.)*cos(np.y/1000.)*10000;
    vp.y = 0;
    vp.z = -sin(np.x/1000.)*cos(np.y/1000.)*10000;
    np.x += 300;
    np.y += 200;
    if(!(PORTB&(1<<10)))
      vlookat = vlookat + vp;
    if(!(PORTB&(1<<7)))
      vlookat = vlookat - vp;
    if(!(PORTB&(1<<8)))
      np.x += 100;
    if(!(PORTB&(1<<9)))
      np.x -= 100;

    // for(int i=0;i<3;i++){
    frame++;
    if(frame%10==0){
      printnum2(0,200,15,0,599/(drawcount-prevcount),3);
      printstr(8*3,200,15,0,(unsigned char*)"fps");
      printnum2(60,200,15,0,tnum,3);
      printstr(60+8*3,200,15,0,(unsigned char*)"poly");
      printnum2(120,200,15,0,(VRAMp-VRAM)/64,3);
      printstr(120+8*3,200,15,0,(unsigned char*)"line");
      prevcount = drawcount;

      printstr(0,210,15,0,(unsigned char*)"mapinfo");
      printnum2(60,210,15,0,pointnum,3);
      printstr(60+8*3,210,15,0,(unsigned char*)"point");
      printnum2(150,210,15,0,polynum,3);
      printstr(150+8*3,210,15,0,(unsigned char*)"poly");
    }
    
    if(ISPRESSED(KEYFIRE)){
      hint(gc,gmap,vlookat,np);
    }
  }
  return 0;
}


void hint(graphiclib& g,map &m,const vector3& lookat,vector2 &np){
  vector2 at;
  vector2 atg;
  int i=0;

  at = m.calcpos(lookat,3);
  atg.x = at.x/3;
  atg.y = at.y/3;
  at += vector2(10,10);
  
  boxfill(10,10,m.getsize().x*3+10,m.getsize().y*3+10,3);
  m.printmap(g,atg,8);
  g.setfill(4);
  g.fillcircle(at,2);

  vector2 dir;
  np.x -= 300;
  np.y -= 200;
  dir.y = -cos(np.x/1000.)*cos(np.y/1000.)*10;
  dir.x = -sin(np.x/1000.)*cos(np.y/1000.)*10;
  np.x += 300;
  np.y += 200;

  // vector2 dir;
  // dir.y = -viewdir.x * 10 / 65536;
  // dir.x = -viewdir.z * 10 / 65536;
  // dir.print();

  //  g.Line(at,at+dir);
  
  wait60thsec(1);
  while(ISPRESSED(KEYFIRE)){
    asm("wait");
  }
  wait60thsec(1);
  while(!ISPRESSED(KEYFIRE)){
    while(drawing);
    g.setbrush(i++&0xF);
    g.fillcircle(at,2);
    while(!drawing);
  }
  wait60thsec(1);
  while(ISPRESSED(KEYFIRE)){
    asm("wait");
  }
  clearscreen();
}
