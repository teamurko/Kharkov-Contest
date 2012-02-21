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

#define forn(i, n) for(int i = 0; i < int(n); ++i)
#define for1(i, n) for(int i = 1; i <= int(n); ++i)
#define forv(i, v) forn(i, v.size())
#define pb push_back
#define all(v) v.begin(), v.end()
#define mp make_pair

typedef pair<int, int> pii;
typedef vector<int> vi;
typedef long long ll;
typedef long double ld;

void require(bool cond, const string& message = "Runtime error")
{
    if (!cond) {
        cerr << message << endl;
        assert(false);
    }
}

template <typename T>
struct PointT
{
    PointT() : x(T()), y(T()) { }
    PointT(T xx, T yy) : x(xx), y(yy) { }
    T x, y;
};

typedef PointT<ll> Point;
typedef vector<Point> Points;

void readData(Points& points)
{
    int n;
    cin >> n;
    points.resize(n);
    forn(i, n) {
        cin >> points[i].x >> points[i].y;
    }
}

bool lexComp(const Point& a, const Point& b)
{
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

template <typename T>
T vectProd(const PointT<T>& a, const PointT<T>& b, const PointT<T>& c)
{
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

bool isClockWise(const Point& a, const Point& b, const Point& c)
{
    return vectProd(a, b, c) < 0;
}

bool isCounterClockWise(const Point& a, const Point& b, const Point& c)
{
    return vectProd(a, b, c) > 0;
}

Points removeConsequentColinear(Points points);

Points convexHull(Points points)
{
    if (points.size() == 1) return points;
    sort(all(points), &lexComp);
    Point p1 = *points.begin(),  p2 = points.back();
    Points up, down;
    up.push_back(p1);
    down.push_back(p1);

    for1(i, points.size() - 1) {
        const Point& pt = points[i];
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

    Points result;
    forv(i, down) result.pb(down[i]);
    reverse(all(up));
    require(!up.empty(), "Up vector is empty");
    up.pop_back();
    forv(i, up) result.pb(up[i]);
    return removeConsequentColinear(result);
}

double dist(const Point& a, const Point& b)
{
    return hypot(a.x - b.x, a.y - b.y);
}

double dist2(const Point& a, const Point& b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

double naiveMinAreaEnclosingBBox(const Points& points)
{
    if (points.size() <= 2) return 0.0;
    int n = points.size();
    double result = 1e12;
    forv(i, points) {
        const Point& cur = points[i];
        const Point& next = points[(i+1) % n];
        int ap = i;
        forn(j, n) {
            if (vectProd(cur, next, points[j]) >
                vectProd(cur, next, points[ap])) {
                ap = j;
            }
        }
        Point ort(cur.x - (next.y - cur.y), cur.y + (next.x - cur.x));
        int dapr = i, dapl = i;
        forn(j, n) {
            ll vp = vectProd(cur, ort, points[j]);
            if (vectProd(cur, ort, points[dapr]) < vp) {
                dapr = j;
            }
            if (vectProd(cur, ort, points[dapl]) > vp) {
                dapl = j;
            }
        }
        double h = vectProd(cur, next, points[ap]);
        double w = vectProd(cur, ort, points[dapr]) -
                   vectProd(cur, ort, points[dapl]);
        require(w >= 0, "Negative width");
        result = min(result, h / dist2(cur, next) * w);
    }
    return result;
}

Points removeConsequentColinear(Points points)
{
    if (points.empty()) return points;
    Points res;
    res.pb(*points.begin());
    for1(i, points.size() - 1) {
        const Point& pt = points[i];
        while (res.size() >= 2 &&
               vectProd(res[res.size() - 2], res.back(), pt) == 0) {
            res.pop_back();
        }
        res.pb(pt);
    }
    while (res.size() >= 3 &&
           vectProd(*res.begin(), res.back(), res[res.size() - 2]) == 0) {
        res.pop_back();
    }
    return res;
}

bool checkNoConsequentColinear(const Points& points)
{
    if (points.size() <= 2) {
        return true;
    }
    int n = points.size();
    forv(i, points) {
        const Point& start = points[i];
        const Point& middle = points[(i + 1) % n];
        const Point& end = points[(i + 2) % n];
        if (vectProd(start, middle, end) == 0) {
            return false;
        }
    }
    return true;
}

double minAreaEnclosingBBox(const Points& points)
{
    int n = points.size();
    int lap = 1, rap = 1, ap = 1;
    double result = 1e12;
    bool first = true;
    forn(index, n) {
        const Point& cur = points[index];
        const Point& next = points[(index + 1) % n];
        while (vectProd(cur, next, points[(ap + 1) % n]) >
               vectProd(cur, next, points[ap])) ap = (ap + 1) % n;
        Point ort(cur.x - (next.y - cur.y), cur.y + (next.x - cur.x));
        while (vectProd(cur, ort, points[(lap + 1) % n]) <
               vectProd(cur, ort, points[lap])) lap = (lap + 1) % n;
        if (first) {
            first = false;
            rap = ap;
        }
        while (vectProd(cur, ort, points[(rap + 1) % n]) >
               vectProd(cur, ort, points[rap])) rap = (rap + 1) % n;

        double h = vectProd(cur, next, points[ap]);
        double w = vectProd(cur, ort, points[rap]) -
                   vectProd(cur, ort, points[lap]);
        require(w >= 0, "Negative width");
        result = min(result, h / dist2(cur, next) * w);
        //cerr << index << " " << lap << " " << ap << " " << rap << endl;
    }
    return result;
}

double solve(const Points& points)
{
    /*
    forv(i, points) {
        cerr << points[i].x << " : " << points[i].y << endl;
    }
    */
    const int MAX_NUM_POINTS_NAIVE = 4;
    if (points.size() <= MAX_NUM_POINTS_NAIVE) {
        return naiveMinAreaEnclosingBBox(points);
    }
    else {
        require(checkNoConsequentColinear(points),
                "There are three consequent colinear points");
        return minAreaEnclosingBBox(points);
    }
}

double solveNaive(const Points& points)
{
    return naiveMinAreaEnclosingBBox(points);
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
    cout.precision(10);
    cout << fixed;
    points = convexHull(points);
    if (var == "naive") {
        cout << solveNaive(points) << endl;
    }
    else {
        double ans = solve(points);
        if (var == "check") {
            double naive = solveNaive(points);
            ostringstream os;
            os << "Naive and correct solutions differ"
               << "  ans : " << ans << " naive : " << naive << endl;
            require(ans == naive, os.str());
        }
        cout << ans << endl;
    }

    return 0;
}
