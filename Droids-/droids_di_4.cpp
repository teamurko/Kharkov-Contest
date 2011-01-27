#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

typedef double LD;
const LD EPS = 1E-9;
const LD PI = 2 * acos (0.0);

struct TD {
 LD x, y, z;
 TD () {}
 TD (LD x, LD y, LD z) : x (x), y (y), z (z) {}
};

TD inTD () {
 double x, y, z;
 scanf ("%lf%lf%lf", &x, &y, &z);
 return TD (x, y, z);
}

TD operator+ (TD a, TD b) {
 return TD (a.x + b.x, a.y + b.y, a.z + b.z);
}
TD operator- (TD a, TD b) {
 return TD (a.x - b.x, a.y - b.y, a.z - b.z);
}
TD operator* (TD a, LD t) {
 return TD (a.x * t, a.y * t, a.z * t);
}
TD operator/ (TD a, LD t) {
 return TD (a.x / t, a.y / t, a.z / t);
}
TD operator* (LD t, TD a) {
 return TD (a.x * t, a.y * t, a.z * t);
}
TD operator% (TD a, TD b) {
 return TD (a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
LD operator* (TD a, TD b) {
 return a.x * b.x + a.y * b.y + a.z * b.z;
}

TD rot (TD v, TD x) {
 const LD fi = sqrt (v * v) * PI;
 if (fi * fi < EPS * EPS / 1000)
  return x;
 v = v / sqrt (v * v);
 const TD d = v * x * v;
 x = x - d;
 const TD y = v % x;
 return d + x * cos (fi) + y * sin (fi);
}

const TD e1 (1, 0, 0);
const TD e2 (0, 1, 0);
const TD e3 (0, 0, 1);

typedef std::vector<TD> VD;

struct TB {
 TD a, b, c;
 TB () {}
 TB (TD a, TD b, TD c) : a (a), b (b), c (c) {}
};

LD f (const VD& V, const TB& B) {
 LD ma = B.a * V[0];
 LD Ma = ma;
 LD mb = B.b * V[0];
 LD Mb = mb;
 LD mc = B.c * V[0];
 LD Mc = mc;
 for (int i = 0; i < (int)V.size(); ++i) {
  const LD aa = V[i] * B.a;
  if (aa > Ma)
   Ma = aa;
  if (aa < ma)
   ma = aa;
  const LD bb = V[i] * B.b;
  if (bb > Mb)
   Mb = bb;
  if (bb < mb)
   mb = bb;
  const LD cc = V[i] * B.c;
  if (cc > Mc)
   Mc = cc;
  if (cc < mc)
   mc = cc;
 }
 Ma -= ma;
 Mb -= mb;
 Mc -= mc;
 LD m = Ma < Mb ? Mc < Ma ? Mc : Ma : Mc < Mb ? Mc : Mb;
 LD M = Ma < Mb ? Mc < Mb ? Mb : Mc : Mc < Ma ? Ma : Mc;
 return M - m;
}

LD f (VD& V, TD v) {
 return f (V, TB (rot (v, e1), rot (v, e2), rot (v, e3)));
}

void outDot (TD x) {
 printf ("%.14lf %.14lf %.14lf\n", x.x, x.y, x.z);
}

void outp (TD v) {
 outDot (rot (v, e1));
 outDot (rot (v, e2));
 outDot (rot (v, e3));
}

void solve (VD& V) {
 TD O (0, 0, 0);
 LD range = PI;
 int nstep = 3;
 TD bstD = O;
 LD bst = f (V, bstD);
 TD X (1, 0, 0);
 TD Y (0, 1, 0);
 TD Z (0, 0, 1);
 for (int q = 0; q < 20; ++q)
 for (int _ = 0; _ < 25; ++_) {
  const double step = range / nstep;
  for (LD x = -range; x <= range; x += step)
  for (LD y = -range; y <= range; y += step*2)
  for (LD z = -range; z <= range; z += step*2) {
   const TD D = O + x * X + y * Y + z * Z;
   const LD cur = f (V, D);
   if (cur < bst) {
    bst = cur;
    bstD = D;
   }
  }
  const TD D = bstD - O;
  if (D * D > EPS) {
   const TD DD = D / sqrt (D * D);
   const TD XX = DD;
   TD YY = X % DD;
   if (YY * YY < EPS)
    YY = Y % DD;
   if (YY * YY < EPS)
    YY = Z % DD;
   const TD ZZ = XX % YY;
   X = XX;
   Y = YY;
   Z = ZZ;
  }
  O = bstD;
  range /= 1.03 + q * 0.15;
 }
 if (bst > 1E-4) {
  fprintf (stderr, "Solution not found, only %lf\n", bst);
  //exit (-1);
 }
 outp (bstD);
}

int main () {
 freopen ("droids.in", "r", stdin);
 freopen ("droids.out", "w", stdout);
 VD V;
 int n;
 scanf ("%d", &n);
 for (int i = 0; i < n; ++i)
  V.push_back (inTD ());
 solve (V);
 return 0;
}
