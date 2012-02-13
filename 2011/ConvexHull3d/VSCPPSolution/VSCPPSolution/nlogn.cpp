#define _CRT_SECURE_NO_WARNINGS

#include "point.h"
#include "line.h"
#include "plane.h"
#include "facet.h"
#include "utils.h"
#include "graph.h"
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

bool symmetric(ConvexFigure::AdjacencyList graph)
{
    forv(i, graph) {
        sort(graph[i].begin(), graph[i].end());
    }
    forv(v, graph) {
        const Ids& adj = graph[v];
        forv(i, adj) {
            Id u = adj[i];
            if (!binary_search(graph[u].begin(), graph[u].end(), v)) return false;
        }
    }
    return true;
}

bool convexHalf(const Polyhedron& phd)
{
    const ConvexFigure::AdjacencyList& graph = phd.graph();
    forv(i, graph) {
        const Ids& adj = graph[i];
        forv(j, adj) {
            if (j == 0) continue;
            Plane plane(phd[i], phd[adj[j]], phd[adj[j-1]]);
            if (!below(phd.points(), plane)) return false;
        }
    }
    return true;
}

bool convex(const Polyhedron& phd)
{
    if (!convexHalf(phd)) return false;
    const ConvexFigure::AdjacencyList& graph = phd.graph();
    forv(i, graph) {
        const Ids& adj = graph[i];
        Plane plane(phd[i], phd[adj[0]], phd[adj.back()]);
        if (!below(phd.points(), plane)) return false;
    }
    return true;
}

Id findClosest(const Polyhedron& polyhedron, Id id, const Point& point, const Point& ort, bool reverse)
{
    double best = -1;
    const Ids& adj = polyhedron.adjacentVertices(id);
    Id result = std::numeric_limits<size_t>::max();
    double sign = 1.0;
    if (reverse) {
        sign = -1.0;
    }
    forv(i, adj) {
        Id v = adj[i];
        if (point.id() == polyhedron[v].id()) continue;
        double scalar;
        if ((scalar = sign * scalarProduct(ortVector(polyhedron[id], point, polyhedron[v]), ort)) > best) {
            best = scalar;
            result = v;
        }
    }
//    assert(result != std::numeric_limits<size_t>::max());
    return result;
}

Id findClosest(const Polyhedron& pOne, const Polyhedron& pTwo, Id idOne, Id idTwo, const Point& ort) 
{
    Id one = findClosest(pOne, idOne, pTwo[idTwo], ort, false);
    Id two = findClosest(pTwo, idTwo, pOne[idOne], ort, true);
    if (scalarProduct(ortVector(pOne[one], pOne[idOne], pTwo[idTwo]), ort) < 
        scalarProduct(ortVector(pOne[idOne], pTwo[idTwo], pTwo[two]), ort)) {
        return pTwo[two].id();
    }
    else {
        return pOne[one].id();
    }
}

