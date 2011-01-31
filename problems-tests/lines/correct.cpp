#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include <set>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cassert>
#include <utility>

using namespace std;

#define EPS 1E-8

#define forn(i, n) for (int i = 0; i < int(n); i++)
#define forv(i, a) for (int i = 0; i < int(a.size()); i++)
#define fors(i, a) for (int i = 0; i < int(a.length()); i++)
#define all(a) a.begin(), a.end()
#define pb push_back
#define mp make_pair
#define VI vector<int>
#define VS vector<string>

#define norm(a) sort(all(a)); a.erase(unique(all(a)), a.end());
#define num(a, v) (int)(lower_bound(all(a), v) - a.begin())

int n;

typedef vector<long long> point;

point p1, p2, v1, v2;

point operator + (const point& p1, const point& p2) {
    point p(p1.size());
    forv(i, p) p[i] = p1[i] + p2[i];
    return p;
}

point operator - (const point& p1, const point& p2) {
    point p(p1.size());
    forv(i, p) p[i] = p1[i] - p2[i];
    return p;
}

point operator * (const point& p1, long long b) {
    point p(p1.size());
    forv(i, p) p[i] = p1[i] * b;
    return p;
}

point readPoint() {
    point p(n);
    forn(i, n) {
        int cur;
        assert(scanf("%d", &cur) == 1);
        p[i] = cur;
        assert(-2000 <= p[i] && p[i] <= 2000);
    }
    return p;
}

long long scalar(const point& p1, const point& p2) {
    long long res = 0;
    forv(i, p1) res += p1[i] * p2[i];
    return res;
}

long long scalar2(const point& p1) {
    return scalar(p1, p1);
}

void outdata() {
}

inline long long sqr(long long x) {
    return x * x;
}

bool parall(const point& v1, const point& v2) {
    return scalar2(v1) * scalar2(v2) == sqr(scalar(v1, v2));
}

void solve() {
    if (parall(v1, v2)) {
        //parall or eq
        if (parall(p2 - p1, v1)) {
            printf("equal\n");
        } else {
            printf("parallel\n");
        }
    } else {
        // cross or skew
        long long a1, b1, c1, a2, b2, c2;
        a1 = scalar(v1, v1);
        b1 = -scalar(v1, v2);
        c1 = scalar(p2 - p1, v1);
        a2 = scalar(v1, v2);
        b2 = -scalar(v2, v2);
        c2 = scalar(p2 - p1, v2);
        long long d, d1, d2;
        d = a1 * b2 - a2 * b1;
        d1= c1 * b2 - c2 * b1;
        d2= a1 * c2 - a2 * c1;
        assert(d != 0);
        point x1, x2;
        x1 = p1 * d + v1 * d1;
        x2 = p2 * d + v2 * d2;
        if (x1 == x2) {
            printf("cross\n");
        } else {
            printf("skew\n");
        }
    }
}

void readdata() {
    point cp1, cp2;
    assert(2 <= n && n <= 50);
    p1 = readPoint();
    cp1 = readPoint();
    v1 = cp1 - p1;
    bool ok = false;
    forv(i, v1) if (v1[i] != 0) ok = true;
    assert(ok);
    p2 = readPoint();
    cp2 = readPoint();
    v2 = cp2 - p2;
    ok = false;
    forv(i, v2) if (v2[i] != 0) ok = true;
    assert(ok);
}

int main() {
    while (true) {
        assert(scanf("%d", &n) == 1);
        if (n == 0) break;
        readdata();
        solve();
        outdata();
    }
    return 0;
}
