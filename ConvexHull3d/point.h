#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "utils.h"
#include <vector>
#include <cmath>
#include <iostream>

double det(double a, double b, double c, double d) 
{
    return a * d - b * c;
}

class Point
{
public:
    Point() : x_(0), y_(0), z_(0), id_(0) {}

    Point(double x, double y, double z) :
        x_(x), y_(y), z_(z), id_(0) {}
    
    double x() const { return x_; }

    double y() const { return y_; }

    double z() const { return z_; }

    double length() const 
    {
        return std::sqrt(x() * x() + y() * y() + z() * z());
    }

    double squaredLength() const
    {
        return x() * x() + y() * y() + z() * z();
    }

    size_t id() const { return id_; }

    void setId(size_t id) { id_ = id; }

    friend std::istream& operator>>(std::istream& in, Point& point); 
    friend std::ostream& operator<<(std::ostream& out, const Point& point);
private:
    double x_;
    double y_;
    double z_;
    size_t id_;
};

std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out.setf(std::ios::fixed, std::ios::floatfield);  
    out.precision(10);
    out << point.x() << " " << point.y() << " " << point.z();
}

Point operator-(const Point& pointA, const Point& pointB)
{
    return Point(pointA.x() - pointB.x(), pointA.y() - pointB.y(),
            pointA.z() - pointB.z());
}

Point operator+(const Point& pointA, const Point& pointB)
{
    return Point(pointA.x() + pointB.x(), pointA.y() + pointB.y(),
            pointA.z() + pointB.z());
}

Point operator/(const Point& point, double divisor)
{
    return Point(point.x() / divisor, point.y() / divisor, point.z() / divisor);
}

bool operator<(const Point& pointA, const Point& pointB) 
{
    if (pointA.x() != pointB.x()) {
        return pointA.x() < pointB.x();
    }
    if (pointA.y() != pointB.y()) {
        return pointA.y() < pointB.y();
    }
    return pointA.z() < pointB.z();
}

bool operator==(const Point& pointA, const Point& pointB)
{
    return pointA.id() == pointB.id() &&
        fabs(pointA.x() - pointB.x()) < constants::EPS &&
        fabs(pointA.y() - pointB.y()) < constants::EPS &&
        fabs(pointA.z() - pointB.z()) < constants::EPS;
}

Point vectorProduct(const Point& a, const Point& b)
{
    return Point(det(a.y(), a.z(), b.y(), b.z()), 
            det(a.z(), a.x(), b.z(), b.x()),
            det(a.x(), a.y(), b.x(), b.y()));
}

Point ortVector(const Point& a, const Point& b, const Point& c)
{
    return vectorProduct(b - a, c - a);
}

double scalarProduct(const Point& a, const Point& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}


typedef std::vector<Point> Points;

#endif //POINT_H_INCLUDED
