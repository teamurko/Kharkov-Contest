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

bool canAdd(const Cones& cones, const Cone& cone)
{
    forv(i, cones)
        if (intersect(cones[i], cone)) return false;
    return true;
}

Point genPoint(int minc = -500, int maxc = 500)
{
    return Point(rnd.next(minc, maxc), rnd.next(minc, maxc));
}

Cone genCone(const Point& p, int minr = 1, int maxr = 100, int minh = 1, int maxh = 100)
{
    return Cone(p, rnd.next(minr, maxr), rnd.next(minh, maxh));
}

void genRandomCones(int minr, int maxr, int minh, int maxh, int numCones, Cones* cones)
{
    cones->pb(genCone(genPoint(), minr, maxr, minh, maxh));
    forn(iteration, numCones-1) {
        Cone addingCone = genCone(genPoint(), minr, maxr, minh, maxh);
        while (!canAdd(*cones, addingCone)) {
            addingCone = genCone(genPoint(), minr, maxr, minh, maxh);        
        }
        cones->pb(addingCone);
    }
}

void printExit(const Cones& cones)
{
    cout << cones.size() << endl;
    forv(i, cones)
    {
        cout << cones[i].center.x << " " << cones[i].center.y << " "
            << cones[i].r << " " << cones[i].h << endl;
    }
    cout.flush();
    exit(0);
}

void test1()
{
    Cones cones;
    cones.pb(Cone(Point(0, 0), 1, 2));
    cones.pb(Cone(Point(0, 3), 1, 3));
    cones.pb(Cone(Point(0, 6), 1, 2));
    printExit(cones);
}

void test2()
{
    Cones cones;
    cones.pb(Cone(Point(0, 0), 1, 1));
    cones.pb(Cone(Point(4, 0), 1, 1));
    cones.pb(Cone(Point(0, 10), 1, 1));
    cones.pb(Cone(Point(0, 15), 1, 1));
    cones.pb(Cone(Point(0, 20), 1, 1));
    printExit(cones);
}

void test3()
{
    Cones cones;
    cones.pb(Cone(Point(0, 0), 10, 1));
    printExit(cones);
}

void testRandom(int numCones)
{
    Cones cones;
    genRandomCones(1, 100, 1, 100, numCones, &cones);
    printExit(cones);   
}

void testOneLine(int numCones, int minh, int maxh) 
{
    int x = -500, y = -500;
    int minr = 1, maxr = 10;
    Cones cones;
    while (numCones--)
    {
        int r = rnd.next(minr, maxr);
        assert(x <= 500 && y <= 500);
        cones.pb(Cone(Point(x, y), r, rnd.next(minh, maxh)));
        int shift = static_cast<int>(r / sqrt(2.0)) + 1;
        x += shift;
        y += shift;
    }
    printExit(cones);
}

void testTouching1()
{
    //answer 2
    Cones cones;
    cones.pb(Cone(Point(1, 0), 1, 2));
    cones.pb(Cone(Point(0, 3), 2, 4));
    cones.pb(Cone(Point(1, 6), 1, 2));
    cones.pb(Cone(Point(1, 10), 2, 7));
    cones.pb(Cone(Point(0, 14), 2, 4));
    printExit(cones);        
}

void testTouching2()
{
    //answer 2
    Cones cones;
    cones.pb(Cone(Point(0, -4), 2, 4));
    cones.pb(Cone(Point(1, 0), 2, 6));
    cones.pb(Cone(Point(0, 4), 2, 2));
    printExit(cones);
}

void testAlmostTouching()
{
    //answer 1
    Cones cones;
    cones.pb(Cone(Point(-500, 1), 100, 1));
    cones.pb(Cone(Point(0, 0), 2, 100));
    cones.pb(Cone(Point(500, 1), 100, 100));
}

int main(int argc, char** argv) 
{
    registerGen(argc, argv);
    if (argc != 2) {
        cerr << "Incorrect number of arguments" << endl;
        exit(1);
    }

    int test = atoi(argv[1]);
    if (test == 1) test1();
    if (test == 2) test2();
    if (test == 3) test3();

    if (test == 4) testOneLine(10, 1, 100);
    if (test == 5) testOneLine(100, 30, 30);
    if (test == 6) testOneLine(100, 1, 10);

    if (test == 7) testTouching1();
    if (test == 8) testTouching2();
    if (test == 9) testAlmostTouching();

    if (test <= 30) testRandom(100);
    if (test <= 40) testRandom(300);
    assert(false);
    return 0;
}
