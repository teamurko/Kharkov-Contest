#include "testlib.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>

#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())

using namespace std;

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

typedef std::vector<Point> Points;

Long det(Long x, Long y, Long z, Long w)
{
    return x * w - y * z;
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

int main(int argc, char* argv[]) {
    registerValidation();
    inf.readInt(1, 100, "Radius of an orange");
    inf.readSpace();
    int n = inf.readInt(1, 1000, "Number planes");
    inf.readEoln();
    std::vector<Point> points;
    for(size_t i = 0; i < n; ++i) {
        Point point;
        point.x = inf.readInt(-100, 100, "x coordinate");
        inf.readSpace();
        point.y = inf.readInt(-100, 100, "y coordinate");
        inf.readSpace();
        point.z = inf.readInt(-100, 100, "z coordinate");
        points.push_back(point);
        inf.readEoln();
    }
    inf.readEof();

    if (!check(points)) {
        std::stringstream message;
        message << "There are equal planes";
        quitf(_fail, message.str().c_str()); 
    }

    return 0;
}
