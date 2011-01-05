#include <iostream>

using namespace std;
#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)

struct Point
{
public:
    Point(double x, double y, double z) :
        x_(x), y_(y), z_(z) {}
    double x() const {
        return x_;
    }
    double y() const {
        return y_;
    }
    double z() const {
        return z_;
    }
private:
    double x_;
    double y_;
    double z_;
};

struct Plane
{
public:
    Plane(double a, double b, double c, double d) :
        a_(a), b_(b), c_(c), d_(d) {}
    Plane(const Point& pointA, const Point& pointB, const Point& pointC) {  
        //TODO    
    }
    double a() const { return a_; }
    double b() const { return b_; }
    double c() const { return c_; }
    double d() const { return d_; }
    double signedDistance(const Point& point) {
        return a() * point.x() + b() * point.y() + c() * point.z() + d();
    }
private:
    double a_;
    double b_;
    double c_;
    double d_;
}

int main() 
{
    return 0;
}