#include "testlib.h"

#include <vector>

int main (int argc, char** argv) {
 char msg[100];
 registerTestlibCmd (argc, argv);
 double a[3][3];
 for (int i = 0; i < 3; ++i)
 for (int j = 0; j < 3; ++j)
  a[i][j] = ouf.readDouble ();
 for (int i = 0; i < 3; ++i)
 for (int j = i; j < 3; ++j) {
  double x = 0;
  for (int k = 0; k < 3; ++k)
   x += a[i][k] * a[j][k];
  if (!doubleCompare (i == j, x, 1E-7))
   quit (_wa, (sprintf (msg, "not a unit basis v[%d] * v[%d] = %.14lf", i, j, x), msg));
 }
 const int n = inf.readInt ();
 double m[3], M[3];
 for (int i = 0; i < n; ++i) {
  const double x = inf.readDouble ();
  const double y = inf.readDouble ();
  const double z = inf.readDouble ();
  for (int j = 0; j < 3; ++j) {
   const double t = x * a[j][0] + y * a[j][1] + z * a[j][2];
   if (!i || t < m[j])
    m[j] = t;
   if (!i || t > M[j])
    M[j] = t;
  }
 }
 const double X = M[0] - m[0];
 const double Y = M[1] - m[1];
 const double Z = M[2] - m[2];
 const double EPS = 1E-5;
 if (! doubleCompare (0.0, X - Y, EPS))
  quit (_wa, (sprintf (msg, "X != Y: %.12lf != %.12lf", X, Y), msg));
 if (! doubleCompare (0.0, X - Z, EPS))
  quit (_wa, (sprintf (msg, "X != Z: %.12lf != %.12lf", X, Z), msg));
 if (! doubleCompare (0.0, Y - Z, EPS))
  quit (_wa, (sprintf (msg, "Y != Z: %.12lf != %.12lf", Y, Z), msg));
 quit (_ok, "Correct!");
 return 0;
}
