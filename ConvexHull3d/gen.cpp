#include "testlib.h"

#include <iostream>

using namespace std;

#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())

struct Point
{
    long long x, y, z;
    Point() {}
    Point(long long x_, long long y_, long long z_) : x(x_), y(y_), z(z_) {}
};

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

typedef std::vector<Point> Points;

long long det(long long x, long long y, long long z, long long w)
{
    return x * w - y * z;
}

struct Plane
{
    Plane(const Point p1, const Point p2, const Point p3)
    {
        a = det(p2.y-p1.y, p2.z-p1.z, p3.y-p1.y, p3.z-p1.z);
        b = -det(p2.x-p1.x, p2.z-p1.z, p3.x-p1.x, p3.z-p1.z);
        c = det(p2.x-p1.x, p2.y-p1.y, p3.x-p1.x, p3.y-p1.y);
        d = -a * p1.x - b * p1.y - c * p1.z;
    }
    bool contains(const Point& point) const
    {
        return a * point.x + b * point.y + c * point.z + d == 0;
    }
    long long a, b, c, d;
};

ostream& operator<<(ostream& out, const Point& p)
{
    out << p.x << " " << p.y << " " << p.z;
    return out;
}

Point ortVector(const Point& a, const Point&b, const Point& c)
{
    Plane plane(a, b, c);
    return Point(plane.a, plane.b, plane.c);
}

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

Point nextPoint(int minc = -500, int maxc = 500)
{
    return Point(rnd.next(minc, maxc), rnd.next(minc, maxc), rnd.next(minc, maxc));
}

//no four points on the same plane
bool check(const Points& points) 
{
    forv(i, points) {
        forn(j, i) {
            forn(k, j) {
                Plane plane(points[i], points[j], points[k]);
                forv(t, points) {
                    if (t == i || t == j || t == k) continue;
                    if (plane.contains(points[t])) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

//checks if the last points is not contained in any plane build on three points 
bool checkLast(const Points& points) 
{
    forv(i, points) {
        if (i + 1 == points.size()) break;
        forn(j, i) {
            forn(k, j) {
                Plane plane(points[i], points[j], points[k]);
                if (plane.contains(points.back())) {
                    return false;
                }
            }
        }
    }
    return true;
}

void random(int from, int to, int minc = -500, int maxc = 500)
{
    int n = rnd.next(from, to);
    Point first = nextPoint(minc, maxc);
    Point second = nextPoint(minc, maxc);
    while (second == first) {   
        second = nextPoint(minc, maxc);
    }
    Point third = nextPoint(minc, maxc);
    Point zero(0, 0, 0);
    while (ortVector(first, second, third) == zero) {
        third = nextPoint(minc, maxc);
    }

    Points points;
    points.push_back(first);
    points.push_back(second);
    points.push_back(third);

    forn(iter, n-3) {
        Point point;
        bool ok;
        do {
            point = nextPoint(minc, maxc);
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
    points.push_back(Point(0, 0, -10));
    points.push_back(Point(0, 0, 467));
    points.push_back(Point(1, 2, -500));
    points.push_back(Point(5, 7, 2));
    print(points);
    exit(0);        
}

void test12() 
{
    Points points;
    points.push_back(Point(0, 0, 0));
    points.push_back(Point(500, 0, 0));
    points.push_back(Point(0, 499, 0));
    points.push_back(Point(0, 0, 498));
    points.push_back(Point(480, 490, 499));
    points.push_back(Point(200, 200, -10));
    points.push_back(Point(200, -10, 200));
    points.push_back(Point(-10, 200, 200));
    points.push_back(Point(10, 11, 12));
    points.push_back(Point(2, 3, 20));
    print(points);
    exit(0);
}

void test18() 
{
    //big tetrahedron
    Points points;
    points.push_back(Point(-500, -500, -500));
    points.push_back(Point(500, -500, -500));
    points.push_back(Point(0, 500, -500));
    points.push_back(Point(0, 0, 500));
    print(points);
    exit(0);
}

void test19()
{
    //tetrahedron with many randomly generated points inside
    Points points;
    points.push_back(Point(-500, -500, -500));
    points.push_back(Point(500, -500, -500));
    points.push_back(Point(0, 500, -500));
    points.push_back(Point(0, 0, 500));
    assert(check(points));
    forn(i, 50) {
        while (true) {
            points.push_back(nextPoint(-70, 70));
            if (checkLast(points)) {
                break;
            }
            points.pop_back();
        }
    }
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
    if (test <= 10) random(6, 20);
    if (test == 11) test11();
    if (test == 12) test12();
    if (test <= 17) random(21, 40);
    if (test == 18) test18();
    if (test == 19) test19();
    if (test <= 40) random(8, 70, -30, 30);
    assert(false);

    return 0;
}
