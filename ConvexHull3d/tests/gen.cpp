#include <iostream>

using namespace std;

void test1()
{
    cout << 4 << endl;
    cout << "0 0 0" << endl;
    cout << "1 0 0" << endl;
    cout << "0 1 0" << endl;
    cout << "0 0 1" << endl;
    exit(0);
}

void test2()
{
    cout << 4 << endl;
    cout << "0 0 0" << endl;
    cout << "10 0 0" << endl;
    cout << "0 10 0" << endl;
    cout << "10 10 10" << endl;
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
    cout << 5 << endl;
    cout << "0 0 0" << endl;
    cout << "10 0 0" << endl;
    cout << "0 10 0" << endl;
    cout << "10 10 10" << endl;
    cout << "5 5 10" << endl;
    exit(0);

}

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
    double signedDistance(const Point& point) 
    {
        return a() * point.x() + b() * point.y() + c() * point.z() + d();
    }
    bool contains(const Point& point) 
    {
        return fabs(signedDistance(point)) < EPS;    
    }
    int sign(const Point& point) 
    {
        double distance = signedDistance(point);
        if (distance < -EPS) {
            return -1;
        }
        if (distance > EPS) {
            return 1;
        }
        return 0;
    }
private:
    double a_;
    double b_;
    double c_;
    double d_;
    static double EPS;
};

double Plane::EPS = 1e-7;

void randomSmall()
{
    int n = rnd.next(6, 21);
        
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
    if (test <= 10) randomSmall();
    assert(false);

    return 0;
}
