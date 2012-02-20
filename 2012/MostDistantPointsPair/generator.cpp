#include "testlib.h"
#include <cassert>
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

#define all(v) (v).begin(), (v).end()
#define forn(i, n) for (int i = 0; i < int(n); ++i)

void printUsage()
{
    cerr << "Usage: binary num-points absolute-max-value" << endl;
}

int main(int argc, char* argv[])
{
    registerGen(argc, argv);
    if (argc != 3) {
        printUsage();
        return 1;
    }
    int n = atoi(argv[1]);
    int maxv = atoi(argv[2]);
    assert(maxv > 0);
    cout << n << endl;
    set<pair<int, int> > points;
    for (int i = 0; i < n; ++i) {
        pair<int, int> pt(rnd.next(-maxv, maxv), rnd.next(-maxv, maxv));
        while (points.count(pt)) {
            pt = make_pair(rnd.next(-maxv, maxv), rnd.next(-maxv, maxv));
        }
        points.insert(pt);
    }
    vector<pair<int, int> > pts(all(points));
    shuffle(all(pts));
    assert(pts.size() == n);
    forn(i, n) {
        cout << pts[i].first << " " << pts[i].second << endl;
    }

    return 0;
}
