#include "graphiclib.hpp"
#include "triangle.hpp"

int triangle::draw(int *zlinebuf,graphiclib &g){
  int sx,ex;
  int sz,ez;
  int zv;
  int deltaz;
  //    std::cout <<"x[0]:"<<pdx[0]/65536<<",x[1]:"<<pdx[1]/65536<<std::endl;

  g.setbrush(col);

  if(pdx[0] >= pdx[1]){
    sx = (pdx[1])/65536;
    ex = (pdx[0]+32768)/65536;
    sz = pdz[1];
    ez = pdz[0];
  }else{
    // volatile int a = 0/0;
    sx = (pdx[0])/65536;
    ex = (pdx[1]+32768)/65536;
    sz = pdz[0];
    ez = pdz[1];
  }
  // printf("%.2f,%.2f",sz/65536.,ez/65536.);

  zv = sz;
  /*sx,ez must be 0-65535.*/
  if(sx!=ex){
    deltaz = (ez-sz)/(ex-sx);
  }else{
    deltaz = 0;
  }

  if(sx >= window_width)goto distnext;
  if(sx < 0){
    zv -= sx * deltaz;
    sx=0;
  }
  if(ex < 0)goto distnext;
  if(ex >= window_width){
    ex = window_width-1;
  }

  //      volatile int x = 0/0;

  {
    int c=sx+yno+ymin+frame;
    unsigned short g=0;
    unsigned short *gr;
    int i;
    gr = VRAM+(yno+ymin)*64+sx/4;
    for(i=sx;i<ex;i++){
      zv += deltaz;
      smoke = min(16777216-zv,16777216/4);
      if(zv < zlinebuf[i]){
	//g.setbrush();
	fpset(i,yno+ymin,(col*smoke/65536/64+(c&1))/2);
	zlinebuf[i] = zv;
      }
      c++;
    }
    
    for(;i<ex;i++){
      ;//plot
      //      printf("%6.1f ",zv/65536.);
      zv += deltaz;
      g=0;
      smoke = min(16777216-zv,16777216/4);
      if(zv < zlinebuf[i]){
	//g.setbrush();
	g=((col*smoke/65536/64+(c&1))/2)<<12;
	zlinebuf[i] = zv;
      }
      c++;
      i++;
      if(i>=ex)break;
      zv += deltaz;
      if(zv < zlinebuf[i]){
	//g.setbrush();
	g|=((col*smoke/65536/64+(c&1))/2)<<8;
	fpover(i,yno+ymin,(col*smoke/65536/64+(c&1))/2);
	zlinebuf[i] = zv;
      }
      c++;
      i++;
      if(i>=ex)break;
      zv += deltaz;
      if(zv < zlinebuf[i]){
	//g.setbrush();
	g|=((col*smoke/65536/64+(c&1))/2)<<4;
	zlinebuf[i] = zv;
      }
      c++;
      i++;
      if(i>=ex)break;
      zv += deltaz;
      if(zv < zlinebuf[i]){
	//g.setbrush();
	g|=(col*smoke/65536/64+(c&1))/2;
	zlinebuf[i] = zv;
      }
      c++;
      *gr++|=g;
    }
    //    puts("");
  }
 distnext:
  if(yno+ymin >= p[1].y){
    phase=1;
  }
  yno++;

  //    zv -= (int64_t)dxdz * (ex-sx+1+(delta[phase][0]+(pdx[0]&0xFFFF))/65536);
  // printf("prev%1.8f ",zv/256./65536.);
  //    zv += dydz;
  //    z_val = zv;

  for(int i=0;i<2;i++){
    pdx[i]+=delta[phase][i];
    pdz[i]+=zdelta[phase][i];
  }

  if(yno+ymin > ymax)return 1;
  //to be continued.
  return 0;
}
  
