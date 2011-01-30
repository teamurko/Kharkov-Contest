#include "testlib.h"
#include <iostream>
#include <queue>
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

template<typename T>
T sqr(T x) 
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
    vector<int> d(graph.size(), INF);
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

typedef long long Long;
struct Frac
{
    Frac(Long a_, Long b_) : a(a_), b(b_)
    {
        if (b < 0) {
            b = -b;
            a = -a;
        }
    }
    Long a, b;
};

bool operator<(const Frac& f1, const Frac& f2)
{
    return f1.a * f2.b < f1.b * f2.a;
}

//TODO
bool visible(const Cone& one, const Cone& two, const Cone& middle)
{
    Frac a(0, 1), b(1, 1);
    Long y1 = one.center.y, x1 = one.center.x, z1 = one.h;
    Long y2 = two.center.y, x2 = two.center.x, z2 = two.h;
    Long y0 = middle.center.y, x0 = middle.center.x, h = middle.h, r = middle.r;

    if (z1 == z2) {
        if (middle.h < z1) return true;
    }
    else {
        if (z1 > z2) {
            if (a < Frac(z1 - h, z1 - z2)) {    
                a = Frac(z1 - h, z1 - z2);
            }
        }
        else {
            if (Frac(h - z1, z2 - z1) < b) {
                b = Frac(h - z1, z2 - z1);
            }
        }
    }
    if (b < a) return true;

    //z(t) = z1 + (z2 - z1) * t;
    //y(t) = y1 + (y2 - y1) * t;
    //x(t) = x1 + (x2 - x1) * t;
    //r(t) = r * (h - z(t)) / h;
    //(x(t) - x0) ^ 2 + (y(t) - y0) ^ 2 <= r^2(t);
    //((x2-x1)*t + x1-x0)^2 + ((y2-y1)*t + y1-y0)^2 <= r*r/h/h*(h-z1 - (z2-z1)*t)^2


    //sqr(x2-x1)t^2*h^2 + 2(x1-x0)*(x2-x1)t*h^2 + sqr(x1-x0)*h^2 + 
    //sqr(y2-y1)t^2*h^2 + 2(y1-y0)*(y2-y1)t*h^2 + sqr(y1-y0)*h^2 <=
    //r^2(sqr(h-z1) - 2(h-z1)(z2-z1)t + sqr(z2-z1)t^2)
    Long A = sqr(x2 - x1) * h * h + sqr(y2 - y1) * h * h - sqr(z2-z1) * r * r;
    Long B = 2 * (x1 - x0) * (x2 - x1) * h * h + 2 * (y1 - y0) * (y2 - y1) * h * h +
        2 * (h - z1) * (z2 - z1) * r * r;
    Long C = sqr(x1 - x0) * h * h + sqr(y1 - y0) * h * h - sqr(h - z1) * r * r;
    // A * t^2 + B * t + C <= 0, t in [a, b] (1)
    // A == 0
    if (A == 0) {
        if (B == 0) {
            return C != 0;
        }
        if (B < 0) {
            Frac f(-C,-B);
            return b < f;
        }
        else {
            Frac f(C, B);
            return f < a;
        }
    }

    // -B +- sqrt(B * B - 4 * A * C) = -B/2/A +- sqrt(B/A*B/A - 4 * C / A)

    const double EPS = 1e-6;
    double D = sqr(B/1.0/A) - 4 * C / static_cast<double>(A);
//    double D = B * (double)B - 4 * A * (double)C;
    // If (1) doesn't have roots, then return true iff A > 0 
    if (D < -EPS) {
        return A > 0;
    }    
    D = sqrt(fabs(D));
    double root1 = -B/2.0/A + D/2.0, root2 = -B/2.0/A - D/2.0;
    // Let r1 and r2 are roots (1) (r1 <= r2).
    // Check if r1 or r2 in [a, b] or a or b satisfy (1)
    if (A * a.a * a.a + B * a.a * a.b + C * a.b * a.b <= 0) {   
        return false;
    }
    if (A * b.a * b.a + B * b.a * b.b + C * b.b * b.b <= 0) {
        return false;
    }
    if (a.a < a.b * root1 + EPS && root1 * b.b < b.a + EPS) {
        return false;
    }
    if (a.a < a.b * root2 + EPS && root2 * b.b < b.a + EPS) {
        return false;
    }
    return true;
}

void testVisible()
{
    while (true) {
    Point p1, p2, p0;
    int r1, h1, r2, h2, r0, h0;
    cin >> p1.x >> p1.y >> r1 >> h1;
    cin >> p2.x >> p2.y >> r2 >> h2;
    cin >> p0.x >> p0.y >> r0 >> h0;
    Cone one(p1, r1, h1);
    Cone middle(p0, r0, h0);
    Cone two(p2, r2, h2);
    cerr << visible(one, two, middle) << endl;
    }
    exit(0);
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
    forv(two, cones) {
        forn(one, two) {
            if (visible(cones, one, two)) {
                (*graph)[one].pb(two);
                (*graph)[two].pb(one);                
            }            
        }
    }    
}

int main()
{
//    testVisible();
//    freopen("49", "rt", stdin);
    int numCones;
    cin >> numCones;
    Cones cones;
    forn(i, numCones) {
        int x, y, r, h;
        cin >> x >> y >> r >> h;
        cones.pb(Cone(Point(x, y), r, h));
    }
    Graph graph;
    buildGraph(cones, &graph);
    /*
    cout << graph.size() << endl;
    forv(i, graph) {
        forv(j, graph[i]) {
            cout << graph[i][j];
            if (j + 1 < graph[i].size()) cout << " ";
        }
        assert(graph[i].size() > 0);
        cout << endl;
    }
    */
    cout << bfs(graph) << endl;
    return 0;
}
