class triangle{
public:
  // top point.
  vector3 p[3];
  color_t col;
  int ymin;
  int ymax;
  int delta[2][2];
  int zdelta[2][2];
  int pdx[2];
  int pdz[2];
  int yno;
  int phase;

  int smoke;
  // int z_val;
  // int dxdz;
  // int dydz;

  int draw(int *zlinebuf,graphiclib &g);  
  int triangle_set(vector3 px[3],color_t col);

  int cymin();
  int cymax();
};
