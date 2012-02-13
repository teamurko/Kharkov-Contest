#define NOFOOTER
#include "testlib.h"

#include "string"
#include "cstdio"
#include "cmath"
using namespace std;

const double EPS = 1.1e-7;

int main(int argc,char* argv[]) {
  registerTestlibCmd(argc,argv);
  string js = ans.readString();
  string ps = ouf.readString();
  if (js == "Impossible")
  {
    if (ps == "Impossible")
    {
      quit(_ok, "really Impossible");
    }
    quit(_wa, "it's Impossible");
  }
  if (ps == "Impossible")
  {
    quit(_wa, "it's possible");
  }
  double jx, jy, px, py;
  char ch;
  sscanf(js.c_str(), "%lf %lf", &jx, &jy);
  if (sscanf(ps.c_str(), "%lf %lf %c", &px, &py, &ch) != 2)
  {
    quit(_pe, "");
  }
  if (fabs(px-jx) > EPS || fabs(py-jy) > EPS)
  {
    quit(_wa, "wrong point");
  }
  quit(_ok, "ok");
}
