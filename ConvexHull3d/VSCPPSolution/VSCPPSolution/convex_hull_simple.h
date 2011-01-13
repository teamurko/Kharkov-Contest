#ifndef CH_SIMPLE_H_INCLUDED

#include "point.h"
#include "convex_figure.h"
#include "convex_hull2.h"
#include <map>

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
                Plane plane(points[k], points[i], points[j]);
                if (below(points, plane)) {
                    addPoint(&convexHull, points[i]);
                    addPoint(&convexHull, points[j]);
                    addPoint(&convexHull, points[k]);                   
                    graph[i].push_back(j);
                    graph[j].push_back(k);
                    graph[k].push_back(i);
//                    cerr << "BELOW" << endl;
                }  
                plane = Plane(points[k], points[j], points[i]);
                if (below(points, plane)) {
                    addPoint(&convexHull, points[i]);
                    addPoint(&convexHull, points[k]);
                    addPoint(&convexHull, points[j]);                   
                    graph[i].push_back(k);
                    graph[k].push_back(j);
                    graph[j].push_back(i);
//                    cerr << "BELOW" << endl;
                }
            }
        }
    }

    for(std::map<Id, Ids>::iterator vIter = graph.begin(); vIter != graph.end(); ++vIter) {
        Id v = vIter->first;
        Ids& adj = vIter->second;
        for(size_t i = 1; i < adj.size(); ++i) {   
            for(size_t j = i + 1; j < adj.size(); ++j) {
                const Plane plane(points[v], points[adj[j]], points[adj[i-1]]);
                if (below(points, plane)) {
                    std::swap(adj[i], adj[j]);
                    break;
                }
            }
        }
        {
            const Plane plane(points[v], points[adj[0]], points[adj.back()]);
            assert(below(points, plane));
        }
    }

    //THINK
    /*
    for(std::map<Id, Ids>::const_iterator vIter = graph.begin(); vIter != graph.end(); ++vIter) {
        Id v = vIter->first;
        const Ids& adj = vIter->second;
        cout << "v = " << v << endl;
        forv(i, adj) {
            cout << " " << adj[i];
        }
        cout << endl;
    }
    */

    /*
    cerr << "START printing convex hull" << endl;
    cerr << convexHull.size() << endl;
    forv(i, convexHull) cerr << " " << convexHull[i];
    cerr << "END printing convex hull" << endl;
    */
    //TODO test
    Ids idMap(points.size());
    forv(i, convexHull) {
        forv(j, points) {
            if (points[j].id() == convexHull[i].id()) {
                idMap[j] = i;
            }
        }
    }
    /*
    cerr << "START printing idMap" << endl;
    forv(i, idMap) {
        cerr << idMap[i];
        if (i + 1 == idMap.size()) cerr << endl;
        else cerr << " ";
    }
    cerr << "DONE printing idMap" << endl;
    */

    //cerr << "START creating polyhedron" << endl;
    *polyhedron = Polyhedron(convexHull);
    for(std::map<Id, Ids>::const_iterator vIter = graph.begin(); vIter != graph.end(); ++vIter) {
        Id v = vIter->first;
        const Ids& adj = vIter->second;
        forv(j, adj) {
            Id u = adj[j];
//            cerr << "START add edge " << endl << idMap[v] << " " << idMap[u] << endl;
            polyhedron->addEdge(idMap[v], idMap[u]);
//            cerr << "DONE add edge" << endl;
        }
    }
    //cerr << "DONE creating polyhedron" << endl;

    {
    /*
        cerr << "START printing polyhedron graph" << endl;
        const ConvexFigure::AdjacencyList& graph = polyhedron->graph();
        forv(v, graph) {
            cerr << "v = " << v << " :";
            forv(j, graph[v]) {
                Id u = graph[v][j];
                cerr << " " << u;
            }
            cerr << endl;
        }
        cerr << "DONE printing" << endl;
    */
    }



}

#endif //CH_SIMPLE_H_INCLUDED