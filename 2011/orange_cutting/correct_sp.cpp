#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#pragma comment (linker, "/STACK:1024000000")
#include <vector>
#include <cmath>
#include <iostream>
#include <map>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <cassert>
#include <cstdio>
#include <sstream>
using namespace std;

#define forn(i, n) for(int i = 0; i < static_cast<int>(n); ++i)
#define for1(i, n) for(int i = 1; i <= static_cast<int>(n); ++i)
#define forv(i, v) forn(i, v.size())
#define all(v) v.begin(), v.end()


typedef long long Long;
typedef size_t Id;
typedef vector<Id> Ids;
typedef vector<Ids> Graph;
typedef vector<vector<bool> > Used;
typedef vector<map<Id, Id> > InverseEdges;
typedef pair<Id, pair<Long, Long> > IdFracPair;

double sqrt(Long x)
{
    return sqrt(static_cast<double>(x));
}

Long det(Long a, Long b, Long c, Long d) 
{
    return a * d - b * c;
}

Long gcd(Long a, Long b)
{
    if (a == 0) return b;
    return gcd(b % a, a);
}

class Point
{
public:
    Point() : x_(0), y_(0), z_(0) {}

    Point(Long x, Long y, Long z) :
        x_(x), y_(y), z_(z) {}
    
    Long x() const { return x_; }
    void setX(Long x) { x_ = x; }

    Long y() const { return y_; }
    void setY(Long y) { y_ = y; }

    Long z() const { return z_; }
    void setZ(Long z) { z_ = z; }

    Long squaredLength() const
    {
        return x() * x() + y() * y() + z() * z();
    }

