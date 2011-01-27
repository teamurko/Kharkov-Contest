#include "testlib.h"

#include <iostream>

using namespace std;

#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())

typedef long long Long;

struct Point
{
    Long x, y, z;
    Point() {}
    Point(Long x_, Long y_, Long z_) : x(x_), y(y_), z(z_) {}
    Long squaredLength() const
    {
        return x * x +  y * y + z * z;        
    }
};

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

typedef std::vector<Point> Points;

Long det(Long x, Long y, Long z, Long w)
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
    Long a, b, c, d;
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

void print(Long r, const Points& points)
{
    cout << r << " " << points.size() << endl;
    forv(i, points) {
        cout << points[i] << endl;
    }
}

void test1()
{
    Points points;
    points.push_back(Point(3, 4, 5));
    print(1, points);
    exit(0);
}

void test2()
{
    Points points;
    points.push_back(Point(1, 0, 0));
    print(13, points);
    exit(0);
}

void test3()
{
    Points points;
    points.push_back(Point(0, 0, 1));
    points.push_back(Point(0, 1, 0));
    print(10, points);
    exit(0);
}

void test4()
{
    Points points;
    points.push_back(Point(1, 0, 0));
    points.push_back(Point(0, 1, 0));
    points.push_back(Point(1, 1, 0));
    points.push_back(Point(100, 2, 0));
    points.push_back(Point(23, -23, 4));
    print(10, points);
    exit(0);
}

Point nextPoint(int minc = -100, int maxc = 100)
{
    return Point(rnd.next(minc, maxc), rnd.next(minc, maxc), rnd.next(minc, maxc));
}

Long scalarProduct(const Point& a, const Point& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Long sqr(Long x)
{
    return x * x;
}

bool colinear(const Point& a, const Point& b) 
{
    return sqr(scalarProduct(a, b)) == a.squaredLength() * b.squaredLength();
}

//no colinear vectors
bool check(const Points& points) 
{
    forv(i, points) {
        forn(j, i) {
            if (colinear(points[i], points[j])) return false;
        }
    }
    return true;
}

//checks if the last point is not colinear with any other added points 
bool checkLast(const Points& points, const Point& point) 
{
    forv(i, points) {
        if (colinear(points[i], point)) return false;
    }
    return true;
}

void generateRandomPoints(int numPoints, int minc, int maxc, Points* points)
{
    points->push_back(nextPoint(minc, maxc));
    forn(iter, numPoints-1)
    {
        Point addingPoint = nextPoint(minc, maxc);
        while (!checkLast(*points, addingPoint)) {
            addingPoint = nextPoint(minc, maxc);
        }            
        points->push_back(addingPoint);
    }
}

void randomTest(int numPoints, int minc, int maxc)
{
    Points points;
    generateRandomPoints(numPoints, minc, maxc, &points);
    print(100, points);
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
    if (test <= 7) randomTest(10, -10, 10);
    if (test <= 10) randomTest(100, -100, 100);
    if (test <= 15) randomTest(200, -100, 100);
    if (test <= 20) randomTest(300, -100, 100);
    assert(false);

    return 0;
}
