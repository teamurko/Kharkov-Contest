#include "testlib.h"
#include <cmath>

using namespace std;

const double EPS = 1E-4;

string ending(int x)
{
    x %= 100;
    if (x / 10 == 1)
        return "th";
    if (x % 10 == 1)
        return "st";
    if (x % 10 == 2)
        return "nd";
    if (x % 10 == 3)
        return "rd";
    return "th";
}

int main(int argc, char * argv[])
{
    setName("compare two sequences of doubles, max absolute or relative error = %.5lf", EPS);
    registerTestlibCmd(argc, argv);

    int n = 0;
    double j, p;

    while (!ans.seekEof()) 
    {
      n++;
      j = ans.readDouble();
      p = ouf.readDouble();
      if (!doubleCompare(j, p, EPS))
        quitf(_wa, "%d%s numbers differ - expected: '%.5lf', found: '%.5lf', error = '%.5lf'", n, ending(n).c_str(), j, p, doubleDelta(j, p));
    }

    if (n == 1)
        quitf(_ok, "found '%.5lf', expected '%.5lf', error '%.5lf'", p, j, doubleDelta(j, p));

    quitf(_ok, "%d numbers", n);
}