void buildTriangulation(const Points& points, const Polyhedron& phdOne, const Polyhedron& phdTwo, 
    Id startIdOne, Id startIdTwo, Ids* boundaryOne, Ids* boundaryTwo, Edges* edges) 
{
    boundaryOne->push_back(startIdOne);
    boundaryTwo->push_back(startIdTwo);

    edges->push_back(Edge(startIdOne, startIdTwo));
    
    Id closestId = findClosest(phdOne, phdTwo, boundaryOne->back(), boundaryTwo->back(), 
        ortVector(phdTwo[startIdTwo], phdOne[startIdOne], phdOne[startIdOne] + Point(0, 0, -1)));

    //check on the same side from plane
    {
//        Plane plane(phdTwo[startIdTwo], phdOne[startIdOne], phdOne[startIdOne] + Point(0, 0, -1));
//        assert(below(points, plane));
    }

    if (phdOne.indexOf(closestId) != std::numeric_limits<size_t>::max()) {
        boundaryOne->push_back(phdOne.indexOf(closestId));
    } else {
//        assert(phdTwo.indexOf(closestId) != std::numeric_limits<size_t>::max());
        boundaryTwo->push_back(phdTwo.indexOf(closestId));
    }

    Point prevPoint = boundaryOne->size() == 2 ? phdOne[boundaryOne->at(0)] : phdTwo[boundaryTwo->at(0)];

    while (boundaryOne->back() != boundaryOne->at(0) || 
        boundaryTwo->back() != boundaryTwo->at(0)) {
        edges->push_back(Edge(boundaryOne->back(), boundaryTwo->back()));
        //check on the same side from plane
        {
//            const Plane plane(prevPoint, phdTwo[boundaryTwo->back()], phdOne[boundaryOne->back()]);
//            assert(below(points, plane));
        }
        const Point ort(ortVector(prevPoint, phdTwo[boundaryTwo->back()], phdOne[boundaryOne->back()]));
        Id closestIdOne = findClosest(phdOne, boundaryOne->back(), phdTwo[boundaryTwo->back()], ort, false);
        Id closestIdTwo = findClosest(phdTwo, boundaryTwo->back(), phdOne[boundaryOne->back()], ort, true);

        double scalarOne = scalarProduct(ort, ortVector(phdOne[closestIdOne], phdOne[boundaryOne->back()],
                                phdTwo[boundaryTwo->back()]));
        double scalarTwo = scalarProduct(ort, ortVector(phdOne[boundaryOne->back()], phdTwo[boundaryTwo->back()],
                                phdTwo[closestIdTwo]));
        if (scalarOne > scalarTwo) {
            prevPoint = phdOne[boundaryOne->back()];
            boundaryOne->push_back(closestIdOne);
        }        
        else {
            prevPoint = phdTwo[boundaryTwo->back()];
            boundaryTwo->push_back(closestIdTwo);
        }
    }
    if (boundaryOne->size() > 1) {
        boundaryOne->pop_back();
    }
    if (boundaryTwo->size() > 1) {
        boundaryTwo->pop_back();
    }
}

void merge(const Points& points, 
    const Polyhedron& phdOne, 
    const Polygon& plgOne, 
    const Polyhedron& phdTwo,
    const Polygon& plgTwo, 
    Polyhedron* polyhedron, 
    Polygon* polygon) 
{
    pair<Id, Id> vertices12 = merge(plgOne, plgTwo, polygon);

    Id a1 = phdOne.indexOf((*polygon)[vertices12.first].id());
    Id b1 = phdTwo.indexOf((*polygon)[vertices12.second].id());

    Ids boundaryOne, boundaryTwo;
    Edges edges;
    buildTriangulation(points, phdOne, phdTwo, a1, b1, &boundaryOne, &boundaryTwo, &edges);

    //removing invisible edges

//    Graph::writeToFile("graph1.gv", phdOne.graph());
    *polyhedron = phdOne;
    forv(i, boundaryOne) {
        size_t nextId = (i + boundaryOne.size() - 1) % boundaryOne.size();
        size_t prevId = (i + 1) % boundaryOne.size();
        polyhedron->removeEdges(boundaryOne[i], boundaryOne[prevId], boundaryOne[nextId]);
    }

//    Graph::writeToFile("rgraph1.gv", phdOne.graph());
//    Graph::writeToFile("graph2.gv", phdTwo.graph());
    Polyhedron other = phdTwo;
    forv(i, boundaryTwo) {
        size_t prevId = (i + boundaryTwo.size() - 1) % boundaryTwo.size();
        size_t nextId = (i + 1) % boundaryTwo.size();
        other.removeEdges(boundaryTwo[i], boundaryTwo[prevId], boundaryTwo[nextId]);
    }

//    Graph::writeToFile("rgraph2.gv", phdTwo.graph());
    
    Ids mapIdOne = polyhedron->leaveReachedFrom(boundaryOne.back());
//    assert(symmetric(polyhedron->graph()));
//    Graph::writeToFile("dgraph1.gv", phdOne.graph());
    Ids mapIdTwo = other.leaveReachedFrom(boundaryTwo.back());
//    Graph::writeToFile("dgraph2.gv", phdTwo.graph());
//    assert(symmetric(other.graph()));
    forv(i, edges) {
        edges[i].setFrom(mapIdOne[edges[i].from()]);
        edges[i].setTo(mapIdTwo[edges[i].to()]);
    }

    /*
    forv(i, edges) {
        Id a = edges[i].from();
        Id b = edges[i].to();
        cerr << (*polyhedron)[a] << endl;
        cerr << other[b] << endl;
    }
    */
    polyhedron->merge(other, edges);
}

