#include "point.h"
#include "line.h"
#include "plane.h"
#include "facet.h"
#include "utils.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <deque>

using namespace std;
#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)
#define forv(i, v) forn(i, v.size())

typedef size_t Id;
typedef std::vector<Id> Ids;

class ConvexFigure 
{
public:
    typedef std::vector<std::vector<Id> > AdjacencyList;

    ConvexFigure() {}

    ConvexFigure(const Points& points) : points_(points), 
            graph_(std::vector<std::vector<Id> >(points.size())) {}
    virtual ~ConvexFigure() {}

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
            if (graph_[from][i] == to) return true;
        }
        return false;
    }

    const std::vector<Id>& adjacentVertices(Id vertex) const 
    {
        return graph_[vertex];
    }

    const Point& operator[](size_t index) const 
    {
        return points_[index];        
    }

    size_t indexOf(Id id) const 
    {
        forv(i, points_) {
            if (points_[i].id() == id) {
                return i;
            }
        }
        return std::numeric_limits<size_t>::max();
    }

    void clear() 
    {
        graph_.clear();
        points_.clear();
    }

    size_t size() const { return points_.size(); }

protected:
    AdjacencyList graph_;
    Points points_;       
};

void dfs(Id v, const ConvexFigure::AdjacencyList& graph, std::vector<bool>& used) 
{
    if (used[v]) return;
    used[v] = true;
    forv(i, graph[v]) dfs(graph[v][i], graph, used);
}

class Polyhedron: public ConvexFigure
{
public:
    Polyhedron() {}
    Polyhedron(const Points& points) : ConvexFigure(points) {}
    virtual ~Polyhedron() {}

    Ids recalc(Id source) 
    {
        std::vector<bool> used(size(), false);
        dfs(source, graph_, used);
        Ids mapIds(size(), 0);
        for(size_t i = 1; i < size(); ++i) {
            mapIds[i] = mapIds[i-1] + static_cast<size_t>(used[i-1]);
        }
        forv(i, graph_) {
            forv(j, graph_[i]) {
                graph_[i][j] = mapIds[graph_[i][j]];
            }
        }
        size_t cur = 0;
        forv(i, graph_) {
            if (used[i]) {
                graph_[cur++] = graph_[i];
            }
        }
        graph_.resize(cur);
        return mapIds;
    }

    void removeEdges(Id source, Id start, Id end) 
    {
        start = (start + 1) % graph_[source].size();
        rotate(graph_[source].begin(), graph_[source].begin() + start, graph_[source].end());
        end = (end + graph_[source].size() - start) % graph_[source].size();
        graph_[source].erase(graph_[source].begin(), graph_[source].begin() + end);
    }

    void merge(const Polyhedron& polyhedron, const Edges& edges) 
    {
        size_t curSize = size();
        forn(i, polyhedron.size()) {
            points_.push_back(polyhedron[i]);
            graph_.push_back(Ids());
        }
        
        forv(i, polyhedron) {
            const Ids& adj = polyhedron.adjacentVertices(i);
            forv(j, adj) {
                addEdge(i + curSize, adj[j] + curSize);
            }
        }
        
        forv(i, edges) {
            const Edge& edge = edges[i];
            addEdge(edge.from(), edge.to() + curSize);
            addEdge(edge.to() + curSize, edge.from());
        }
    }

private:
};

class Polygon: public  ConvexFigure
{
public:     
    Polygon() {}
    Polygon(const Points& points) : ConvexFigure(points) {}
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
    forn(i, convexHull.size()) {
        polygon->addEdge(i, (i+1)%convexHull.size());
    }
}

void addPoint(Points* points, const Point& point)
{
    forn(i, points->size()) {
        if ((*points)[i].id() == point.id()) return;
    }
    points->push_back(point);
}

