#include "matrix4.hpp"
#include "vector3.hpp"

Matrix4 loadPerspective(int fovy, int aspect,int zNear, int zFar,int width,int height){
  Matrix4 m;
  const int dz(zFar - zNear);
  (void)width;
  (void)height;

  if (dz != 0.0f) {
    m.m[ 5] = int(65536 / tan(fovy*3.1415926535898/65536));
    m.m[ 0] = (int64_t)m.m[5]*aspect/65536;
    m.m[10] = -int64_t(zFar + zNear) *65536 / dz;
    m.m[11] = -65536;
    m.m[14] = (int64_t)(-65536)*2 * zFar/65536 * zNear / dz;
    m.m[ 1] = m.m[ 2] = m.m[ 3] = m.m[ 4] =m.m[ 6] = m.m[ 7] = m.m[ 8] = m.m[ 9] = m.m[15] = 0;
  }
  return m;
}

Matrix4 lookat(vector3 goal,vector3 eye){
  Matrix4 m,rm;

  vector3 t,r,s;
  t=eye-goal;
  r=cross(vector3(0,-65536,0),t);
  s=cross(t,r);

  int rs,ss,ts;
  rs = r.abs();
  ss = s.abs();
  ts = t.abs();

  m.m[0] = int64_t(r.x)*65536/rs;
  m.m[4] = int64_t(r.y)*65536/rs;
  m.m[8] = int64_t(r.z)*65536/rs;

  m.m[1] = int64_t(s.x)*65536/ss;
  m.m[5] = int64_t(s.y)*65536/ss;
  m.m[9] = int64_t(s.z)*65536/ss;

  m.m[2] = int64_t(t.x)*65536/ts;
  m.m[6] = int64_t(t.y)*65536/ts;
  m.m[10] = int64_t(t.z)*65536/ts;
  // print(m);

  return m*translation(-eye*1);
}

// void print(Matrix4 m){
//   for(int i=0;i<16;i++){
//     std::cout <<std::fixed<< std::setprecision(5)<<m.m[i]/65536. << ",";
//     if(i%4==3)std::cout<<std::endl;
//   }
//   std::cout<<std::endl;
// }

Matrix4 rotatex(uint16_t ang){
  Matrix4 r;
  int sint,cost;
  sint = int(sinf(((float)ang)*2*3.14159265359/65536)*65536);
  cost = int(cosf(((float)ang)*2*3.14159265359/65536)*65536);

  r.m[0 ] = 65536, r.m[4 ] = 0    , r.m[8 ] = 0     , r.m[12] = 0;
  r.m[1 ] = 0, r.m[5 ] =  cost, r.m[9 ] = -sint , r.m[13] = 0;
  r.m[2 ] = 0, r.m[6 ] =  sint, r.m[10] = cost  , r.m[14] = 0;
  r.m[3 ] = 0, r.m[7 ] = 0    , r.m[11] = 0     , r.m[15] = 65536;

  return r;
}

Matrix4 rotatey(uint16_t ang){
  Matrix4 r;
  int sint,cost;
  sint = int(sinf(((float)ang)*2*3.14159265359/65536)*65536);
  cost = int(cosf(((float)ang)*2*3.14159265359/65536)*65536);

  r.m[0 ] =  cost, r.m[4 ] = 0    , r.m[8 ] = sint , r.m[12] = 0;
  r.m[1 ] =  0   , r.m[5 ] = 65536, r.m[9 ] = 0    , r.m[13] = 0;
  r.m[2 ] = -sint, r.m[6 ] = 0    , r.m[10] = cost , r.m[14] = 0;
  r.m[3 ] =  0   , r.m[7 ] = 0    , r.m[11] = 0    , r.m[15] = 65536;

  return r;
}

Matrix4 rotatez(uint16_t ang){
  Matrix4 r;
  int sint,cost;
  sint = int(sin(((double)ang)*2*3.14159265359/65536.)*65536);
  cost = int(cos(((double)ang)*2*3.14159265359/65536.)*65536);

  r.m[0 ] = cost , r.m[4 ] = -sint , r.m[8 ] = 0    , r.m[12] = 0;
  r.m[1 ] = sint , r.m[5 ] = cost  , r.m[9 ] = 0    , r.m[13] = 0;
  r.m[2 ] = 0    , r.m[6 ] = 0     , r.m[10] = 65536, r.m[14] = 0;
  r.m[3 ] = 0    , r.m[7 ] = 0     , r.m[11] = 0    , r.m[15] = 65536;

  return r;
}

Matrix4 translation(vector3 v){
  return Matrix4(65536,0,0,0,
		 0,65536,0,0,
		 0,0,65536,0,
		 v.x,v.y,v.z,65536);
}

Matrix4 magnify(int n){
  return Matrix4(65536*n,0,0,0,
		 0,65536*n,0,0,
		 0,0,65536*n,0,
		 0,0,0,65536);
}

Matrix4 magnify_y(int n){
  return Matrix4(65536,0,0,0,
		 0,n,0,0,
		 0,0,65536,0,
		 0,0,0,65536);
}

