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
    solve();
    return 0;
}