void convexHullSimple(Points points, Polyhedron* polyhedron, Polygon* polygon)
{
    assert(points.size() <= 7);
    convexHull2(points, polygon);
    Points convexHull;
    std::map<Id, Ids> graph;
    forv(k, points) {
        forn(j, k) {
            forn(i, j) {
                const Plane plane(points[i], points[j], points[k]);
                if (below(points, plane)) {
                    addPoint(&convexHull, points[i]);
                    addPoint(&convexHull, points[j]);
                    addPoint(&convexHull, points[k]);                   
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
                const Plane plane(points[v], points[graph[v][i-1]], points[graph[v][j]]);
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

class Compare 
{
public:
    Compare(const Point& point) : point_(point) {}
    virtual bool operator()(const Point& p1, const Point& p2) const = 0;
protected:
    Point point_;
};

class CompareLeft: public Compare
{
public:
    CompareLeft(const Point& point) : Compare(point) {}
    virtual bool operator()(const Point& p1, const Point& p2) const
    {
        return turnsLeft(point_, p1, p2);
    }
};

class CompareNotRight: public Compare
{
public:
    CompareNotRight(const Point& point) : Compare(point) {}
    virtual bool operator()(const Point& p1, const Point& p2) const
    {
        return !turnsLeft(point_, p1, p2);
    }
}; 


Id findTangent(const Points& points, const Compare& comp)
{
    assert(points.size() >= 3);
    size_t cur = 0, next = cur + 1;
    while (comp(points[cur], points[next])) {
        next = cur;
        cur = (cur + points.size() - 1) % points.size();
    }    
    while (!comp(points[cur], points[next])) {
        cur = next;
        next = (next + 1) % points.size();
    }
    return cur;
}

// Polygons are convex, vertices are counter-clockwise ordered
std::pair<Id, Id> merge(const Polygon& polygonA, const Polygon& polygonB, Polygon* result)
{
    Points pa = polygonA.vertices();
    Points pb = polygonB.vertices();

    Point innerPoint = (pa[0] + pa[1] + pa[2]) / 3.0;

    Id beginId = findTangent(pb, CompareLeft(innerPoint));
    Id endId = findTangent(pb, CompareNotRight(innerPoint)) + 1;

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
        if (da.size() >= 2 && !turnsLeft(db.back(), da[0], da[1])) {
            da.pop_front();            
            pointRemoved = true;
        }
        if (db.size() >= 2 && !turnsLeft(db[db.size()-2], db.back(), da.front())) {
            db.pop_back();
            pointRemoved = true;
        }
        if (da.size() >= 2 && !turnsLeft(da[da.size()-2], da.back(), db[0])) {   
            da.pop_back();
            pointRemoved = true;
        }
        if (db.size() >= 2 && !turnsLeft(da.back(), db[0], db[1])) {
            db.pop_front();
            pointRemoved = true;
        }
    } while (pointRemoved);

    Points mergedPoints(da.begin(), da.end());
    copy(db.begin(), db.end(), back_inserter(mergedPoints));

    *result = Polygon(mergedPoints);

    forn(i, da.size()+db.size()) {
        result->addEdge(i, (i+1) % (da.size() + db.size()));
    } 

    return std::make_pair(da.size()-1, da.size());
}

//TODO refactor
Id findClosestOne(const Polyhedron& polyhedron, Id id, const Point& point, const Point& ort)
{
    const Ids& adj = polyhedron.adjacentVertices(id);
    double best = -1;
    Id result = std::numeric_limits<size_t>::max();
    forv(i, adj) {
        Id v = adj[i];
        double scalar;
        if ((scalar = scalarProduct(ortVector(polyhedron[id], point, polyhedron[v]), ort)) > best) {
            best = scalar;
            result = v;
        }
    }
    assert(result != std::numeric_limits<size_t>::max());
    return result;
}

Id findClosestTwo(const Polyhedron& polyhedron, Id id, const Point& point, const Point& ort)
{
    const Ids& adj = polyhedron.adjacentVertices(id);
    double best = -1;
    Id result = std::numeric_limits<size_t>::max();
    forv(i, adj) {
        Id v = adj[i];
        double scalar;
        if ((scalar = scalarProduct(ortVector(polyhedron[id], polyhedron[v], point), ort)) > best) {
            best = scalar;
            result = v;
        }
    }
    assert(result != std::numeric_limits<size_t>::max());
    return result;
}

Id findClosest(const Polyhedron& pOne, const Polyhedron& pTwo, Id idOne, Id idTwo, const Point& ort) 
{
    Id one = findClosestOne(pOne, idOne, pTwo[idTwo], ort);
    Id two = findClosestTwo(pTwo, idTwo, pOne[idOne], ort);
    if (scalarProduct(ortVector(pOne[one], pOne[idOne], pTwo[idTwo]), ort) < 
        scalarProduct(ortVector(pOne[idOne], pTwo[idTwo], pTwo[two]), ort)) {
        return two;
    }
    else {
        return one;
    }
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

//    Id nextPointId = polygon->adjacentVertices(startPoint.id())[0];

    //TODO make merge and removal of invisible facets

    Id a1 = phdOne.indexOf(plgOne[vertices12.first].id());
    Id b1 = phdTwo.indexOf(plgTwo[vertices12.second].id());
    Ids boundaryOne(1, a1);
    Ids boundaryTwo(1, b1);


    Edges edges;
    edges.push_back(Edge(a1, b1));
    
    Id closestId = findClosest(phdOne, phdTwo, boundaryOne.back(), boundaryTwo.back(), 
        ortVector(phdTwo[b1], phdOne[a1], phdOne[a1] + Point(0, 0, -1)));

    if (phdOne.indexOf(closestId) != std::numeric_limits<size_t>::max()) {
        boundaryOne.push_back(phdOne.indexOf(closestId));
    } else {
        assert(phdTwo.indexOf(closestId) != std::numeric_limits<size_t>::max());
        boundaryTwo.push_back(phdTwo.indexOf(closestId));
    }

    Point prevPoint = boundaryOne.size() == 2 ? phdOne[boundaryOne[0]] : phdTwo[boundaryTwo[0]];

    while (boundaryOne.back() != boundaryOne[0] || 
        boundaryTwo.back() != boundaryTwo[0]) {
        edges.push_back(Edge(boundaryOne.back(), boundaryTwo.back()));
        const Plane plane(prevPoint, phdTwo[boundaryTwo.back()], phdOne[boundaryOne.back()]);
        const Point ort(ortVector(prevPoint, phdTwo[boundaryTwo.back()], phdOne[boundaryOne.back()]));
        Id closestIdOne = findClosestOne(phdOne, boundaryOne.back(), phdTwo[boundaryTwo.back()], ort);
        Id closestIdTwo = findClosestTwo(phdTwo, boundaryTwo.back(), phdOne[boundaryOne.back()], ort);

        if (scalarProduct(ort, ortVector(phdOne[closestIdOne], phdOne[boundaryOne.back()],
                                phdTwo[boundaryTwo.back()])) <
            scalarProduct(ort, ortVector(phdOne[boundaryOne.back()], phdTwo[boundaryTwo.back()],
                                phdTwo[closestIdTwo]))) {
            prevPoint = phdOne[boundaryOne.back()];
            boundaryOne.push_back(closestIdOne);
        }        
        else {
            prevPoint = phdTwo[boundaryTwo.back()];
            boundaryTwo.push_back(closestIdTwo);
        }
    }
    //removing invisible edges

    boundaryOne.pop_back();
    boundaryTwo.pop_back();

    forv(i, boundaryOne) {
        size_t prevId = (i + boundaryOne.size() - 1) % boundaryOne.size();
        size_t nextId = (i + 1) % boundaryOne.size();
        phdOne.removeEdges(boundaryOne[i], boundaryOne[prevId], boundaryOne[nextId]);
    }

    forv(i, boundaryTwo) {
        size_t prevId = (i + boundaryTwo.size() - 1) % boundaryTwo.size();
        size_t nextId = (i + 1) % boundaryTwo.size();
        phdTwo.removeEdges(boundaryTwo[i], boundaryTwo[nextId], boundaryTwo[prevId]);
    }

    vector<Id> mapIdOne = phdOne.recalc(boundaryOne.back());
    vector<Id> mapIdTwo = phdTwo.recalc(boundaryTwo.back());
    forv(i, edges) {
        edges[i].setFrom(mapIdOne[edges[i].from()]);
        edges[i].setTo(mapIdTwo[edges[i].to()]);
    }
    
    phdOne.merge(phdTwo, edges);

    *polyhedron = phdOne;
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
    solve();
    return 0;
}
