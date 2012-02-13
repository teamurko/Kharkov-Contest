#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

const double EPS = 1e-9;

inline int sgn(double a){
  return (fabs(a) < EPS) ? 0 : (a > 0) ? 1 : -1;
}

struct point{
  int x, y, z;
  point(){
    x = y = z = 0;
  }
  point(int xx, int yy, int zz){
    x = xx;
    y = yy;
    z = zz;
  }
  point operator - (const point& other) const{
    return point(x-other.x, y-other.y, z-other.z);
  }
  int operator * (const point& other) const{
    return x*other.x + y*other.y + z*other.z;
  }
  point operator % (const point& other) const{
    return point(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
  }
};

const int MAXN = 101;

point S, A, B;
int n;
int xs[MAXN][2], ys[MAXN][2], hs[MAXN];
vector<double> ts;

void add_plane(point u, point v, point w){
  point N = (v-u)%(w-u);
  if ((N.x == 0) && (N.y == 0) && (N.z == 0)) return;
  int D = N * u;
  int va = N * A - D;
  int vb = N * B - D;
  if (sgn(va) * sgn(vb) < 0){
    ts.push_back(fabs(va) / fabs(vb - va));
  }
}

void unite(double& l1, double& r1, double l2, double r2){
  if (l2 > l1) l1 = l2;
  if (r2 < r1) r1 = r2;
}

bool check(double zx, double zy){
  int i;
  for (i=0; i<n; i++){
    double cl=0, cr=1;
    if (S.z > hs[i]){
      double ct = double(S.z - hs[i]) / S.z;
      unite(cl, cr, ct, 1);
    }
    
    if (sgn(S.x-xs[i][0]) * sgn(zx-xs[i][0]) < 0){
      double ct = fabs(S.x - xs[i][0]) / fabs(S.x - zx);
      if (S.x < xs[i][0]) unite(cl, cr, ct, 1);
      else unite(cl, cr, 0, ct);
    }
    else if (S.x < xs[i][0]) unite(cl, cr, 1, 0);
    
    if (sgn(S.x-xs[i][1]) * sgn(zx-xs[i][1]) < 0){
      double ct = fabs(S.x - xs[i][1]) / fabs(S.x - zx);
      if (S.x > xs[i][1]) unite(cl, cr, ct, 1);
      else unite(cl, cr, 0, ct);
    }
    else if (S.x > xs[i][1]) unite(cl, cr, 1, 0);
    
    if (sgn(S.y-ys[i][0]) * sgn(zy-ys[i][0]) < 0){
      double ct = fabs(S.y - ys[i][0]) / fabs(S.y - zy);
      if (S.y < ys[i][0]) unite(cl, cr, ct, 1);
      else unite(cl, cr, 0, ct);
    }
    else if (S.y < ys[i][0]) unite(cl, cr, 1, 0);
    
    if (sgn(S.y-ys[i][1]) * sgn(zy-ys[i][1]) < 0){
      double ct = fabs(S.y - ys[i][1]) / fabs(S.y - zy);
      if (S.y > ys[i][1]) unite(cl, cr, ct, 1);
      else unite(cl, cr, 0, ct);
    }
    else if (S.y > ys[i][1]) unite(cl, cr, 1, 0);
    
    if (cl < cr - EPS){
      double ct = (cl + cr) / 2;
      double px = S.x + ct * (zx - S.x);
      double py = S.y + ct * (zy - S.y);
      if ((xs[i][0] + EPS < px) && (px < xs[i][1] - EPS) && (ys[i][0] + EPS < py) && (py < ys[i][1] - EPS)){
        return false;
      }
    }

  }
  return true;
}

int main(){
/*
#ifndef ONLINE_JUDGE
  freopen("sniper.in","r",stdin);
  freopen("sniper.out","w",stdout);
#endif
*/
  int i, j;
  scanf("%d%d%d", &S.x, &S.y, &S.z);
  scanf("%d%d%d%d", &A.x, &A.y, &B.x, &B.y);
  scanf("%d", &n);
  ts.push_back(0);
  ts.push_back(1);
  for (i=0; i<n; i++){
    scanf("%d%d%d%d%d", &xs[i][0], &ys[i][0], &xs[i][1], &ys[i][1], &hs[i]);
    for (j=0; j<4; j++){
      add_plane(S, point(xs[i][j/2], ys[i][j%2], 0), point(xs[i][j/2], ys[i][j%2], hs[i]));
      add_plane(S, point(xs[i][j/2], ys[i][j%2], hs[i]), point(xs[i][j%2], ys[i][1-j/2], hs[i]));
    }
  }
  sort(ts.begin(), ts.end());
  for (i=0; i<(int)ts.size(); i++){
    double t = ts[i];
    double zx = (1-t)*A.x + t*B.x;
    double zy = (1-t)*A.y + t*B.y;
    if (check(zx, zy)){
      printf("%.10lf %.10lf\n", zx, zy);
      return 0;
    }
  }
  printf("Impossible\n");
  return 0;
}
