#include "point.h"
#include "line.h"
#include "plane.h"
#include "facet.h"
#include "utils.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>

using namespace std;
#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())

typedef size_t Id;
typedef std::vector<Id> Ids;

class ConvexFigure 
{
public:
    typedef std::vector<std::vector<Id> > AdjacencyList;
    ConvexFigure(const Points& points) : points_(points), 
            graph_(std::vector<std::vector<Id> >(points.size())) {}
    virtual ~ConvexFigure() = 0;

    void addEdge(Id from, Id to)
    {
        graph_[from].push_back(to);
    }

    void addEdge(const Edge& edge)
    {
        graph_[edge.from()].push_back(edge.to());
    }
     
    bool adjacent(Id from, Id to) const
    {
        forv(i, graph_[from]) {
            if (grpah_[from][i] == to) return true;
        }
        return false;
    }

    const std::vector<Id>& adjacentVertices(Id vertex) const 
    {
        return grpah_[vertex];
    }

    void clear() 
    {
        graph_.clear();
        points_.clear();
    }

private:
    AdjacencyList graph_;
    const Points points_;       
};

class Polyhedron: public ConvexFigure
{
public:
    Polyhedron() {}
    virtual ~Polyhedron() {}
private:
};

class Polygon: public  ConvexFigure
{
public:     
    Polygon() {}
    virtual ~Polygon() {}
    Points vertices() const
    {
        return points_;
    }

private:
};

class PointsComparator
{
public:
    PointsComparator(const Point& point) : origin_(point) {}

    bool operator() (const Point& pointA, const Point& pointB) const
    {
        const Point& A = pointA - origin_, B = pointB - origin_;
        double det = det2(A, B);
        if (fabs(det) < constants::EPS) {
            return A.squaredLength() < B.squaredLength();
        } else {
            return det > 0;
        }            
    }
private:
    Point origin_;
};

void convexHull2(Points points, Polygon* polygon)
{
    assert(points.size() >= 3);
    Points::iterator originIterator = 
        min_element(points.begin(), points.end());
    Point origin = *originIterator;
    swap(*points.begin(), *originIterator);

    PointsComparator comparator(origin); 

    sort(points.begin()+1, points.end(), comparator); 
    
    typedef Points ConvexHull;

    ConvexHull convexHull;
    Points::const_iterator iter = points.begin();
    forn(i, 2) {
        convexHull.push_back(*iter++);
    }
    for ( ;iter != points.end(); ++iter) {
        while (convexHull.size() >= 2 && 
            !turnsLeft(convexHull[convexHull.size()-2],
                       convexHull[convexHull.size()-1],
                       *iter)) {
            convexHull.pop_back();
        }
    }

    //remove intermediate points
    while (convexHull.size() >= 3 && 
        sameLine(convexHull[0], convexHull.back(), convexHull[convexHull.size()-2])) {
            convexHull.pop_back();
    }
    //TODO check if convexHull is ok, 
    //e.g. it does not contain tree points on the same line

    *polygon = Polygon(convexHull);
    forn(i, convexHull) {
        polygon->addEdge(i, (i+1)%convexHull.size());
    }
}

void addPoint(Points* points, const Point& point)
{
    forn(i, points->size()) {
        if (points->[i].id() == point.id()) return;
    }
    points->push_back(point);
}

void convexHullSimple(Points points, Polyhedron* polyhedron, Polygon* polygon)
{
    assert(points.size() <= 7);
    convexHull2(points, polygon);
    Points convexHull;
    std::map<Id, Ids > graph;
    forv(k, points) {
        forn(j, k) {
            forn(i, j) {
                const Plane plane(points[i], points[j], points[k]);
                if (below(points, plane)) {
                    addPoint(convexHull, points[i]);
                    addPoint(convexHull, points[j]);
                    addPoint(convexHull, points[k]);                   
                    graph[i].push_back(j);
                    graph[j].push_back(k);
                    graph[k].push_back(i);
                }  
            }
        }
    }            
    forv(v, graph) {
        for(size_t i = 1; i < graph[v].size(); ++i) {   
            for(size_t j = i + 1; j < graph[v].size(); ++j) {
                const Plane plane(points[v], points[graph[v][i-1]], points[grpah[v][j]]);
                if (below(points, plane)) {
                    swap(graph[v][i], graph[v][j]);
                    break;
                }
            }
        }
        {
            const Plane plane(points[v], points[graph[v][graph[v].size()-1]], points[graph[v][0]]);
            assert(below(points, plane));
        }
    }
    //TODO test
    vector<Id> idMap(points.size());
    forv(i, convexHull) {
        idMap[convexHull[i].id()] = i;
    }
    *polyhedron = Polyhedron(convexHull);
    forv(v, graph) {    
        forv(j, graph[v]) {
            Id u = graph[v][j];
            polyhedron->addEdge(idMap[v], idMap[u]);
        }
    }
}

