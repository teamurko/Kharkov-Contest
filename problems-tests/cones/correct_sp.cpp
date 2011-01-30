#include "testlib.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
using namespace std;

#define forn(i, n) for(int i = 0; i < static_cast<int>(n); ++i)
#define for1(i, n) for(int i = 1; i <= static_cast<int>(n); ++i)
#define forv(i, v) forn(i, v.size())
#define all(v) v.begin(), v.end()
#define pb push_back

int sqr(int x) 
{
    return x * x;
}

struct Point
{
    Point() {}
    Point(int x_, int y_) : x(x_), y(y_) {}
    int x, y;
};

struct Cone
{
    Cone() {}
    Cone(const Point& p, int r_, int h_) : center(p), r(r_), h(h_) {}
    Point center;
    int r, h;   
};

typedef vector<Cone> Cones;

int dist2(const Point& a, const Point& b)
{
    return sqr(a.x - b.x) + sqr(a.y - b.y);
}

bool intersect(const Cone& a, const Cone& b)
{
    return dist2(a.center, b.center) <= sqr(a.r + b.r);
}

void bfs(const Cones& cones)
{
    const int INF = 100000;
    queue<int> q;
    vector<int> d(cones.size(), INF);
    q.push(0);
    d[0] = 0;

}

int main()
{
    int numCones;
    cin >> numCones;
    Cones cones;
    forn(i, numCones) {
        int x, y, r, h;
        cin >> x >> y >> r >> h;
        cones.pb(Cones(Point(x, y), r, h));
    }
    return 0;
}