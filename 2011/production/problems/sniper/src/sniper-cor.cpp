#include "..\correct.h"

using namespace std;

const int MAXN = 100;
const int MAXC = 100;

InputStream in("sniper.in");

int ReadCoord(){
  int x = in.ReadInt();
  in.Assert(-MAXC<=x && x<=MAXC);
  return x;
}

int coords[MAXN][4];

int snum;
int segs[5*MAXN][4];

inline void add_seg(int x1, int y1, int x2, int y2){
  int t;
  if ((x1 > x2) || ((x1 == x2) && (y1 > y2))){
    t = x1;
    x1 = x2;
    x2 = t;
    t = y1;
    y1 = y2;
    y2 = t;
  }
  segs[snum][0] = x1;
  segs[snum][1] = y1;
  segs[snum][2] = x2;
  segs[snum][3] = y2;
  snum++;
}

inline int sgn(int x){
  return (x == 0) ? 0 : (x > 0) ? 1 : -1;
}

inline int vpv(int x1, int y1, int x2, int y2){
  return x1*y2 - x2*y1;
}

inline int spv(int x1, int y1, int x2, int y2){
  return x1*x2 + y1*y2;
}

int intersect(int a, int b){
  int s1 = sgn(vpv(segs[a][2]-segs[a][0], segs[a][3]-segs[a][1], segs[b][0]-segs[a][0], segs[b][1]-segs[a][1]));
  s1 *= sgn(vpv(segs[a][2]-segs[a][0], segs[a][3]-segs[a][1], segs[b][2]-segs[a][0], segs[b][3]-segs[a][1]));
  int s2 = sgn(vpv(segs[b][2]-segs[b][0], segs[b][3]-segs[b][1], segs[a][0]-segs[b][0], segs[a][1]-segs[b][1]));
  s2 *= sgn(vpv(segs[b][2]-segs[b][0], segs[b][3]-segs[b][1], segs[a][2]-segs[b][0], segs[a][3]-segs[b][1]));
  if ((s1 > 0) || (s2 > 0)) return 0;
  if (vpv(segs[a][2]-segs[a][0], segs[a][3]-segs[a][1], segs[b][2]-segs[b][0], segs[b][3]-segs[b][1]) != 0) return 1;
  if (spv(segs[a][0]-segs[b][0], segs[a][1]-segs[b][1], segs[a][2]-segs[b][0], segs[a][3]-segs[b][1]) <= 0) return 1;
  if (spv(segs[b][0]-segs[a][0], segs[b][1]-segs[a][1], segs[b][2]-segs[a][0], segs[b][3]-segs[a][1]) <= 0) return 1;
  return 0;
}

void out_seg(int i){
  for (int j=0; j<4; j++) printf("%d ", segs[i][j]);
  printf("\n");
}

int main(){

  int sx, sy, sz;
  sx = ReadCoord();
  in.RequireSpace();
  sy = ReadCoord();
  in.RequireSpace();
  sz = ReadCoord();
  in.Assert(sz >= 0);
  in.RequireEoln(false);
  
  int ax, ay, bx, by;
  ax = ReadCoord();
  in.RequireSpace();
  ay = ReadCoord();
  in.RequireSpace();
  bx = ReadCoord();
  in.RequireSpace();
  by = ReadCoord();
  in.RequireEoln(false);

  int n = in.ReadInt();
  in.Assert(0<=n && n<=MAXN);
  in.RequireEoln(false);

  int i, j;
  for (i=0; i<n; i++){
    for (j=0; j<4; j++){
      coords[i][j] = ReadCoord();
      in.RequireSpace();
    }
    in.Assert(coords[i][0] < coords[i][2]);
    in.Assert(coords[i][1] < coords[i][3]);
    int hs = in.ReadInt();
    in.Assert(hs > 0);
    in.Assert(!(coords[i][0]<=sx && sx<=coords[i][2] && coords[i][1]<=sy && sy<=coords[i][3] && sz<=hs));
    add_seg(coords[i][0], coords[i][1], coords[i][0], coords[i][3]);
    add_seg(coords[i][2], coords[i][1], coords[i][2], coords[i][3]);
    add_seg(coords[i][0], coords[i][1], coords[i][2], coords[i][1]);
    add_seg(coords[i][0], coords[i][3], coords[i][2], coords[i][3]);
    in.RequireEoln(false);
  }

  add_seg(ax, ay, bx, by);

  for (i=0; i<snum; i++){
    for (j=i+1; j<snum; j++){
      if (i/4 == j/4) continue;
      if (intersect(i, j)){
        out_seg(i);
        out_seg(j);
      }
      in.Assert(!intersect(i, j));
    }
  }

  in.RequireEof(false);

  return 0;
}
