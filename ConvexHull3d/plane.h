#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "point.h"
#include "utils.h"

#include <iostream>

struct Plane
{
public:
    Plane(double a, double b, double c, double d) :
        a_(a), b_(b), c_(c), d_(d) {}
    Plane(const Point& pointA, const Point& pointB, const Point& pointC) 
    {  
        a_ = det(pointB.y() - pointA.y(), pointB.z() - pointA.z(),
                pointC.y() - pointA.y(), pointC.z() - pointA.z());
        b_ = -det(pointB.x() - pointA.x(), pointB.z() - pointA.z(),
                pointC.x() - pointA.x(), pointC.z() - pointA.z());
        c_ = det(pointB.x() - pointA.x(), pointB.y() - pointA.y(),
                pointC.x() - pointA.x(), pointC.y() - pointA.y());
        d_ = -a() * pointA.x() - b() * pointA.y() - c() * pointA.z();
    }
    double a() const { return a_; }
    double b() const { return b_; }
    double c() const { return c_; }
    double d() const { return d_; }
    double signedDistance(const Point& point) const
    {
        return a() * point.x() + b() * point.y() + c() * point.z() + d();
    }
    bool contains(const Point& point) const
    {
        return fabs(signedDistance(point)) < constants::EPS;    
    }
    Point ort() const 
    {
        return Point(a(), b(), c());
    }
    int sign(const Point& point) const
    {
        double distance = signedDistance(point);
        if (distance < -constants::EPS) {
            return -1;
        }
        if (distance > constants::EPS) {
            return 1;
        }
        return 0;
    }
private:
    double a_;
    double b_;
    double c_;
    double d_;
};

std::istream& operator>>(std::istream& in, Point& point)
{
    in >> point.x_ >> point.y_ >> point.z_;
    return in;    
} 

#endif //PLANE_H_INCLUDED
