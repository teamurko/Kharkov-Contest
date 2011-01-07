#include "point.h"
#include "plane.h"
#include "facet.h"

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

bool isFacet(const Points& points, const Facet& facet) 
{
    Plane plane(points[facet[0]], points[facet[1]], points[facet[2]]);
    forv(i, points) {
        if (plane.signedDistance(points[i]) > constants::EPS) {
            return false;
        }
    }
    return true;
}

Facet findInitialFacetSimple(const Points& points) 
{
    forv(i, points) {
        forn(j, i) {
            forn(k, j) {
                if (isFacet(points, makeFacet(i, j, k))) {
                    return makeFacet(i, j, k);
                }
                if (isFacet(points, makeFacet(j, i, k))) {
                    return makeFacet(j, i, k);
                }
            }
        }
    }
    cerr << "Cannot find initial facet (simple)" << endl;
    assert(false);
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
            if (i == a || i == j) continue;
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

void check(const Points& points, const Facet& facet)
{
    const Plane plane(points[facet[0]], points[facet[1]], points[facet[2]]);
    forv(j, points) {
        if (plane.signedDistance(points[j]) > constants::EPS) {
            cerr << "Not a facet " + facet.str() << endl;
            assert(false);
        }
    }
}

void check(const Points& points, const Facets& facets)
{
    forv(i, facets) {
        check(points, facets[i]);
    }
}

void convexHullWrapping(const Points& points, Facets* facets)
{
    //facets are ordered counter-clockwise, first index is the least
    Facet initialFacet = findInitialFacetSimple(points);
    check(points, initialFacet);
//    cerr << "Initial facet found " << initialFacet << endl;
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
            if (bestIndex == numeric_limits<size_t>::max()) {
                cerr << "Index is not found" << endl;
                assert(false);
            }
            Facet adjFacet = makeFacet(edge.from(), edge.to(), bestIndex);
            if (!used.count(adjFacet)) {
//                cerr << adjFacet << " size " << used.size() << endl;
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
    check(points, answer);
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
