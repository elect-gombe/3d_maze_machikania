#include "vector3.hpp"
#include <math.h>

class Matrix4{
public:
  int m[16];
  Matrix4(){
    for(int i=0;i<16;i++){
      m[i]=0;
    }
    for(int i=0;i<4;i++){
      m[i*4+i]=65536;
    }
  }
  Matrix4(int m00,int m01,int m02,int m03,
	  int m10,int m11,int m12,int m13,
	  int m20,int m21,int m22,int m23,
	  int m30,int m31,int m32,int m33){
    m[0]=m00,m[1]=m01,m[2]=m02,m[3]=m03;
    m[4]=m10,m[5]=m11,m[6]=m12,m[7]=m13;
    m[8]=m20,m[9]=m21,m[10]=m22,m[11]=m23;
    m[12]=m30,m[13]=m31,m[14]=m32,m[15]=m33;

  }

  int operator[](int n){
    return m[n];
  }
  
  
  Matrix4& rotate_x(uint16_t angle){
    /*とりあえず。�?��??*/
    int sint,cost;
    sint = sin(((double)angle)*2*3.14159265359/65536)*65536;
    cost = cos(((double)angle)*2*3.14159265359/65536)*65536;

    int   m1 = m[1],  m2 = m[2],
      m5 = m[5],  m6 = m[6],
      m9 = m[9],  m10= m[10],
      m13= m[13], m14= m[14];
    
    m[1] = m1 * cost/65536 + m2 *-sint/65536;
    m[2] = m1 * sint/65536 + m2 * cost/65536;
    m[5] = m5 * cost/65536 + m6 *-sint/65536;
    m[6] = m5 * sint/65536 + m6 * cost/65536;
    m[9] = m9 * cost/65536 + m10*-sint/65536;
    m[10]= m9 * sint/65536 + m10* cost/65536;
    m[13]= m13* cost/65536 + m14*-sint/65536;
    m[14]= m13* sint/65536 + m14* cost/65536;

    return *this;
  }

  Matrix4& rotate_y(uint16_t angle) {
    int sint,cost;
    sint = sin(((double)angle)*2*3.14159265359/65536)*65536;
    cost = cos(((double)angle)*2*3.14159265359/65536)*65536;

    int   m0 = m[0],  m2 = m[2],
      m4 = m[4],  m6 = m[6],
      m8 = m[8],  m10= m[10],
      m12= m[12], m14= m[14];

    m[0] = m0 * cost/65536 + m2 * sint/65536;
    m[2] = m0 *-sint/65536 + m2 * cost/65536;
    m[4] = m4 * cost/65536 + m6 * sint/65536;
    m[6] = m4 *-sint/65536 + m6 * cost/65536;
    m[8] = m8 * cost/65536 + m10* sint/65536;
    m[10]= m8 *-sint/65536 + m10* cost/65536;
    m[12]= m12* cost/65536 + m14* sint/65536;
    m[14]= m12*-sint/65536 + m14* cost/65536;

    return *this;
  }

  Matrix4& rotate_z(uint16_t angle){
    int sint,cost;
    sint = sin(((double)angle)*2*3.14159265359/65536)*65536;
    cost = cos(((double)angle)*2*3.14159265359/65536)*65536;
    int   m0 = m[0],  m1 = m[1],
      m4 = m[4],  m5 = m[5],
      m8 = m[8],  m9 = m[9],
      m12= m[12], m13= m[13];
  
    m[0] = m0 * cost/65536 + m1 *-sint/65536;
    m[1] = m0 * sint/65536 + m1 * cost/65536;
    m[4] = m4 * cost/65536 + m5 *-sint/65536;
    m[5] = m4 * sint/65536 + m5 * cost/65536;
    m[8] = m8 * cost/65536 + m9 *-sint/65536;
    m[9] = m8 * sint/65536 + m9 * cost/65536;
    m[12]= m12* cost/65536 + m13*-sint/65536;
    m[13]= m12* sint/65536 + m13* cost/65536;

    return *this;
  }
  
  Matrix4& scale(float s){
    return scale(s, s, s);
  }

  Matrix4& scale(float x, float y, float z){
    m[0] = m[0]*x/65536;   m[4] = m[4]*x/65536;   m[8]  = m[8]  *x/65536;  m[12] = m[12] * x/65536;
    m[1] = m[1]*y/65536;   m[5] = m[5]*y/65536;   m[9]  = m[9]  *y/65536;  m[13] = m[13] * y/65536;
    m[2] = m[2]*z/65536;   m[6] = m[6]*z/65536;   m[10] = m[10] *z/65536;  m[14] = m[14] * z/65536;
    return *this;
  }

  vector3 applyit(vector3 v){
    int w;
    
    vector3 av;
    av.x = int(int64_t(v.x)*m[0]/65536)+int(int64_t(m[4])*v.y/65536)+int(int64_t(v.z)*m[8]/65536)+m[12];
    av.y = int(int64_t(v.x)*m[1]/65536)+int(int64_t(m[5])*v.y/65536)+int(int64_t(v.z)*m[9]/65536)+m[13];
    av.z = int(int64_t(v.x)*m[2]/65536)+int(int64_t(m[6])*v.y/65536)+int(int64_t(v.z)*m[10]/65536)+m[14];
    w  = int(int64_t(v.x)*m[3]/65536)+int(int64_t(m[7])*v.y/65536)+int(int64_t(v.z)*m[11]/65536)+m[15];
    if(w==0)w=1;
    // printf("wwwww%d\n",w);

    int rw;
    
    rw = 4294967296 / w;
    av.x=(int64_t)av.x*rw/65536;
    av.y=(int64_t)av.y*rw/65536;//65536;///w;
    av.z=int64_t(av.z)*rw/65536;//65536;//w;

    return av;
  }
  
};

