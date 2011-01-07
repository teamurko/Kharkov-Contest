#include "point.h"
#include "plane.h"
#include "facet.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;
#define forn(i, n) for(size_t i = 0; i < static_cast<size_t>(n); ++i)

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
//    test();
    solve();
    return 0;
}
