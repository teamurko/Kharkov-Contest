#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

class Line
{
public:
    Line(const Point& pointA, const Point& pointB) :
        a_(pointB.x() - pointA.x()), b_(pointA.y() - pointB.y()),
        c_(-a() * pointA.x() - b() * pointA.y()) {}

    double a() const { return a_; }

    double b() const { return b_; }

    double c() const { return c_; }

    double signedDistance(const Point& point) const 
    {
        return a() * point.x() + b() * point.y() + c();
    }

private:
    double a_;
    double b_;
    double c_;
};


#endif //LINE_H_INCLUDED