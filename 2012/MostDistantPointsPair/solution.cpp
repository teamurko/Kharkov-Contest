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

void readData(Points& points)
{
    int n;
    cin >> n;
    points.resize(n);
    forn(i, n) {
        cin >> points[i].x >> points[i].y;
    }
}

ll naiveMostDistant(const Points& points)
{
    ll result = 0;
    forv(j, points) {
        forn(i, j) {
            result = max(result, dist2(points[i], points[j]));
        }
    }
    return result;
}

ll mostDistant(const Points& points)
{
    int n = points.size();
    int lap = 0, rap = 1;
    ll result = 0;
    forn(index, n) {
        int prevIndex = (index + n - 1) % n;
        const Point& prev = points[prevIndex];
        const Point& cur = points[index];
        const Point& next = points[(index + 1) % n];
        while (vectProd(prev, cur, points[(lap + 1) % n]) >
               vectProd(prev, cur, points[lap])) lap = (lap + 1) % n;
        while (vectProd(cur, next, points[(rap + 1) % n]) >=
               vectProd(cur, next, points[rap])) rap = (rap + 1) % n;
        for (int i = lap; i != rap; i = (i + 1) % n) {
            result = max(result, dist2(cur, points[i]));
        }
        result = max(result, dist2(cur, points[rap]));
    }
    return result;
}

ll solve(const Points& points)
{
    cerr << "ch size : " << points.size() << endl;
    const int MAX_NUM_POINTS_NAIVE = 4;
    if (points.size() <= MAX_NUM_POINTS_NAIVE) {
        return naiveMostDistant(points);
    }
    else {
        REQUIRE(checkNoConsequentColinear(points),
                "There are three consequent colinear points");
        return mostDistant(points);
    }
}

ll solveNaive(const Points& points)
{
    return naiveMostDistant(points);
}

void printUsage(const char* binary)
{
    cerr << binary << " " << "sol | naive | check" << endl;
}

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }
    string var = argv[1];
    Points points;
    readData(points);
    if (var == "naive") {
        cout << solveNaive(points) << endl;
    }
    else {
        ll ans = solve(convexHull(points));
        if (var == "check") {
            ll naive = solveNaive(points);
            ostringstream os;
            os << "Naive and correct solutions differ"
               << "  ans : " << ans << " naive : " << naive << endl;
            REQUIRE(ans == naive, os.str());
        }
        cout << ans << endl;
    }

    return 0;
}
