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

typedef PointT<ld> PointD;
typedef vector<PointD> DPoints;
typedef VectorT<ld> VectorD;
const ld EPS = 1e-9;

bool lexCompD(const PointD& a, const PointD& b)
{
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

bool isClockWise(const PointD& a, const PointD& b, const PointD& c)
{
    return vectProd(a, b, c) < -EPS;
}

bool isCounterClockWise(const PointD& a, const PointD& b, const PointD& c)
{
    return vectProd(a, b, c) > EPS;
}

DPoints removeConsequentColinear(DPoints points);
bool checkNoConsequentColinear(const DPoints& points);

DPoints convexHull(DPoints points)
{
    if (points.size() == 1) return points;
    sort(all(points), &lexCompD);
    PointD p1 = *points.begin(),  p2 = points.back();
    DPoints up, down;
    up.push_back(p1);
    down.push_back(p1);

    for1(i, points.size() - 1) {
        const PointD& pt = points[i];
        if (i + 1 == points.size() || isClockWise(p1, pt, p2)) {
            while (up.size() >= 2 &&
                   !isClockWise(up[up.size()-2], up.back(), pt)) {
                up.pop_back();
            }
            up.pb(pt);
        }
        if (i + 1 == points.size() || isCounterClockWise(p1, pt, p2)) {
            while (down.size() >= 2 &&
                   !isCounterClockWise(
                            down[down.size()-2], down.back(), pt)) {
                down.pop_back();
            }
            down.pb(pt);
        }
    }

    DPoints result;
    forv(i, down) result.pb(down[i]);
    reverse(all(up));
    REQUIRE(!up.empty(), "Up vector is empty");
    up.pop_back();
    forv(i, up) result.pb(up[i]);
    result = removeConsequentColinear(result);
    REQUIRE(checkNoConsequentColinear(result),
        "No three consequent colinear points are allowed in a convex hull.");
    return result;
}

ld dist(const PointD& a, const PointD& b)
{
    return hypotl(a.x - b.x, a.y - b.y);
}

ld dist2(const PointD& a, const PointD& b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

DPoints removeConsequentColinear(DPoints points)
{
    if (points.empty()) return points;
    DPoints res;
    res.pb(*points.begin());
    for1(i, points.size() - 1) {
        const PointD& pt = points[i];
        while (res.size() >= 2 &&
               fabsl(vectProd(res[res.size() - 2], res.back(), pt)) < EPS) {
            res.pop_back();
        }
        res.pb(pt);
    }
    while (res.size() >= 3 &&
           fabsl(vectProd(*res.begin(), res.back(),
                          res[res.size()-2])) < EPS) {
        res.pop_back();
    }
    return res;
}

bool checkNoConsequentColinear(const DPoints& points)
{
    if (points.size() <= 2) {
        return true;
    }
    int n = points.size();
    forv(i, points) {
        const PointD& start = points[i];
        const PointD& middle = points[(i + 1) % n];
        const PointD& end = points[(i + 2) % n];
        if (fabsl(vectProd(start, middle, end)) < EPS) {
            return false;
        }
    }
    return true;
}


void readData(DPoints& points)
{
    int n; cin >> n;
    REQUIRE(n >= 1 && n <= 40000, "N does not satisfy restrictions");
    points.resize(n);
    forn(i, n) {
        cin >> points[i].x >> points[i].y;
    }
}

ld height(const PointD& a, const PointD& b, const PointD& c)
{
    ld dst = dist(a, b);
    REQUIRE(dst > EPS, "Degenerate side");
    return vectProd(a, b, c) / dst;
}

VectorD rotate(const VectorD& v, ld alp)
{
    ld cs = cosl(alp);
    ld sn = sinl(alp);
    return VectorD(v.x * cs - v.y * sn, v.x * sn + v.y * cs);
}

pair<int, int> calc(const DPoints& ps, int a, int b, int lptr, int rptr, ld h)
{
    int n = ps.size();
    ld dety = (height(ps[a], ps[b], ps[lptr]) -
               height(ps[a], ps[b], ps[rptr])) / h;
    ld alp = acosl(dety);
    VectorD v = rotate(VectorD(ps[a], ps[b]), alp);
    pair<int, int> res(0, 0);
    if (vectProd(v, VectorD(ps[lptr], ps[(lptr + 1) % n])) < -EPS) {
        res.first = 1;
    }
    else if (vectProd(v, VectorD(ps[lptr], ps[(lptr + n - 1) % n])) < -EPS) {
//        res.first = -1;
    }
    if (vectProd(v, VectorD(ps[rptr], ps[(rptr + 1) % n])) > EPS) {
        res.second = 1;
    }
    else if (vectProd(v, VectorD(ps[rptr], ps[(rptr + n - 1) % n])) > EPS) {
//        res.second = -1;
    }
    return res;
}


ld getAns(const DPoints& ps, int a, int b, int lptr, int rptr, ld h)
{
    /*
    cerr << a << " " << b << endl;
    cerr << lptr << " " << rptr << endl;
    cerr << h << endl;
    */
    int n = ps.size();
    ld dety = (height(ps[a], ps[b], ps[lptr]) -
               height(ps[a], ps[b], ps[rptr])) / h;
    ld alp = acosl(dety);
    VectorD vec(ps[a], ps[b]);
    VectorD ort = VectorD(-vec.y, vec.x);
    ld width =
        height(ps[a], PointD(ps[a].x + ort.x, ps[a].y + ort.y), ps[rptr]) -
        height(ps[a], PointD(ps[a].x + ort.x, ps[a].y + ort.y), ps[lptr]);
    REQUIRE(width >= -EPS, "Width should be positive : " << width);
    return (width - dety * tanl(alp)) + h / fabsl(dety);
}


bool good(const pair<int, int>& res)
{
    return res.first == 0 || res.second == 0;
}

ld solve(const DPoints& points)
{
    cerr << "ch size : " << points.size() << endl;
    if (points.size() <= 2) {
        return dist(points[0], points.back());
    }
    int n = points.size();
    int opp = 0;
    forn(i, n) {
        if (height(points[0], points[1], points[i]) >
            height(points[0], points[1], points[opp])) {
            opp = i;
        }
    }
    ld curH = height(points[0], points[1], points[opp]);
    int lptr = 1, rptr = opp;
    while (lptr <= opp) {
        pii c = calc(points, 0, 1, lptr, rptr, curH);
        if (good(c)) {
            break;
        }
        lptr = (lptr + c.first) % n;
        rptr = (rptr + c.second) % n;
    }
    cerr << "x" << endl;
    ld ans = getAns(points, 0, 1, lptr, rptr, curH);
    for1(i, n - 1) {
        int ni = (i + 1) % n;
        ld hh = height(points[i], points[ni], points[(opp + 1) % n]);
        if (hh > curH) {
            curH = hh;
            opp = (opp + 1) % n;
        }
        pii c = calc(points, i, ni, lptr, rptr, curH);
        lptr = (lptr + c.first) % n;
        rptr = (rptr + c.second) % n;
        ans = min(getAns(points, i, ni, lptr, rptr, curH), ans);
    }
    return ans;
}

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }
    DPoints points;
    readData(points);
    ld ans = solve(convexHull(points)) * 2;
    cout.precision(10);
    cout << fixed << ans << endl;

    return 0;
}
