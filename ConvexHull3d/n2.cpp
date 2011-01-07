#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <limits>
#include <queue>
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

typedef vector<Point> Points;

double det(double a, double b, double c, double d) 
{
    return a * d - b * c;
}

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

    void order() 
    {
        size_t i = static_cast<size_t>(min_element(vertices_.begin(), vertices_.end()) - vertices_.begin());
        rotate(vertices_.begin(), vertices_.begin() + i, vertices_.end());
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
    facet.order();
    return facet;
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

Facet findInitialFacet(const Points& points) 
{
    size_t a = 0;
    forv(i, points) {
        if (points[i].x() < points[a].x()) {
            a = i;
        }
    }
    Point ortNormal(-1, 0, 0);

    size_t b = 0, c = 0;
    double bestCosine = -1;

    forv(j, points) {
        if (j == a) continue;
        forv(i, points) {
            if (i == a) continue;
            const Point ort2 = ortVector(points[a], points[i], points[j]);
            const Point ort2Normal = ort2 / ort2.length();

            double cosine = scalarProduct(ort2Normal, ortNormal); 
            if (fabs(cosine) > bestCosine) {
                bestCosine = fabs(cosine);
                b = i;
                c = j;
                if (cosine < 0) swap(b, c);
            }
        }
    }

    return makeFacet(a, b, c);
}

void convexHullWrapping(const Points& points, Facets* facets)
{
    //facets are ordered counter-clockwise, first index is the least
    Facet initialFacet = findInitialFacet(points);
    facets->push_back(initialFacet);
    set<Facet> used;
    used.insert(initialFacet);

    queue<Facet> queueFacets;
    
    queueFacets.push(initialFacet);

    while (!queueFacets.empty()) {
        Facet facet = queueFacets.front(); 
        queueFacets.pop();
        Point ort = ortVector(points[facet[0]], points[facet[1]], points[facet[2]]);
        Point ortNormal = ort / ort.length();
        forn(i, facet.size()) {
            Edge edge(facet[(i + 1) % facet.size()], facet[i]);
            size_t bestIndex = numeric_limits<size_t>::max();
            double bestCosine = -1;
            forv(j, points)
            {
                Point ort2 = ortVector(points[edge.from()], points[edge.to()], points[j]);
                Point ort2Normal = ort2 / ort2.length();
                double cosine = scalarProduct(ort2Normal, ortNormal);
                if (bestCosine < cosine) {
                    bestCosine = cosine;
                    bestIndex = j;
                }                                        
            }
            assert(bestIndex != numeric_limits<size_t>::max());
            Facet adjFacet = makeFacet(edge.from(), edge.to(), bestIndex);
            if (!used.count(adjFacet)) {
                used.insert(adjFacet);
                queueFacets.push(adjFacet);
                facets->push_back(adjFacet);
            }
        }
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
    convexHullWrapping(points, &answer);
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
