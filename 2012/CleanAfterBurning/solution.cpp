#include "../common/geometry.h"
#include <cassert>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <queue>
#include <iostream>
using namespace std;

void printUsage(const char* binary)
{
    cerr << binary << " " << "sol | check" << endl;
}

ll scalar(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y;
}

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    string action = argv[1];
    REQUIRE(action == "sol", "Unknown action " << argv[1]);

    Points tri(3);
    forn(i, 3) {
        cin >> tri[i].x >> tri[i].y;
    }
    REQUIRE(vectProd(tri[0], tri[1], tri[2]) != 0, "Degenerate triangle");
    if (vectProd(tri[0], tri[1], tri[2]) < 0) {
        swap(tri[1], tri[2]);
    }

    double ans = 1e10;
    forn(i, 3) {
        const Point& cur = tri[i];
        const Point& next = tri[(i + 1) % 3];
        const Point& opp = tri[(i + 2) % 3];
        if (scalar(Vector(cur, next), Vector(cur, opp)) < 0) {
            ans = min(ans, dist(cur, next) + dist(cur, opp));
        }
        else if (scalar(Vector(cur, next), Vector(next, opp)) > 0) {
            ans = min(ans, dist(cur, next) + dist(next, opp));
        }
        else  {
            REQUIRE(vectProd(cur, next, opp) > 0, "Negative area");
            double dst = dist(cur, next);
            ans = min(ans, dst + vectProd(cur, next, opp) / dst);
        }
    }
    cout.precision(10);
    cout << fixed << ans << endl;

    return 0;
}
