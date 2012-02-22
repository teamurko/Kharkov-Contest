#include "testlib.h"
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

template <typename T>
struct VectorT
{
    VectorT() : x(T()), y(T()) { }
    VectorT(T xx, T yy) : x(xx), y(yy) { }
    VectorT(const PointT<T>& start, const PointT<T>& end) :
        x(end.x - start.x), y(end.y - start.y) { }
    T x, y;
};

typedef PointT<ll> Point;
typedef VectorT<ll> Vector;
typedef vector<Point> Points;

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool lexComp(const Point& a, const Point& b)
{
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

template <typename T>
T vectProd(const VectorT<T>& a, const VectorT<T>& b)
{
    return a.x * b.y - a.y * b.x;
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
bool checkNoConsequentColinear(const Points& points);

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
    result = removeConsequentColinear(result);
    require(checkNoConsequentColinear(result),
        "No three consequent colinear points are allowed in a convex hull.");
    return result;
}

double dist(const Point& a, const Point& b)
{
    return hypot(a.x - b.x, a.y - b.y);
}

ll dist2(const Point& a, const Point& b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
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

template <typename T>
T det(T a, T b, T c, T d)
{
    return a * d - b * c;
}

void printUsage()
{
    cerr << "Usage: binary num-test psize psize2 max-coord" << endl;
}

Point nextPoint(int maxv)
{
    return Point(rnd.next(-maxv, maxv), rnd.next(-maxv, maxv));
}

Point nonZero(int maxv)
{
    Point pt = nextPoint(maxv);
    while (pt.x == 0 && pt.y == 0) {
        pt = nextPoint(maxv);
    }
    return pt;
}

void print(const Points& ps)
{
    cout << ps.size() << endl;
    forv(i, ps) {
        cout << ps[i].x << " " << ps[i].y << endl;
    }
}

// sample 1
void test1()
{
    printf("4\n0 0\n1 0\n1 1\n0 1\n0 0\n3\n2 0\n3 0\n3 1\n-1 0\n");
    exit(0);
}

// sample 2
void test2()
{
    printf("4\n0 0\n1 0\n1 1\n0 1\n-1 1\n4\n-1 3\n0 3\n0 4\n-1 4\n1 -1\n");
    exit(0);
}

// sample 3
void test3()
{
    printf("4\n0 0\n1 0\n1 1\n0 1\n0 0\n3\n2 0\n3 0\n3 1\n0 1\n");
    exit(0);
}


void specTest4()
{
    cout << 3 << endl;
    cout << "10 10\n";
    cout << "20 20\n";
    cout << "10 20\n";
    cout << "20 0\n";
    cout << 3 << endl;
    cout << "100 10\n";
    cout << "110 10\n";
    cout << "110 20\n";
    cout << "10 0\n";
    exit(0);
}

void specTest5()
{
    cout << 3 << endl;
    cout << "10 10\n";
    cout << "20 20\n";
    cout << "10 20\n";
    cout << "20 0\n";
    cout << 3 << endl;
    cout << "100 10\n";
    cout << "110 10\n";
    cout << "110 20\n";
    cout << "-10000 0\n";
    exit(0);
}

void specTest6()
{
    cout << 3 << endl;
    cout << "10 10\n";
    cout << "20 20\n";
    cout << "10 20\n";
    cout << "20 0\n";
    cout << 3 << endl;
    cout << "100 10\n";
    cout << "110 10\n";
    cout << "110 20\n";
    cout << "20 0\n";
    exit(0);
}

void specTest7()
{
    cout << 3 << endl;
    cout << "0 0\n";
    cout << "1 0\n";
    cout << "0 1\n";
    cout << "0 10000\n";
    cout << 3 << endl;
    cout << "2 11\n";
    cout << "1 10\n";
    cout << "2 10\n";
    cout << "0 -10000\n";
    exit(0);
}

void specTest8()
{
    cout << 3 << endl;
    cout << "-1 0\n";
    cout << "0 0\n";
    cout << "-1 1\n";
    cout << "0 0\n";
    cout << 3 << endl;
    cout << "-1 5000\n";
    cout << "0 5000\n";
    cout << "0 10000\n";
    cout << "2 -9999\n";
    exit(0);
}

void specTest9()
{
    cout << 3 << endl;
    cout << "-1 0\n";
    cout << "0 0\n";
    cout << "-1 1\n";
    cout << "0 0\n";
    cout << 3 << endl;
    cout << "-1 5000\n";
    cout << "0 5000\n";
    cout << "0 10000\n";
    cout << "2 -10000\n";
    exit(0);
}

void maxTest20()
{
}

void maxTest21()
{

}

int main(int argc, char* argv[])
{
    registerGen(argc, argv);
    if (argc != 5) {
        printUsage();
        return 1;
    }

    int test = atoi(argv[1]);
    switch (test) {
        case 1: test1();
        case 2: test2();
        case 3: test3();
        case 4: specTest4();
        case 5: specTest5();
        case 6: specTest6();
        case 7: specTest7();
        case 8: specTest8();
        case 9: specTest9();
        case 20: maxTest20();
        case 21: maxTest21();
    }

    int psize = atoi(argv[2]);
    int psize2 = atoi(argv[3]);
    int maxv = atoi(argv[4]);

    assert(maxv > 0);

    set<pair<int, int> > points;
    for (int i = 0; i < (psize + psize2) * 10; ++i) {
        pair<int, int> pt(rnd.next(-maxv, maxv), rnd.next(-maxv, maxv));
        while (points.count(pt)) {
            pt = make_pair(rnd.next(-maxv, maxv), rnd.next(-maxv, maxv));
        }
        points.insert(pt);
    }

    vector<pair<int, int> > tmp(all(points));
    Points pts(points.size());

    forv(i, pts) pts[i].x = tmp[i].first, pts[i].y = tmp[i].second;
    shuffle(all(pts));

    Vector vect(rnd.next(1, maxv), rnd.next(-maxv, maxv));
    Points first, second;

    forv(i, pts) {
        ll vp = vectProd(vect, Vector(pts[i].x, pts[i].y));
        if (vp > 0) {
            first.pb(pts[i]);
        } else if (vp < 0) {
            second.pb(pts[i]);
        }
    }

    first = convexHull(first);
    while (psize < first.size()) first.pop_back();

    second = convexHull(second);
    while (psize2 < second.size()) second.pop_back();

    print(first);
    Point v = nextPoint(maxv);
    cout << v.x << " " << v.y << endl;

    print(second);
    v = nextPoint(maxv);
    cout << v.x << " " << v.y << endl;

    return 0;
}
