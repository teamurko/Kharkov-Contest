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

vi toVec(ll n)
{
    vi r;
    while (n > 0) {
        r.pb(n % 10);
        n /= 10;
    }
    if (r.empty()) r.pb(0);
    return r;
}

vi mul(const vi& a, const vi& b)
{
    vi c(a.size() + b.size());
    forv(i, c) {
        forn(j, i + 1) {
            if (j < a.size() && i - j < b.size()) {
                c[i] += a[j] * b[i - j];
            }
        }
        if (c[i] > 10) {
            c[i + 1] += c[i] / 10;
            c[i] %= 10;
        }
    }
    while (c.size() > 1 && c.back() == 0) c.pop_back();
    return c;
}

vi mul(ll na, ll nb)
{
    REQUIRE(na >= 0 && nb >= 0, "Cannot multiply negative numbers!");
    vi a = toVec(na);
    vi b = toVec(nb);
    return mul(a, b);
}

int cmp(const vi& a, const vi& b)
{
    REQUIRE(!a.empty() && !b.empty(), "Empty big integers!");
    if (a.size() > b.size()) {
        return 1;
    } else if (a.size() < b.size()) {
        return -1;
    } else {
        for (int i = a.size() - 1; i >= 0; --i) {
            if (a[i] > b[i]) return 1;
            else if (a[i] < b[i]) return -1;
        }
    }
    return 0;
}

void cancel(ll& a, ll& b)
{
    ll d = gcd(a, b);
    a /= d;
    b /= d;
}

template <typename T>
T update(const T& res, ll a, ll b, ll c)
{
    REQUIRE(a >= 0 && b >= 0, "Negative width or heigth");
    REQUIRE(c > 0, "Distance should be positive");
    cancel(a, c);
    cancel(b, c);
    vi pre = mul(a, b);
    if (cmp(mul(res.first, toVec(c)),
            mul(toVec(res.second), pre)) == 1) {
        return mp(pre, c);
    }
    return res;
}

pair<vi, ll> naiveMinAreaEnclosingBBox(const Points& points)
{
    if (points.size() <= 2) return mp(vi(1, 0), 1);
    int n = points.size();
    pair<vi, ll> res = mp(vi(1, 1), 0);
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
        ll h = vectProd(cur, next, points[ap]);
        ll w = vectProd(cur, ort, points[dapr]) -
                   vectProd(cur, ort, points[dapl]);
        ll dst = dist2(cur, next);
        res = update(res, h, w, dst);
    }
    return res;
}

pair<vi, ll> minAreaEnclosingBBox(const Points& points)
{
    int n = points.size();
    int lap = 1, rap = 1, ap = 1;
    pair<vi, ll> result(vi(1, 1), 0);
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

        ll h = vectProd(cur, next, points[ap]);
        ll w = vectProd(cur, ort, points[rap]) -
                   vectProd(cur, ort, points[lap]);
        ll dst = dist2(cur, next);
        result = update(result, h, w, dst);
    }
    return result;
}

pair<vi, ll> solve(const Points& points)
{
    cerr << "ch size : " << points.size() << endl;
    const int MAX_NUM_POINTS_NAIVE = 4;
    if (points.size() <= MAX_NUM_POINTS_NAIVE) {
        return naiveMinAreaEnclosingBBox(points);
    }
    else {
        REQUIRE(checkNoConsequentColinear(points),
                "There are three consequent colinear points");
        return minAreaEnclosingBBox(points);
    }
}

pair<vi, ll> solveNaive(const Points& points)
{
    return naiveMinAreaEnclosingBBox(points);
}

void printUsage(const char* binary)
{
    cerr << binary << " " << "sol | naive | check" << endl;
}

ostream& operator<<(ostream& os, const pair<vi, ll>& obj)
{
    for (int i = obj.first.size() - 1; i >= 0; --i) {
        os << obj.first[i];
    }
    os << "/";
    os << obj.second;
    return os;
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
    points = convexHull(points);
    if (var == "naive") {
        cout << solveNaive(points) << endl;
    }
    else {
        pair<vi, ll> ans = solve(points);
        if (var == "check") {
            pair<vi, ll> naive = solveNaive(points);
            REQUIRE(ans == naive, "Naive and correct solutions differ"
                                  << "  ans : " << ans << " naive : "
                                  << naive);
        }
        cout << ans << endl;
    }

    return 0;
}