//Points should be sorted lexicographically x, y, z
void convexHull(const Points& points, Polyhedron* polyhedron, Polygon* polygon) 
{
    if (points.size() <= 7) {
        convexHullSimple(points, polyhedron, polygon);
    } else {
        Points pointsOne(points.begin(), points.begin() + points.size()/2),
            pointsTwo(points.begin() + points.size()/2, points.end());
        Polyhedron phdOne, phdTwo;
        Polygon plgOne, plgTwo;
        convexHull(pointsOne, &phdOne, &plgOne);
        convexHull(pointsTwo, &phdTwo, &plgTwo);
        merge(points, phdOne, plgOne, phdTwo, plgTwo, polyhedron, polygon);
    }
//    Graph::writeToFile("graph_all.gv", polyhedron->graph());
//    assert(symmetric(polyhedron->graph()));
//    assert(convex(*polyhedron));
}

typedef std::vector<std::vector<bool> > Used;
typedef std::vector<std::map<Id, Id> > InverseEdges;

void dfs(Id prev, Id v, const ConvexFigure::AdjacencyList& graph, 
    Used& used, const InverseEdges& ie, Ids& obs, Facets* facets)
{
//    assert(prev < graph[v].size());
    Id cur = (prev + 1) % graph[v].size();
    bool foundFacet = true;
    while (prev != cur) {
        if (!used[v][cur]) {
            used[v][cur] = true;
            obs.push_back(graph[v][cur]);
            dfs(ie[v].find(graph[v][cur])->second, graph[v][cur], graph, used, ie, obs, facets);
            obs.pop_back();
            foundFacet = false;
        } 
        else if (foundFacet) {
            foundFacet = false;
//            assert(!obs.empty());
//            print<Id>(cerr, obs);
            Ids::iterator iter = obs.end()-1;
//            assert(v == obs.back());
            while (*(--iter) != obs.back());
//            cerr << "iter " << *iter << endl;
//            assert(*iter == obs.back());
            //Ids ids(iter, obs.end());
            //while (iter != obs.end()) ids.push_back(idMap[*(iter++)]);            
            //facets->push_back(Facet(ids));
            facets->push_back(Facet(Ids(iter+1, obs.end())));
        }
        cur = (cur + 1) % graph[v].size();
    }
}

void extractFacets(const Polyhedron& polyhedron, Facets* facets) 
{
    InverseEdges inverseEdges(polyhedron.size());
    
    const ConvexFigure::AdjacencyList& graph = polyhedron.graph();
    forn(i, polyhedron.size()) {
        const Ids& adj = polyhedron.adjacentVertices(i);
        forv(j, adj) {
            inverseEdges[adj[j]][i] = j;
        }
    } 

//    assert(symmetric(graph));

    forn(i, polyhedron.size()) {
        const Ids& adj = polyhedron.adjacentVertices(i);
        forv(j, adj) {
//            assert(inverseEdges[i].find(adj[j])->second < polyhedron.adjacentVertices(adj[j]).size());
        }
    }

    Used used(polyhedron.size());
    forv(i, used) {
        used[i] = std::vector<bool>(polyhedron.adjacentVertices(i).size(), false);
    }
    Ids observed(1);

    Ids idMap(polyhedron.size());
    forv(i, idMap) {
        idMap[i] = polyhedron[i].id();
    }

    dfs(0, 0, polyhedron.graph(), used, inverseEdges, observed, facets);

    forv(i, (*facets)) {
        (*facets)[i].mapIds(idMap);
        (*facets)[i].order();
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
        point.setId(i);
        points.push_back(point);      
    }       

    sort(points.begin(), points.end());
    
    Polyhedron polyhedron;
    Polygon polygon;
    convexHull(points, &polyhedron, &polygon);
//    assert(convex(polyhedron));
//    Graph::writeToFile("graph_all.gv", polyhedron.graph());

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
//    freopen("input.txt", "rt", stdin);
    solve();
    return 0;
}