int triangle::triangle_set(vector3 px[3],color_t col){
  int delta_top_mid;
  int delta_top_btm;
  int delta_mid_btm;
  int top_mid_x;
  int top_btm_x;
  int top_mid_z;
  int top_btm_z;
  // int delta_zx;
  // int delta_zy;
  vector3 t;
  vector3 cr;

  yno=0;
  phase = 0;
  this->col = col;
    
    
  for(int i=0;i<3;i++){
    p[i]=px[i];
  }
  if(p[0].y > p[1].y) {
    t=p[0];
    p[0]=p[1];
    p[1]=t;
  }
  if(p[0].y > p[2].y){
    t=p[0];
    p[0]=p[2];
    p[2]=t;
  }
  if(p[1].y > p[2].y){
    t=p[1];
    p[1]=p[2];
    p[2]=t;
  }

  // std::cout << "("<<p[0].x<<","<<p[0].y<<","<<p[0].z<<")"<<std::endl;
  // std::cout << "("<<p[1].x<<","<<p[1].y<<","<<p[1].z<<")"<<std::endl;
  // std::cout << "("<<p[2].x<<","<<p[2].y<<","<<p[2].z<<")"<<std::endl;

  // {
  //   vector3 a = p[2]-p[0];
  //   vector3 b = p[1]-p[0];
      
  //   printf("a=(%d,%d,%d)\n",a.x,a.y,a.z);
  //   printf("b=(%d,%d,%d)\n",b.x,b.y,b.z);

  //   a.x*=65536; b.x*=65536;
  //   a.y*=65536; b.y*=65536;
  // {
  // 	int aabs;
  // 	aabs = (unsigned int)0xFFFFFFFF/a.abs();
  // 	// printf("aabs.....................%f",aabs/65536.);
  // 	a.x = (int64_t)a.x * aabs/65536;
  // 	a.y = (int64_t)a.y * aabs/65536;
  // 	a.z = (int64_t)a.z * aabs/65536;
  // 	printf("a(%.2f,%.2f,%.2f)\n",a.x/65536.,a.y/65536.,a.z/65536.);
  // }
  // {
  // 	int babs;
  // 	babs = (unsigned int)0xFFFFFFFF/a.abs();
  // 	b.x = (int64_t)b.x * babs/65536;
  // 	b.y = (	int64_t)b.y * babs/65536;
  // 	b.z = (int64_t)b.z * babs/65536;
  // 	printf("b(%.2f,%.2f,%.2f)\n",b.x/65536.,b.y/65536.,b.z/65536.);
  // }

  // cr = cross(b,a);
  //	printf("cr.z is zero(%d,%d,%d)\n",cr.x,cr.y,cr.z);
 
  //   if(cr.z&&cr.y!=0){
  // 	{
  // 	  int crabs;
  // 	  crabs = cr.abs();
  // 	  delta_zx = int64_t(1<<8)*cr.z/cr.x;
  // 	  delta_zy = int64_t(1<<8)*cr.z/cr.y;
  // 	}
  // 	dxdz = delta_zx;
  // 	dydz = delta_zy;
  // 	printf("cr=(%.5f,%.5f,%.5f)\n",cr.x/65536.,cr.y/65536.,cr.z/65536.);
  // 	printf("(dz/dx,dz/dy)=(%.8f,%.8f)\n",delta_zx/256.,delta_zy/256.);
  //   }else{
  // 	printf("cr.z is zero(%d,%d,%d)\n",cr.x,cr.y,cr.z);
  //   }
  // }

  this->col = col;
  ymin = cymin();
  ymax = cymax();
  if(ymax>=window_height)ymax=window_height-1;

  top_btm_x = p[0].x;
  top_mid_x = p[0].x;
  top_btm_z = p[0].z;
  top_mid_z = p[0].z;

  if(p[0].y==p[1].y){
    phase=1;
    top_mid_x = p[1].x;
    top_mid_z = p[1].z;
  }


  if(ymin>=window_height-1) return -1; 
  if(ymax <= 0) return -1;
  if (ymin < 0) {
    ymin = 0;
    if (p[1].y >= 0) {
      if(p[0].y != p[1].y){
	top_mid_x = (p[1].x - p[0].x) * int64_t(p[1].y) / (p[0].y - p[1].y) + p[1].x;
	top_mid_z = (p[1].z - p[0].z) * int64_t(p[1].y) / (p[0].y - p[1].y) + p[1].z;
      }
    }else{
      if(p[1].y!=p[2].y){
	top_mid_x = (p[2].x - p[1].x) * int64_t(p[2].y) / (p[1].y - p[2].y) + p[2].x;
	top_mid_z = (p[2].z - p[1].z) * int64_t(p[2].y) / (p[1].y - p[2].y) + p[2].z;
      }
    }
    if(p[0].y != p[2].y){
      top_btm_x = (p[2].x - p[0].x ) * int64_t(p[2].y) / (p[0].y - p[2].y ) + p[2].x;
      top_btm_z = (p[2].z - p[0].z ) * int64_t(p[2].y) / (p[0].y - p[2].y ) + p[2].z;
    }
  }
  if(ymin < 0)ymin=0;
  //z_val = p[2].z*256;
  
  if(p[2].y!=p[1].y){
    delta_top_mid = int64_t(65536)*(p[1].x-p[2].x)/(p[1].y-p[2].y);
  }else{
    delta_top_mid = 0;
  }
  
  if(p[1].y!=p[0].y){
    delta_mid_btm = int64_t(65536)*(p[1].x-p[0].x)/(p[1].y-p[0].y);
  }else{
    delta_mid_btm = 0;
  }

  if(p[2].y!=p[0].y){
    delta_top_btm = int64_t(65536)*(p[2].x-p[0].x)/(p[2].y-p[0].y);
  }else{
    delta_top_btm = 0;
  }

  int zdelta_top_mid;
  int zdelta_top_btm;
  int zdelta_mid_btm;
  if(p[2].y!=p[1].y){
    zdelta_top_mid = int64_t(256)*(p[1].z-p[2].z)/(p[1].y-p[2].y);
  }else{
    zdelta_top_mid = 0;
  }

  if(p[1].y!=p[0].y){
    zdelta_mid_btm = int64_t(256)*(p[1].z-p[0].z)/(p[1].y-p[0].y);
  }else{
    zdelta_mid_btm = 0;
  }

  if(p[2].y!=p[0].y){
    zdelta_top_btm = int64_t(256)*(p[2].z-p[0].z)/(p[2].y-p[0].y);
  }else{
    zdelta_top_btm = 0;
  }
  // printf("zdelta %f %f %f\n",zdelta_top_btm/65536.,zdelta_top_mid/65536.,zdelta_mid_btm/65536.);
    
  int split_x;
  if(p[0].y != p[2].y){
    split_x = ((int64_t(p[0].x-p[2].x)*(p[1].y-p[0].y)*65536)/(p[0].y-p[2].y)+p[0].x*65536);
    // printf("\n\n");
  }else{
    split_x = p[2].x*65536;
  }

  // printf("sprit%.2f",split_x/65536.);
    
  if(p[1].x*65536 >= split_x){
    pdx[0] = top_mid_x*65536;
    pdx[1] = top_btm_x*65536;
    pdz[0] = top_mid_z*256;
    pdz[1] = top_btm_z*256;
    delta[1][0]=delta_top_mid;
    delta[1][1]=delta_top_btm;
    delta[0][0]=delta_mid_btm;
    delta[0][1]=delta_top_btm;
    zdelta[1][0]=zdelta_top_mid;
    zdelta[1][1]=zdelta_top_btm;
    zdelta[0][0]=zdelta_mid_btm;
    zdelta[0][1]=zdelta_top_btm;
      
  }else{
    pdx[0] = top_btm_x*65536;
    pdx[1] = top_mid_x*65536;
    pdz[0] = top_btm_z*256;
    pdz[1] = top_mid_z*256;
    delta[1][0]=delta_top_btm;
    delta[1][1]=delta_top_mid;
    delta[0][0]=delta_top_btm;
    delta[0][1]=delta_mid_btm;
    zdelta[1][0]=zdelta_top_btm;
    zdelta[1][1]=zdelta_top_mid;
    zdelta[0][0]=zdelta_top_btm;
    zdelta[0][1]=zdelta_mid_btm;
  }
  // printf("start @%f @%f\n",pdx[0]/65536.,pdx[1]/65536.);
  // printf("delta %f %f %f %f\n",delta[0][0]/65536.,delta[0][1]/65536.,delta[1][0]/65536.,delta[1][1]/6
  return 0;
}

int triangle::cymin(){
  return p[0].y;
}

int triangle::cymax(){
  return p[2].y;
}
