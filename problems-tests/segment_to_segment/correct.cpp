#include <iostream>
#include <cmath>
using namespace std;

class Point
{
public:
    Point() {}
    Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}  
    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }
    friend istream& operator>>(istream& in, Point& point);
    double dist() const
    {
        return sqrt(x() * x() + y() * y() + z() * z());
    }
private:
    double x_;
    double y_;
    double z_;    
};

istream& operator>>(istream& in, Point& point)
{
    in >> point.x_ >> point.y_ >> point.z_;
    return in;
}


Point operator+(const Point& a, const Point& b) 
{
    return Point(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

Point operator-(const Point& a, const Point& b) 
{
    return Point(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

Point operator*(const Point& a, double coef)
{
    return Point(a.x() * coef, a.y() * coef, a.z() * coef);
}

Point operator*(double coef, const Point& a)
{
    return Point(a.x() * coef, a.y() * coef, a.z() * coef);
}

double scalarProduct(const Point& a, const Point& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

double det(double a, double b, double c, double d)
{
    return a * d - b * c;
}

double dist(const Point& a, const Point& b)
{
    return (a-b).dist();
}

double distPointSegment(const Point& p, const Point& a, const Point& b)
{
    //(a + (b - a) * t - p, (b-a)) = 0
    Point v(b-a);
    double A = scalarProduct(v, v);
    if (A > 1e-7) {
        double t = scalarProduct(p-a,v) / A;
        t = min(1.0, t);
        t = max(0.0, t);
        return dist(p, a + v * t);
    } 
    else {
        return dist(p, a);
    }
}

int main()
{
    Point p11, p12, p21, p22;
    cin >> p11 >> p12 >> p21 >> p22;
    Point v1(p12 - p11), v2(p22 - p21);
    //(p11 + v1 * t1  - p21 - v2 * t2, v1) = 0
    //(p11 + v1 * t1 - p21 - v2 * t2, v2) = 0
    //v1*v1*t1 - v1*v2*t2 = (p21-p11)*v1
    //v1*v2*t1 - v2*v2*t2 = (p21-p11)*v2 

    double ans = 1e100;

    double a11 = scalarProduct(v1, v1);
    double a12 = -scalarProduct(v1, v2);
    double c1 = scalarProduct(p21-p11, v1);
    double a21 = scalarProduct(v1, v2);
    double a22 = -scalarProduct(v2, v2);
    double c2 = scalarProduct(p21-p11, v2);
    double d = det(a11, a12, a21, a22);
    if (fabs(d) > 1e-7) {   
        //v1 || v2
        double t1 = det(c1, a12, c2, a22) / d;
        double t2 = det(a11, c1, a21, c2) / d;
        t1 = min(1.0, t1);
        t1 = max(0.0, t1);
        t2 = min(1.0, t2);
        t2 = max(0.0, t2);
        ans = min(ans, dist(p11 + v1 * t1, p21 + v2 * t2));
    } 
    ans = min(ans, distPointSegment(p11, p21, p22));
    ans = min(ans, distPointSegment(p12, p21, p22));
    ans = min(ans, distPointSegment(p21, p11, p12));
    ans = min(ans, distPointSegment(p22, p11, p12));
    cout.precision(10);
    cout << fixed << ans << endl;
}
