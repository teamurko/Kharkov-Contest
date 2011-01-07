#include "point.h"
#include "plane.h"
#include "testlib.h"
#include "utils.h"

#include <iostream>

using namespace std;

#define forn(i, n) for(int i = 0; i < int(n); ++i)
#define forv(i, v) forn(i, v.size())

void print(const Points& points)
{
    cout << points.size() << endl;
    forv(i, points) {
        cout << points[i] << endl;
    }
}

void test1()
{
    Points points;
    points.push_back(Point(0, 0, 0));
    points.push_back(Point(1, 0, 0));
    points.push_back(Point(0, 1, 0));
    points.push_back(Point(0, 0, 1));
    print(points);
    exit(0);
}

void test2()
{
    Points points;
    points.push_back(Point(0, 0, 0));
    points.push_back(Point(10, 0, 0));
    points.push_back(Point(0, 10, 0));
    points.push_back(Point(10, 10, 10));
    print(points);
    exit(0);
}

void test3()
{
    cout << 5 << endl;
    cout << "0 0 0" << endl;
    cout << "10 0 0" << endl;
    cout << "0 10 0" << endl;
    cout << "10 10 10" << endl;
    cout << "5 5 1" << endl;
    exit(0);
}

void test4()
{
    Points points;
    points.push_back(Point(0, 0, 0));
    points.push_back(Point(10, 0, 0));
    points.push_back(Point(0, 10, 0));
    points.push_back(Point(10, 10, 10));
    points.push_back(Point(5, 5, 10));
    print(points);
    exit(0);
}

Point nextPoint()
{
    const int MINC = -500;
    const int MAXC = 500;
    return Point(rnd.next(MINC, MAXC+1), rnd.next(MINC, MAXC+1), rnd.next(MINC, MAXC+1));
}

void randomSmall()
{
    int n = rnd.next(6, 21);
    Point first = nextPoint();
    Point second = nextPoint();
    while (second == first) {   
        second = nextPoint();
    }
    Point third = nextPoint();
    Point zero(0, 0, 0);
    while (ortVector(first, second, third) == zero) {
        third = nextPoint();
    }

    Points points;
    points.push_back(first);
    points.push_back(second);
    points.push_back(third);

    forn(iter, n-3) {
        Point point;
        bool ok;
        do {
            point = nextPoint();
            ok = true;
            forv(i, points) {
                forn(j, i) {
                    forn(k, j) {
                        Plane plane(points[i], points[j], points[k]);
                        if (plane.contains(point)) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) break;
                }
                if (!ok) break;
            }
        } while (!ok);
        points.push_back(point);
    }
    cout << points.size() << endl;
    forv(i, points) {
        cout << points[i] << endl;
    }
    exit(0);
}

void test11()
{
    Points points;
    points.push_back(Point(-20, 0, 0));
    points.push_back(Point(0, -20, 0));
    points.push_back(Point(100, 100, 0));
    points.push_back(Point(0, 0, 0));
    points.push_back(Point(0, 0, 500));
    points.push_back(Point(1, 1, -500));
    points.push_back(Point(5, 5, 2));
    print(points);
    exit(0);        
}

void test12() 
{
    Points points;
    points.push_back(Point(0, 0, 0));
    points.push_back(Point(500, 0, 0));
    points.push_back(Point(0, 500, 0));
    points.push_back(Point(0, 0, 500));
    points.push_back(Point(499, 499, 499));
    points.push_back(Point(200, 200, -10));
    points.push_back(Point(200, -10, 200));
    points.push_back(Point(-10, 200, 200));
    points.push_back(Point(10, 11, 12));
    points.push_back(Point(2, 3, 20));
    print(points);
    exit(0);
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
    if (test == 4) test4();
    if (test <= 10) randomSmall();
    if (test == 11) test11();
    if (test == 12) test12();
    assert(false);

    return 0;
}