    Point normal() const 
    {   
//        cerr << "normal() : " << x() << " " << y() << " " << z() << endl;
        Long len = gcd(abs(x()), gcd(abs(y()), abs(z())));
        return Point(x()/len, y()/len, z()/len);
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
    return out;
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
    return pointA.x() == pointB.x() &&
           pointA.y() == pointB.y() &&
           pointA.z() == pointB.z();
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

Long sqr(Long x)
{
    return x * x;
}

bool colinear(const Point& a, const Point& b)
{
    return sqr(scalarProduct(a, b)) == a.squaredLength() * b.squaredLength();
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
        return signedDistance(point) == 0;    
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

class Facet
{
public:
    Facet() {}
    Facet(const Ids& indices)
    {
        vertices_ = indices;
    }

    Id operator[](Id index) const
    {
        assert(index < size());
        return vertices_[index];
    }

    void add(Id vertexIndex) 
    {
        vertices_.push_back(vertexIndex);
    }

    size_t size() const
    {
        return vertices_.size();
    }

    void order() 
    {
        size_t shift = static_cast<size_t>(
                std::min_element(vertices_.begin(), vertices_.end()) 
                        - vertices_.begin());
        std::rotate(vertices_.begin(), vertices_.begin() + shift, vertices_.end());
    }

    friend std::ostream& operator<<(std::ostream& out, const Facet& facet);

    std::string str() const 
    {
        std::ostringstream out;
        out << size() << " ";
        out << this->operator[](0) << " " << this->operator[](1) << " " << this->operator[](2);
        return out.str();
    }

private:
    Ids vertices_;                        
};

typedef vector<Facet> Facets;
typedef vector<Plane> Planes;

std::ostream& operator<<(std::ostream& out, const Facet& facet)
{
    out << facet.size();
    for(size_t i = 0; i < facet.size(); ++i) {
        out << " " << facet[i];
    }
    return out;
}

bool operator<(const Facet& facetA, const Facet& facetB)
{
    if (facetA.size() != facetB.size()) {
        return facetA.size() < facetB.size();
    }        
    for(size_t i = 0; i < facetA.size(); ++i) {
        if (facetA[i] != facetB[i]) {
            return facetA[i] < facetB[i];
        }
    }
    return false;
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

bool Cmp(const IdFracPair& one, const IdFracPair& two) 
{
    Long _1 = one.second.first * two.second.second;
    Long _2 = one.second.second * two.second.first;
    return _1 < _2;
}

bool turnsLeft(const Point& a, const Point& b, const Point& c)
{
    return scalarProduct(c, vectorProduct(a, b)) > 0;
}

int depth;

void dfs(Id prev, Id v, const Graph& graph, 
    Used& used, const InverseEdges& ie, Ids& obs, Facets* facets)
{
    ++depth;
//    cerr << "DFS depth : " << depth << endl;    
//    cerr << "Obs size : " << obs.size() << endl;
    assert(prev < graph[v].size());
    Id cur = prev;
    bool foundFacet = true;
//    cerr << "START cycle" << endl;
    do {
//        cerr << "iteration : " << ++iterationNumber << endl;
        cur = (cur + 1) % graph[v].size();
        if (!used[v][cur]) {
            used[v][cur] = true;
            obs.push_back(graph[v][cur]);
            dfs(ie[v].find(graph[v][cur])->second, graph[v][cur], graph, used, ie, obs, facets);
            obs.pop_back();
            foundFacet = false;
        } 
        else if (foundFacet) {
//            cerr << "adding " << facets->size() << " facet" << endl;
            foundFacet = false;
            assert(!obs.empty());
            Ids::iterator iter = obs.end()-1;
            assert(v == obs.back());
            assert(iter != obs.begin());
            while (*(--iter) != obs.back()) {
                assert(iter != obs.begin());
            }
            assert(*iter == obs.back());
            facets->push_back(Facet(Ids(iter+1, obs.end())));
//            cerr << "added facet" << endl;
        }
    } while (cur != prev);
    --depth;
}

struct CmpPnt
{
CmpPnt(const Point& point, const Points& points) : ort_(point), points_(points) {}

bool operator() (Id a, Id b)
{
    return turnsLeft(points_.at(a), points_.at(b), ort_);
}
    const Point ort_;
    const Points& points_;
};

void printAnswer(vector<double> answer)
{
    sort(all(answer));
    cout.precision(10);
    cout << fixed;
    cout << answer.size() << endl;
    forv(i, answer) {
        cout << answer[i] << endl;        
    }
}

double angle(const Point& a, const Point& b) 
{
    const double lenOne = sqrt(a.squaredLength());
    const double lenTwo = sqrt(b.squaredLength());
//    cerr << "In angle" << endl;
//    cerr << a << " " << b << endl;
    const double scalar = scalarProduct(a, b) / lenOne / lenTwo;
//    cerr << "scalar : " << scalar << endl;
    return acos(scalar);
}

//All points belong to the same plane.
//All planes with normal orts equals 'points' intersect
//in a line passing through linePoint and O.
void sort(const Point& linePoint, const Points& points, Ids& ids)
{
    if (ids.size() <= 1) return;
    Ids left(1, ids.at(0)), right;
    CmpPnt comparator(linePoint, points);
    for1(i, ids.size()-1) {
        if (comparator(ids.at(0), ids[i])) {
            left.push_back(ids[i]);
        }       
        else {
            right.push_back(ids[i]);
        }
    }
    sort(all(left), comparator);
    forv(i, left) {
        if (i > 0) {
            assert(comparator(left[i-1], left[i]));
        }
    }
    sort(all(right), comparator);
    forv(i, right) {
        if (i > 0) {
            assert(comparator(right[i-1], right[i]));
        }
    }
    assert(left.size() > 0);
    assert(right.size() > 0);
    assert(comparator(left.back(), right.at(0)));
    assert(comparator(right.back(), left.at(0)));
    ids = left;
   //TODO use std function append
    forv(i, right) ids.push_back(right[i]);
}

void checkSum(const vector<double>& numbers, double r)
{
    double sum = - 4 * M_PI * r * r * r / 3.0;
    sum = accumulate(all(numbers), sum);
    assert(fabs(sum) < 1e-4);
}

//all planes intersect in a line passing through O and linePoint
void solve2Points(Long r, Point linePoint, const Planes& planes)
{
    //at least two planes
    Points points;
    Ids ids;
    forv(i, planes) {   
        points.push_back(planes[i].ort());
        points.push_back(-planes[i].ort());
        ids.push_back(2*i);
        ids.push_back(2*i+1);
    }        
    sort(linePoint, points, ids);
    ids.push_back(ids.at(0));
    vector<double> answer;
    for1(i, ids.size() - 1) {
        const double ang = angle(points[ids.at(i-1)], points[ids.at(i)]);
        answer.push_back(ang * 2 * r * r * r / 3.0);
    }
    checkSum(answer, r);
    printAnswer(answer);
}

void orientate(Point& v)
{
    if (v.x() < 0 || (v.x() == 0 && v.y() < 0) 
        || (v.x() == 0 && v.y() == 0 && v.z() < 0)) {
        v.setX(-v.x());
        v.setY(-v.y());
        v.setZ(-v.z());
    }
}

int main()
{
    Points checkingOrts; 
    size_t numPlanes;
    Long radius;
    cin >> radius >> numPlanes;
    vector<Plane> planes;
    forn(i, numPlanes) {
        Point point;
        cin >> point;
        planes.push_back(Plane(point.x(), point.y(), point.z(), 0));
        checkingOrts.push_back(point);
        orientate(checkingOrts.back());        
    }

    //one plane
    if (numPlanes == 1) {
        vector<double> answer;
        answer.push_back(2 * M_PI * radius * radius * radius / 3.0);
        answer.push_back(answer.back());
        checkSum(answer, radius);
        printAnswer(answer);
        return 0;
    }

//    cerr << "START intersecting planes" << endl;
    Points points;
    vector<Points> intersections(numPlanes);
    forn(i, numPlanes) {
        Points& pointsOnPlane = intersections[i];
        forn(j, numPlanes) {
            if (i == j) continue;
            const Point p = vectorProduct(planes[i].ort(), planes[j].ort()).normal();
            pointsOnPlane.push_back(p);
            pointsOnPlane.push_back(-p);
        }
        forv(j, pointsOnPlane) {
            points.push_back(pointsOnPlane[j]);
        }
    }

//    cerr << "END intersection planes" << endl;

    sort(all(points));
    points.erase(unique(all(points)), points.end());

    
    Graph graph(points.size());

    //two points, all planes intersect in one line
    if (points.size() == 2) {
        solve2Points(radius, points[0], planes);
        return 0;
    }

//    cerr << "START adding edges" << endl;
    forn(i, numPlanes) {
        Ids ids;
        forv(j, intersections[i]) {
            ids.push_back(lower_bound(all(points), intersections[i][j]) - points.begin());                    
        }
        sort(all(ids));
        ids.erase(unique(all(ids)), ids.end());
        sort(planes[i].ort(), points, ids);
        ids.erase(unique(all(ids)), ids.end());
//        cerr << "Ids : " << ids.size() << endl;
        ids.push_back(ids.at(0));
        for1(j, ids.size()-1) {
            assert(turnsLeft(points[ids[j-1]], points[ids[j]], planes[i].ort()));
            //TODO think if Long type overflows
            assert(colinear(planes[i].ort(), vectorProduct(points[ids[j-1]], points[ids[j]])));
            graph[ids[j-1]].push_back(ids[j]);
            graph[ids[j]].push_back(ids[j-1]);
        }            
    }
//    cerr << "END adding edges" << endl;

//    cerr << "Created graph with " << graph.size() << " points" << endl;
    // Orientating adjacent vertices.
    // Removing equal.

    forv(v, graph) {
        Ids& adj = graph[v];
        sort(points[v], points, adj);
    }

//    cerr << "Sorted adjacent vertices" << endl;

//    cerr << "Checking adjacent vertices are sorted" << endl;

    forv(i, graph) {
        const Ids& adj = graph[i];
        forv(j, adj) {
            int nx = (j + 1) % adj.size();
            assert(turnsLeft(points[adj[j]], points[adj[nx]], points[i]));
        }
    }

//    cerr << "Checked adjacent vertices are sorted" << endl;

    size_t numVertices = points.size();
    Used used(numVertices);
    forv(i, graph) {
        used[i].resize(graph[i].size());
    }
    Ids obs(1);
    Facets facets;
    InverseEdges invEdges(numVertices);
    forv(v, graph) {
        const Ids& adj = graph[v];
        forv(i, adj) {
            invEdges[adj[i]][v] = i;
        }
    }
//    cerr << "START dfs" << endl;
    dfs(0, 0, graph, used, invEdges, obs, &facets);
//    cerr << "END dfs" << endl;

    //TODO output answer
//    cerr << "START calculating parts" << endl;
//    forv(i, points) {
//        cerr << points[i] << endl;
//    }
    vector<double> answer;
    forv(i, facets) {
        const Facet& facet = facets[i];
        double anglesSum = 0;
        forv(prevIndex, facet) {    
            size_t curIndex = (prevIndex + 1) % facet.size();
            size_t nextIndex = (curIndex + 1) % facet.size();
            anglesSum += angle(vectorProduct(points[facet[prevIndex]], points[facet[curIndex]]),
                                vectorProduct(points[facet[nextIndex]], points[facet[curIndex]]));
        }
        assert(facet.size() >= 3);
        anglesSum -= (facet.size() - 2) * M_PI;
        anglesSum *= radius * radius * radius / 3.0;
        answer.push_back(anglesSum);
    }
    checkSum(answer, radius);
    printAnswer(answer);    

    return 0;
}
