#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())
#define all(v) v.begin(), v.end()


typedef long long Long;

Long det(Long a, Long b, Long c, Long d) 
{
    return a * d - b * c;
}

class Point
{
public:
    Point() : x_(0), y_(0), z_(0) {}

    Point(Long x, Long y, Long z) :
        x_(x), y_(y), z_(z) {}
    
    Long x() const { return x_; }

    Long y() const { return y_; }

    Long z() const { return z_; }

    Long squaredLength() const
    {
        return x() * x() + y() * y() + z() * z();
    }

    Point normal() const 
    {   
        Long len = gcd(abs(x()), gcd(abs(y), abs(z)));
        return Point(x_/=len, y_/=len, z_/=len);
    }

    friend std::istream& operator>>(std::istream& in, Point& point); 
private:
    Long x_;
    Long y_;
    Long z_;
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

Point operator-(const Point& point)
{
    return Point(-point.x(), -point.y(), -point.z());
}

std::istream& operator>>(std::istream& in, Point& point)
{
    in >> point.x_ >> point.y_ >> point.z_;
    return in;    
} 

std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out.setf(std::ios::fixed, std::ios::floatfield);  
    out.precision(10);
    out << point.x() << " " << point.y() << " " << point.z();
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

Long scalarProduct(const Point& a, const Point& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

int sign(Long num)
{
    if (num > 0) return 1;
    if (num == 0) return 0;
    return -1;
}

int signScalarProduct(const Point& a, const Point& b)
{
    return sign(scalarProduct(a, b));
}


typedef std::vector<Point> Points;

struct Plane
{
public:
    Plane(Long a, Long b, Long c, Long d) :
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
    Long a() const { return a_; }
    Long b() const { return b_; }
    Long c() const { return c_; }
    Long d() const { return d_; }
    Long signedDistance(const Point& point) const
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
        Long distance = signedDistance(point);
        if (distance < 0) {
            return -1;
        }
        if (distance > 0) {
            return 1;
        }
        return 0;
    }
private:
    Long a_;
    Long b_;
    Long c_;
    Long d_;
};

typedef vector<Plane> Planes;
typedef size_t Id;
typedef vector<Id> Ids;
typedef vector<Ids> Graph;

bool Cmp(const pair<Id, pair<Long, Long> >& one, const pair<Id, pair<Long, Long> >& two) 
{
    Long _1 = one.second.first * two.second.second;
    Long _2 = one.second.second * two.second.first;
    return _1 < _2;
}


int main()
{
    Long R;
    size_t numPlanes;
    cin >> R >> numPlanes;
    vector<Plane> planes(numPlanes);
    forn(i, numPlanes) {
        Point point;
        cin >> point;
        planes[i] = Plane(point.x(), point.y(), point.z(), 0);
    }

    vector<Points> intersections(numPlanes);

    Points points;
    forn(i, numPlanes) {
        Points& pts = intersections[i];
        forn(j, numPlanes) {
            if (i == j) continue;
            Point p = vectorProduct(planes[i].ort(), planes[j].ort()).normal();
            pts.push_back(p);
            pts.push_back(-p);
            points.push_back(p);
            points.push_back(pts.back());
        }
    }

    sort(all(points));
    points.erase(unique(all(points)), points.end());

    
    Graph graph(points.size());

    forn(i, numPlanes) {
        const Points& pts = intersections[i];
        forv(j, pts) {
            graph[i].push_back(static_cast<size_t>(lower_bound(all(points), pts[j]) - points.begin()));
        }            
        sort(all(graph[i]));
        graph[i].erase(unique(all(graph[i])), graph[i].end());
    }

    //orientate adjacent vertices

    forv(v, graph) {
        const Ids& adj = graph[v];
        pair<Id, pair<Long, Long> > left, right;
        forv(i, adj) {
            if (i == 0) continue;
            int s = signScalarProduct(points[v], vectorProduct(points[adj[0]], points[adj[i]]));
            assert(s != 0);
            pair<Long, Long> frac;
            frac.first = scalarProduct(points[adj[i]], points[adj[0]]);
            frac.second = points[adj[i]].squaredLength();
            if (s > 0) {
                left.push_back(make_pair(adj[i], frac));
            }
            else {
                right.push_back(make_pair(adj[i], frac));
            }
        }
        sort(all(left), Cmp);
        sort(all(right), Cmp); 
        reverse(all9(right));
        size_t curIndex = 1;
        forv(i, left) adj[curIndex++] = left[i].first;
        forv(i, right) adj[curIndex++] = right[i].first;        
    }

    

    return 0;
}