#define _CRT_SECURE_NO_WARNINGS

#include "point.h"
#include "line.h"
#include "plane.h"
#include "facet.h"
#include "utils.h"
#include "convex_figure.h"
#include "convex_hull_simple.h"

#include <iostream>
#include <cmath>
#include <iterator>
#include <set>
#include <stack>
#include <cstdlib>
#include <deque>

using namespace std;

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

    if (endId == 0) {
        endId = pb.size();
    }
    deque<Point> db;
    forn(i, endId) {
        db.push_back(pb[i]);
    }

    Point middlePoint = (db.front() + db.back()) / 2.0;

    Id firstIndexA = findTangent(pa, CompareLeft(middlePoint));
    Id lastIndexA = findTangent(pa, CompareNotRight(middlePoint)) + 1;

    rotate(pa.begin(), pa.begin() + firstIndexA, pa.end());
    lastIndexA = (lastIndexA + pa.size() - firstIndexA) % pa.size();

    if (lastIndexA == 0) {
        lastIndexA = pa.size();
    }

    deque<Point> da;
    forn(i, lastIndexA) {
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
        if (point.id() == polyhedron[v].id()) continue;
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
        if (polyhedron[v].id() == point.id()) continue;
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
        return pTwo[idTwo].id();
    }
    else {
        return pOne[idOne].id();
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

    //merge and removal of invisible facets

    Id a1 = phdOne.indexOf((*polygon)[vertices12.first].id());
    Id b1 = phdTwo.indexOf((*polygon)[vertices12.second].id());
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

    Ids mapIdOne = phdOne.leaveReachedFrom(boundaryOne.back());
    Ids mapIdTwo = phdTwo.leaveReachedFrom(boundaryTwo.back());
    forv(i, edges) {
        edges[i].setFrom(mapIdOne[edges[i].from()]);
        edges[i].setTo(mapIdTwo[edges[i].to()]);
    }
    
    phdOne.merge(phdTwo, edges);

    *polyhedron = phdOne;
}

typedef std::vector<std::vector<bool> > Used;
typedef std::vector<std::map<Id, Id> > InverseEdges;

void dfs(Id prev, Id v, const ConvexFigure::AdjacencyList& graph, 
    Used& used, const InverseEdges& ie, Ids& obs, const Ids& idMap, 
    Facets* facets)
{
    Id cur = (prev + 1) % graph[v].size();
    bool foundFacet = true;
    while (prev != cur) {
        if (!used[v][cur]) {
            used[v][cur] = true;
            obs.push_back(graph[v][cur]);
            dfs(ie[v].find(graph[v][cur])->second, graph[v][cur], graph, used, ie, obs, idMap, facets);
            obs.pop_back();
            foundFacet = false;
        } 
        else if (foundFacet) {
            foundFacet = false;
            assert(!obs.empty());
            Ids::iterator iter = obs.end()-1;
            assert(v == obs.back());
            while (*(--iter) != obs.back());
            //Ids ids(iter, obs.end());
            //while (iter != obs.end()) ids.push_back(idMap[*(iter++)]);            
            //facets->push_back(Facet(ids));
            facets->push_back(Facet(Ids(iter+1, obs.end())));
            facets->back().order();
        }
        cur = (cur + 1) % graph[v].size();
    }
}

void extractFacets(const Polyhedron& polyhedron, Facets* facets) 
{
    InverseEdges inverseEdges(polyhedron.size());

    forn(i, polyhedron.size()) {
        const Ids& adj = polyhedron.adjacentVertices(i);
        forv(j, adj) {
            inverseEdges[adj[j]][i] = j;
        }
    } 

    Used used(polyhedron.size());
    forv(i, used) {
        used[i] = std::vector<bool>(polyhedron.adjacentVertices(i).size(), false);
    }
    Ids observed;
    observed.push_back(0);

    const ConvexFigure::AdjacencyList& graph = polyhedron.graph();

    Ids idMap(polyhedron.size());
    forv(i, idMap) {
        idMap[i] = polyhedron[i].id();
    }

    dfs(0, 0, polyhedron.graph(), used, inverseEdges, observed, idMap, facets);
}

void solve() 
{
    int numPoints;
    cin >> numPoints;
    Points points;
    forn(i, numPoints) {
        Point point;
        cin >> point;
        point.setId(i);
        points.push_back(point);      
    }       

    sort(points.begin(), points.end());
    
    Polyhedron polyhedron;
    Polygon polygon;
    convexHull(points, &polyhedron, &polygon);

    Facets answer;

    extractFacets(polyhedron, &answer);

    sort(answer.begin(), answer.end());
    cout << answer.size() << endl;
    forn(i, answer.size()) {
        cout << answer[i] << endl;        
    }
}

int main() 
{
    freopen("input.txt", "rt", stdin);
    solve();
    return 0;
}
