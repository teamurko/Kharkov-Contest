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

void readAsteroid(Points& points, Vector& vector)
{
    int n;
    cin >> n;
    points.resize(n);
    forn(i, n) {
        cin >> points[i].x >> points[i].y;
    }
    cin >> vector.x >> vector.y;
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

double dist2(const Point& a, const Point& b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

Points naiveMinkovskySum(const Points& polyOne, const Points& polyTwo)
{
    Points points;
    points.reserve(polyOne.size() * polyTwo.size());
    forv(i, polyOne) {
        forv(j, polyTwo) {
            points.pb(polyOne[i]);
            points.back().x += polyTwo[j].x;
            points.back().y += polyTwo[j].y;
        }
    }
    return convexHull(points);
}

Points minkovskySum(const Points& polyOne, const Points& polyTwo)
{
    require(!polyOne.empty() && !polyTwo.empty(),
            "Polygons should not be empty.");
    Points sum;
    sum.pb(*polyOne.begin());
    sum.back().x += polyTwo.begin()->x;
    sum.back().y += polyTwo.begin()->y;
    int oni = 0, twi = 0;
    int n = polyOne.size();
    int m = polyTwo.size();
    while (true) {
        Vector one(polyOne[oni], polyOne[(oni + 1) % n]);
        Vector two(polyTwo[twi], polyTwo[(twi + 1) % m]);
        ll vp = vectProd(one, two);
        if (vp >= 0) {
            oni = (oni + 1) % n;
        }
        if (vp <= 0) {
            twi = (twi + 1) % m;
        }
        Point np(polyOne[oni].x + polyTwo[twi].x,
                 polyOne[oni].y + polyTwo[twi].y);
        if (np == *sum.begin()) {
            break;
        }
        sum.pb(np);
    }
    return sum;
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
    FracT() : x(T()), y(T()) { normalize(); }
    void normalize()
    {
        if (y < 0) {
            y = -y;
            x = -x;
        }
        T d = gcd(y, abs(x));
        y /= d;
        x /= d;
    }
    T x, y;
};

typedef FracT<ll> Frac;

Frac intersect(const Vector& v, const Point& a, const Point& b)
{
    ll d = det(v.x, a.x-b.x, v.y, a.y-b.y);
    if (d == 0) return Frac(1, 0);
    Frac t1(det(a.x, a.x-b.x, a.y, a.y-b.y), d);
    Frac t2(det(v.x, a.x, v.y, a.y), d);
    if (t1.x < 0 || t2.x < 0 || t2.x > t2.y) return Frac(1, 0);
    return t1;
}

Frac min(const Frac& a, const Frac& b)
{
    if (a.x * b.y < b.x * a.y) {
        return a;
    }
    else {
        return b;
    }
}

void printUsage(const char* binary)
{
    cerr << binary << " " << "sol | naive | check" << endl;
}

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    if (argc != 1) {
        printUsage(argv[0]);
        return 1;
    }
    Vector one, two;
    Points ao, at;
    readAsteroid(ao, one);
    readAsteroid(at, two);
    forv(i, at) {
        at[i].x = -at[i].x;
        at[i].y = -at[i].y;
    }
    cout.precision(10);
    cout << fixed;

    Points sum = minkovskySum(ao, at);
    require(sum == naiveMinkovskySum(ao, at),
            "Polygons sum methods give different results");

    Vector v(one.x - two.x, one.y - two.y);
    size_t nonneg = 0, nonpos = 0;
    Point origin;
    forv(i, sum) {
        ll vp = vectProd(v, Vector(origin, sum[i]));
        if (vp >= 0) {
            ++nonneg;
        }
        if (vp <= 0) {
            ++nonpos;
        }
    }
    if (nonneg == 0 || nonpos == 0) {
        cout << "No collision" << endl;
    }
    else {
        Frac ans(1, 0);
        forv(i, sum) {
            ans = min(ans, intersect(v, sum[i], sum[(i + 1) % sum.size()]));
        }
        cout << ans.x << "/" << ans.y << endl;
    }
    return 0;
}
