#ifndef COMMON_GEOMETRY_H
#define COMMON_GEOMETRY_H

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#define forn(i, n) for(int i = 0; i < int(n); ++i)
#define for1(i, n) for(int i = 1; i <= int(n); ++i)
#define forv(i, v) forn(i, v.size())
#define pb push_back
#define all(v) v.begin(), v.end()
#define mp make_pair

#define REQUIRE(cond, message) \
    if (!(cond)) { std::cerr << message << std::endl; }

typedef std::pair<int, int> pii;
typedef std::vector<int> vi;
typedef long long ll;
typedef long double ld;

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
typedef std::vector<Point> Points;

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
    REQUIRE(!up.empty(), "Up vector is empty");
    up.pop_back();
    forv(i, up) result.pb(up[i]);
    result = removeConsequentColinear(result);
    REQUIRE(checkNoConsequentColinear(result),
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

template <typename T>
T gcd(T a, T b)
{
    if (a == 0) return b;
    return gcd(b % a, a);
}

template <typename T>
struct FracT
{
    FracT(T xx, T yy) : x(xx), y(yy) { normalize(); }
    FracT() : x(T()), y(T(1)) { normalize(); }
    void normalize()
    {
        if (y < 0) {
            y = -y;
            x = -x;
        }
        T ux = x;
        if (ux < 0) ux = -ux;
        T d = gcd(y, ux);
        y /= d;
        x /= d;
    }
    T x, y;
};

typedef FracT<ll> Frac;

#endif // COMMON_GEOMETRY_H
