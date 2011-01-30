#include "testlib.h"

#include <iostream>
#include <cassert>

using namespace std;

#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())
#define all(v) v.begin(), v.end()

typedef long long Long;

struct Point
{
    Long x_, y_, z_;
    Point() {}
    Point(Long x, Long y, Long z) : x_(x), y_(y), z_(z) {}
    void setX(Long x) { x_ = x; }
    void setY(Long y) { y_ = y; }
    void setZ(Long z) { z_ = z; }
    Long x() const { return x_; }
    Long y() const { return y_; }
    Long z() const { return z_; }
    Long squaredLength() const
    {
        return x_ * x_ +  y_ * y_ + z_ * z_;        
    }
};

bool operator==(const Point& a, const Point& b)
{
    return a.x() == b.x() && a.y() == b.y() && a.z() == b.z();
}

bool operator<(const Point& a, const Point& b)
{
    if (a.x() != b.x()) return a.x() < b.x();
    if (a.y() != b.y()) return a.y() < b.y();
    return a.z() < b.z();
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
        a_ = det(p2.y()-p1.y(), p2.z()-p1.z(), p3.y()-p1.y(), p3.z()-p1.z());
        b_ = -det(p2.x()-p1.x(), p2.z()-p1.z(), p3.x()-p1.x(), p3.z()-p1.z());
        c_ = det(p2.x()-p1.x(), p2.y()-p1.y(), p3.x()-p1.x(), p3.y()-p1.y());
        d_ = -a_ * p1.x() - b_ * p1.y() - c_ * p1.z();
    }
    bool contains(const Point& point) const
    {
        return a_ * point.x() + b_ * point.y() + c_ * point.z() + d_ == 0;
    }
    Long d() const { return d_; }
    Long a() const { return a_; }
    Long b() const { return b_; }
    Long c() const { return c_; }
    Long a_, b_, c_, d_;
};

ostream& operator<<(ostream& out, const Point& p)
{
    out << p.x() << " " << p.y() << " " << p.z();
    return out;
}

Point ortVector(const Point& a, const Point&b, const Point& c)
{
    Plane plane(a, b, c);
    return Point(plane.a(), plane.b(), plane.c());
}

void print(Long r, const Points& points)
{
    cout << r << " " << points.size() << endl;
    forv(i, points) {
        cout << points[i] << endl;
    }
}

Point nextPoint(int minc = -100, int maxc = 100)
{
    return Point(rnd.next(minc, maxc), rnd.next(minc, maxc), rnd.next(minc, maxc));
}

Long scalarProduct(const Point& a, const Point& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
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

void orientate(Point& p)
{
    if (p.x() < 0 || (p.x() == 0 && p.y() < 0) ||
        (p.x() == 0 && p.y() == 0 && p.z() < 0)) {
            p.setX(-p.x());
            p.setY(-p.y());
            p.setZ(-p.z());            
        }    
}

//checks if the last point is not colinear with any other added points 
bool checkLast(const Points& points, const Point& point) 
{
    forv(i, points) {
        if (colinear(points[i], point)) return false;
    }
    return true;
}

//as planes passing through O
void sortUnique(Points* points)
{
    forn(i, points->size()) orientate((*points)[i]);
    sort(points->begin(), points->end());
    points->erase(unique(all((*points))), points->end());       
}

bool checkLastDiffPlanes(const Points& points, const Point& adding)
{
    forv(i, points) {
        if (points[i] == adding) return false;
    }
    if (adding == Point(0, 0, 0)) return false;
    forv(i, points)
    {
        forn(j, i) {
            const Plane plane(points[i], points[j], adding);
            if (plane.d() == 0) return false;
        }
    } 
    return true;
}

void generateRandomPointsDiffPlanes(int numPoints, int minc, int maxc, Points* points)
{
    points->push_back(nextPoint(minc, maxc));
    while (points->back() == Point(0, 0, 0)) 
        points->back() = nextPoint(minc, maxc);
    forn(iter, numPoints-1)
    {
        Point addingPoint = nextPoint(minc, maxc);
        while (!checkLastDiffPlanes(*points, addingPoint)) {
            addingPoint = nextPoint(minc, maxc);
        }            
        points->push_back(addingPoint);
    }
}

Long gcd(Long a, Long b)
{
    if (a == 0) return b;
    return gcd(b % a, a);
}

Point norm(const Point& p)
{
    Long d = gcd(p.x(), gcd(p.y(), p.z()));
    return Point(p.x()/d, p.y()/d, p.z()/d);
}

Point vectorProduct(const Point& a, const Point& b)
{
    return Point(det(a.y(), a.z(), b.y(), b.z()), 
            det(a.z(), a.x(), b.z(), b.x()),
            det(a.x(), a.y(), b.x(), b.y()));
}

void randomDenseTest(int numPoints, int minc = -100, int maxc = 100)
{
    Points points;
    generateRandomPointsDiffPlanes(numPoints, -10, 10, &points);
    Points planes;
    forv(i, points) {
        forn(j, i)
        {   
            planes.push_back(vectorProduct(points[i], points[j]));
        }
    }
    sortUnique(&planes);
    print(100, planes);
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
    if (test <= 3) randomDenseTest(20);
    if (test <= 6) randomDenseTest(30);
    assert(false);

    return 0;
}
