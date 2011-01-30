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

typedef size_t Id;
typedef vector<Id> Ids;
typedef vector<Ids> Graph;

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

int bfs(const Graph& graph)
{
    const int INF = 100000;
    queue<int> q;
    vector<int> d(cones.size(), INF);
    q.push(0);
    d[0] = 0;
    while (!q.empty()) {
        Id from = q.front(); q.pop();
        forv(i, graph[from]) {
            Id to = graph[from][i];
            if (d[to] == INF) {
                d[to] = d[from] + 1;
                q.push(to);
            }
        }    
    }
    if (d.back() == INF) return -1;
    return d.back();
}

//TODO
bool visible(const Cone& one, const Cone& two, const Cone& middle)
{
    
}

bool visible(const Cones& cones, int one, int two)
{
    forv(i, cones) {
        if (i == one || i == two) continue;
        if (!visible(cones[one], cones[two], cones[i])) return false;
    }
    return true;
}

void buildGraph(const Cones& cones, Graph* graph)
{
    graph->resize(cones.size());
    forv(one, cones) {
        forn(two, toIndex) {
            if (visible(cones, one, two)) {
                (*graph)[one].pb(two);
                (*graph)[two].pb(one);                
            }            
        }
    }    
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