#include "../common/geometry.h"
#include "testlib.h"
#include <cassert>
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

void printUsage()
{
    cerr << "Usage: binary num-test num-points absolute-max-value" << endl;
}

void pr(int x, int y)
{
    cout << x << " " << y << endl;
}

void test1()
{
    cout << 4 << endl;
    pr(0, 0);
    pr(10, 0);
    pr(10, 10);
    pr(5, 6);
    exit(0);
}

void test2()
{
    cout << 3 << endl;
    pr(0, 0);
    pr(5, 5);
    pr(10, 10);
    exit(0);
}

void specTest3()
{
    const int n = 10001;
    cout << n << endl;
    forn(i, n) {
        pr(i, i);
    }
    exit(0);
}

//TODO add other hand tests, max tests

void print(const Points& points)
{
    cout << points.size() << endl;
    forv(i, points) {
        cout << points[i].x << " " << points[i].y << endl;
    }
}

void maxTest10()
{
    int maxv = 100000000;
    Points pas(1, Point(-maxv, -maxv));
    Vector v(1, 1);
    while (pas.back().y + v.y <= maxv) {
        pas.pb(pas.back());
        pas.back().x += v.x;
        pas.back().y += v.y;
        ++v.y;
    }
    v.x = -1, v.y = -1;
    while (pas.back().y + v.y > -maxv) {
        pas.pb(pas.back());
        pas.back().x += v.x;
        pas.back().y += v.y;
        --v.y;
    }
    print(pas);
    pas = convexHull(pas);
    while (pas.size() >= 100000) pas.pop_back();
    exit(0);
}

int main(int argc, char* argv[])
{
    registerGen(argc, argv);
    if (argc != 4) {
        printUsage();
        return 1;
    }
    int test = atoi(argv[1]);
    int n = atoi(argv[2]);
    int maxv = atoi(argv[3]);
    assert(maxv > 0);
    switch (test) {
        case 1: test1();
        case 2: test2();
        case 3: specTest3();
        case 10: maxTest10();
    }
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
