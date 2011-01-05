#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>

using namespace std;
#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())

namespace constants
{
    const double EPS = 1e-7;
}

class Point
{
public:
    Point() : x_(0), y_(0), z_(0) {}

    Point(double x, double y, double z) :
        x_(x), y_(y), z_(z) {}
    
    double x() const { return x_; }

    double y() const { return y_; }

    double z() const { return z_; }

    double length() const 
    {
        return sqrt(x() * x() + y() * y() + z() * z());
    }

    double squaredLength() const
    {
        return x() * x() + y() * y() + z() * z();
    }

    size_t id() const { return id_; }

    friend istream& operator>>(istream& in, Point& point); 
private:
    double x_;
    double y_;
    double z_;
    size_t id_;
};

Point operator-(const Point& pointA, const Point& pointB)
{
    return Point(pointA.x() - pointB.x(), pointA.y() - pointB.y(),
            pointA.z() - pointB.z());
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

typedef vector<Point> Points;

double det(double a, double b, double c, double d) 
{
    return a * d - b * c;
}

class Plane
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
        return fabs(signedDistance(point)) < constants::EPS;    
    }

    int sign(const Point& point) 
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

istream& operator>>(istream& in, Point& point)
{
    in >> point.x_ >> point.y_ >> point.z_;
    return in;    
} 

void test() 
{
    {
        Point A(0, 1, 2), B(3, 323, 234), C(32.23, 232, -234);
        Plane plane(A, B, C);
        assert(plane.contains(A));
        assert(plane.contains(B));
        assert(plane.contains(C));
    }
    {
        Point A(0, 0, 0), B(1, 0, 0), C(0, 0, 1);
        Plane plane(A, B, C);
        assert(plane.contains(A));
        assert(plane.contains(B));
        assert(plane.contains(C));
        forn(i, 100) {
            assert(plane.contains(Point(2*i, 0, i)));
        }
    }
    cout << "Tests are passed successfully" << endl;
}

class Facet
{
public:
    typedef vector<size_t> Indices;

    Facet() {}

    Facet(const Indices& indices)
    {
        vertices_ = indices;
    }

    size_t operator[](size_t index) const
    {
        assert(index < size());
        return vertices_[index];
    }

    void add(size_t vertexIndex) 
    {
        vertices_.push_back(vertexIndex);
    }

    size_t size() const
    {
        return vertices_.size();
    }

    void sort() 
    {
        std::sort(vertices_.begin(), vertices_.end());
    }

    friend ostream& operator<<(ostream& out, const Facet& facet);
     
private:
    Indices vertices_;                        
};

typedef vector<Facet> Facets;

ostream& operator<<(ostream& out, const Facet& facet)
{
    out << facet.size();
    forn(i, facet.size()) {
        out << " " << facet[i];
    }
    return out;
}

bool operator<(const Facet& facetA, const Facet& facetB)
{
    if (facetA.size() != facetB.size()) {
        return facetA.size() < facetB.size();
    }        
    forn(i, facetA.size()) {
        if (facetA[i] != facetB[i]) {
            return facetA[i] < facetB[i];
        }
    }
}


Facet makeFacet(size_t a, size_t b, size_t c) 
{
    Facet facet;
    facet.add(a);
    facet.add(b);
    facet.add(c);
    facet.sort();
    return facet;
}

class Edge
{
public:
    Edge(size_t from, size_t to) : from_(from), to_(to) {}
    size_t from() const { return from_; }
    size_t to() const { return to_; }
private:
    size_t from_;
    size_t to_;
};

bool operator<(const Edge& edgeA, const Edge& edgeB) 
{
    return edgeA.from() < edgeB.from() || 
        (edgeA.from() == edgeB.from() &&
         edgeA.to() < edgeB.to());
}

typedef set<Edge> Edges;

class ConvexFigure 
{
public:
    ConvexFigure() {}
    virtual ~ConvexFigure() = 0;
    void addEdge(size_t from, size_t to)
    {
        edges_.insert(Edge(from, to));
    }
    void addEdge(const Edge& edge)
    {
        edges_.insert(edge);
    } 
private:
    Edges edges_;            
};

class Polyhedron: public ConvexFigure
{
public:
    Polyhedron() {}
    virtual ~Polyhedron() {}
private:
};

class Polygon 
{
public:     
    Polygon() {}
    virtual ~Polygon() {}
private:
};

double det2(const Point& pointA, const Point& pointB)
{
    return pointA.x() * pointB.y() - pointA.y() * pointB.x();
}

class PointsComparator
{
public:
    PointsComparator(const Point& point) : origin_(point) {}

    bool operator() (const Point& pointA, const Point& pointB) const
    {
        double det = det2(pointA - origin_, pointB - origin_);
        if (fabs(det) < constants::EPS) {

        } else {
            return det > 0;
        }            
    }
private:
    Point origin_;
};

void convexHullSimple(Points points, Polyhedron* polyhedron, Polygon* polygon)
{
    
}

void convexHull2(Points points, Polygon* polygon)
{
    assert(points.size() >= 3);
    Points::iterator originIterator = 
        min_element(points.begin(), points.end());
    Point origin = *originIterator;
    Points::iterator lastPointIterator = 
        points.end()-1;
    swap(*lastPointIterator, *originIterator);

    PointsComparator comparator(origin); 

    forv(i, points) {
            
    }
}

void convexHull(const Points& points, Polyhedron* polyhedron, Polygon* polygon) 
{
    if (points.size() <= 7) {
                
    }
}


void solve() 
{
    int numPoints;
    cin >> numPoints;
    Points points;
    forn(i, numPoints) {
        Point point;
        cin >> point;
        points.push_back(point);      
    }       

    Facets answer;
    forn(i, points.size()) {
        forn(j, i) {
            forn(k, j) {
                Plane plane(points[i], points[j], points[k]);
                size_t numAbove = 0, numBelow = 0;
                forn(t, points.size()) {
                    int sn = plane.sign(points[t]);
                    if (sn < 0) ++numBelow;
                    else if (sn > 0) ++numAbove;
                }            
                if (!numAbove) {    
                    answer.push_back(makeFacet(i, j, k));
                }
                if (!numBelow) {
                    answer.push_back(makeFacet(j, i, k));
                }
            }
        }
    }

    sort(answer.begin(), answer.end());
    cout << answer.size() << endl;
    forn(i, answer.size()) {
        cout << answer[i] << endl;        
    }
}

int main() 
{
    test();
    solve();
    return 0;
}