Matrix4 rotatex(uint16_t ang);
Matrix4 rotatey(uint16_t ang);
Matrix4 rotatez(uint16_t ang);
Matrix4 translation(vector3 v);

void print(Matrix4 );
static inline
Matrix4 operator*(const Matrix4& m,const Matrix4& n) {
  Matrix4 r;

  r= Matrix4(int64_t(m.m[0])*n.m[0]  / 65536 + int64_t(m.m[4])*n.m[1]  / 65536 + int64_t(m.m[8])*n.m[2]   / 65536 + int64_t(m.m[12])*n.m[3]/ 65536,
	     int64_t(m.m[1])*n.m[0]  / 65536 + int64_t(m.m[5])*n.m[1]  / 65536 + int64_t(m.m[9])*n.m[2]   / 65536 + int64_t(m.m[13])*n.m[3]/ 65536,
	     int64_t(m.m[2])*n.m[0]  / 65536 + int64_t(m.m[6])*n.m[1]  / 65536 + int64_t(m.m[10])*n.m[2]  / 65536 + int64_t(m.m[14])*n.m[3]/ 65536,
	     int64_t(m.m[3])*n.m[0]  / 65536 + int64_t(m.m[7])*n.m[1]  / 65536 + int64_t(m.m[11])*n.m[2]  / 65536 + int64_t(m.m[15])*n.m[3]/ 65536,
	     
	     int64_t(m.m[0])*n.m[4]  / 65536 + int64_t(m.m[4])*n.m[5]  / 65536 + int64_t(m.m[8])*n.m[6]   / 65536 + int64_t(m.m[12])*n.m[7]/ 65536,
	     int64_t(m.m[1])*n.m[4]  / 65536 + int64_t(m.m[5])*n.m[5]  / 65536 + int64_t(m.m[9])*n.m[6]   / 65536 + int64_t(m.m[13])*n.m[7]/ 65536,
	     int64_t(m.m[2])*n.m[4]  / 65536 + int64_t(m.m[6])*n.m[5]  / 65536 + int64_t(m.m[10])*n.m[6]  / 65536 + int64_t(m.m[14])*n.m[7]/ 65536,
	     int64_t(m.m[3])*n.m[4]  / 65536 + int64_t(m.m[7])*n.m[5]  / 65536 + int64_t(m.m[11])*n.m[6]  / 65536 + int64_t(m.m[15])*n.m[7]/ 65536,
		 
	     int64_t(m.m[0])*n.m[8]  / 65536 + int64_t(m.m[4])*n.m[9]  / 65536 + int64_t(m.m[8])*n.m[10]  / 65536 + int64_t(m.m[12])*n.m[11]/ 65536,
	     int64_t(m.m[1])*n.m[8]  / 65536 + int64_t(m.m[5])*n.m[9]  / 65536 + int64_t(m.m[9])*n.m[10]  / 65536 + int64_t(m.m[13])*n.m[11]/ 65536,
	     int64_t(m.m[2])*n.m[8]  / 65536 + int64_t(m.m[6])*n.m[9]  / 65536 + int64_t(m.m[10])*n.m[10] / 65536 + int64_t(m.m[14])*n.m[11]/ 65536,
	     int64_t(m.m[3])*n.m[8]  / 65536 + int64_t(m.m[7])*n.m[9]  / 65536 + int64_t(m.m[11])*n.m[10] / 65536 + int64_t(m.m[15])*n.m[11]/ 65536,

	     int64_t(m.m[0])*n.m[12] / 65536 + int64_t(m.m[4])*n.m[13] / 65536 + int64_t(m.m[8])*n.m[14]  / 65536 + int64_t(m.m[12])*n.m[15]/ 65536,
	     int64_t(m.m[1])*n.m[12] / 65536 + int64_t(m.m[5])*n.m[13] / 65536 + int64_t(m.m[9])*n.m[14]  / 65536 + int64_t(m.m[13])*n.m[15]/ 65536,
	     int64_t(m.m[2])*n.m[12] / 65536 + int64_t(m.m[6])*n.m[13] / 65536 + int64_t(m.m[10])*n.m[14] / 65536 + int64_t(m.m[14])*n.m[15]/ 65536,
	     int64_t(m.m[3])*n.m[12] / 65536 + int64_t(m.m[7])*n.m[13] / 65536 + int64_t(m.m[11])*n.m[14] / 65536 + int64_t(m.m[15])*n.m[15]/ 65536);

  return r;
}
Matrix4 loadPerspective(int fovy, int aspect,int zNear, int zFar,int width,int height);
Matrix4 magnify(int n);
Matrix4 lookat(vector3 eye,vector3 goal);
Matrix4 magnify_y(int n);
