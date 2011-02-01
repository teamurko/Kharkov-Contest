#include "testlib.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>

struct Point
{
    long long x, y, z;
};

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
    double a, b, c, d;
};

int main(int argc, char* argv[]) {
    registerValidation();
    int n = inf.readInt(1, 10000, "Number points");
    inf.readEoln();
    std::vector<Point> points;
    for(size_t i = 0; i < n; ++i) {
        Point point;
        point.x = inf.readInt(-500, 500, "x coordinate");
        inf.readSpace();
        point.y = inf.readInt(-500, 500, "y coordinate");
        inf.readSpace();
        point.z = inf.readInt(-500, 500, "z coordinate");
        points.push_back(point);
        inf.readEoln();
    }
    inf.readEof();

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < i; ++j) {
            for(int k = 0; k < j; ++k) {
                const Plane plane(points[i], points[j], points[k]);
                for(int t = 0; t < n; ++t) {
                    if (t == i || t == j || t == k) continue;
                    if (plane.contains(points[t])) {
                        std::stringstream message;
                        message << "Four points on the same plane : " << k << " " << j << " " << i << " " << t;
                        quitf(_fail, message.str().c_str()); 
                    }
                }
            }
        }
    }

    
    return 0;
}
