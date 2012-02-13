#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

typedef long double LD;
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
 printf ("%.14lf %.14lf %.14lf\n", (double)x.x, (double)x.y, (double)x.z);
}

void outp (TD v) {
 outDot (rot (v, e1));
 outDot (rot (v, e2));
 outDot (rot (v, e3));
}

void solve (VD& V) {
 TD O (0, 0, 0);
 LD range = PI;
 int nstep = 5;
 TD bstD = O;
 LD bst = f (V, bstD);
 LD rrange = range;
 int t = 0;
 while (bst > 1E-6 && ++t) {
  for (int q = 0; q < 2; ++q)
  for (int _ = 0; _ < 20; ++_) {
   const double step = range / nstep;
   for (LD y = -range; y <= range; y += step / 3)
   for (LD z = -range; z <= range; z += step) 
   if (y * y + z * z <= range * range) {
    const TD D = O + TD (y, y, z);
    const LD cur = f (V, D);
    if (cur < bst) {
     bst = cur;
     bstD = D;
    }
   }
   O = bstD;
   range /= 1.1 + q;
  }
  rrange /= 1.2;
  range = rrange;
 }
 if (bst > 1E-4) {
  fprintf (stderr, "Solution not found, only %lf\n", (double)bst);
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