// Polygons are convex, vertices are counter-clockwise ordered
std::pair<Id, Id> merge(const Polygon& polygonA, const Polygon& polygonB, Polygon* result)
{
    Points pa = polygonA.vertices();
    Points pb = polygonB.vertices();

    Point innerPoint = (pa[0] + pa[1] + pa[2]) / 3.0;

    Id beginId = findTangent(pb, innerPoint, cmp1);
    Id endId = findTangent(pb, innerPoint, cmp2) + 1;

    rotate(pb.begin(), pb.begin() + beginId, pb.end());
    endId = (endId + pb.size() - beginId) % pb.size();

    deque<Point> db;
    forn(i, endId) {
        db.push_back(pb[i]);
    }

    Point middlePoint = (db.front() + db.back()) / 2.0;

    const Line line(innerPoint, middlePoint);

    Id firstIndexA = 0;
    while (line.signedDistance(pa[firstIndexA]) > -constants::EPS) {
        firstIndexA = (firstIndexA + 1) % pa.size();        
    } 
    while (line.signedDistance(pa[firstIndexA]) < - constants::EPS) {
        firstIndexA = (firstIndexA + pa.size() - 1) % pa.size();
    }

    ++firstIndexA;
    rotate(pa.begin(), pa.begin() + firstIndexA, pa.end());

    if (fabs(line.signedDistance(pa.back())) < constants::EPS) {
        pa.pop_back();
    }

    deque<Point> da;

    forv(i, pa) {
        da.push_back(pa[i]);
    }

    //da and db can be merged

    bool pointRemoved;
    do {  
        pointRemoved = false;        
        if (da.size() >= 2 && !turnLeft(db.back(), da[0], da[1])) {
            da.pop_front();            
            pointRemoved = true;
        }
        if (db.size() >= 2 && !turnLeft(db[db.size()-2], db.back(), da.front())) {
            db.pop_back();
            pointRemoved = true;
        }
        if (da.size() >= 2 && !turnLeft(da[da.size()-2], da.back(), db[0])) {   
            da.pop_back();
            pointRemoved = true;
        }
        if (db.size() >= 2 && !turnLeft(da.back(), db[0], db[1])) {
            db.pop_front();
            pointRemoved = true;
        }
    } while (pointRemoved);

    Points mergedPoints(da.begin(), da.end());
    mergedPoints.append(db.begin(), db.end());

    *result = Polygon(mergedPoints);

    forn(i, da.size()+db.size()) {
        result->addEdge(i, (i+1) % (da.size() + db.size()));
    } 

    return std::make_pair(da.size()-1, da.size());
}


//Points should be sorted lexicographically x, y, z
void convexHull(const Points& points, Polyhedron* polyhedron, Polygon* polygon) 
{
    if (points.size() <= 7) {
        convexHullSimple(points, polyhedron, polygon);
        return;            
    }

    Points pointsOne(points.begin(), points.begin() + points.size()/2),
            pointsTwo(points.begin() + points.size()/2, points.end());
    Polyhedron phdOne, phdTwo;
    Polygon plgOne, plgTwo;
    convexHull(pointsOne, &phdOne, &plgOne);
    convexHull(pointsTwo, &phdTwo, &plgTwo);
    pair<Id, Id> vertices12 = merge(plgOne, plgTwo, polygon);

    Id nextPointId = polygon->adjacentVertices(startPoint.id())[0];

    //TODO make merge and removal of invisible facets

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
