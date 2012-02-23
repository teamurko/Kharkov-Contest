#include "../common/geometry.h"
#include "testlib.h"
#include <cassert>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <queue>
#include <iostream>
using namespace std;

void printUsage()
{
    cerr << "Usage: binary num-test psize psize2 max-coord" << endl;
}

Point nextPoint(int maxv)
{
    return Point(rnd.next(-maxv, maxv), rnd.next(-maxv, maxv));
}

Point nonZero(int maxv)
{
    Point pt = nextPoint(maxv);
    while (pt.x == 0 && pt.y == 0) {
        pt = nextPoint(maxv);
    }
    return pt;
}

void print(const Points& ps)
{
    cout << ps.size() << endl;
    forv(i, ps) {
        cout << ps[i].x << " " << ps[i].y << endl;
    }
}

// sample 1
void test1()
{
    printf("4\n0 0\n1 0\n1 1\n0 1\n0 0\n3\n2 0\n3 0\n3 1\n-1 0\n");
    exit(0);
}

// sample 2
void test2()
{
    printf("4\n0 0\n1 0\n1 1\n0 1\n-1 1\n4\n-1 3\n0 3\n0 4\n-1 4\n1 -1\n");
    exit(0);
}

// sample 3
void test3()
{
    printf("4\n0 0\n1 0\n1 1\n0 1\n0 0\n3\n2 0\n3 0\n3 1\n0 1\n");
    exit(0);
}


void specTest4()
{
    cout << 3 << endl;
    cout << "10 10\n";
    cout << "20 20\n";
    cout << "10 20\n";
    cout << "20 0\n";
    cout << 3 << endl;
    cout << "100 10\n";
    cout << "110 10\n";
    cout << "110 20\n";
    cout << "10 0\n";
    exit(0);
}

void specTest5()
{
    cout << 3 << endl;
    cout << "10 10\n";
    cout << "20 20\n";
    cout << "10 20\n";
    cout << "20 0\n";
    cout << 3 << endl;
    cout << "100 10\n";
    cout << "110 10\n";
    cout << "110 20\n";
    cout << "-10000 0\n";
    exit(0);
}

void specTest6()
{
    cout << 3 << endl;
    cout << "10 10\n";
    cout << "20 20\n";
    cout << "10 20\n";
    cout << "20 0\n";
    cout << 3 << endl;
    cout << "100 10\n";
    cout << "110 10\n";
    cout << "110 20\n";
    cout << "20 0\n";
    exit(0);
}

void specTest7()
{
    cout << 3 << endl;
    cout << "0 0\n";
    cout << "1 0\n";
    cout << "0 1\n";
    cout << "0 10000\n";
    cout << 3 << endl;
    cout << "2 11\n";
    cout << "1 10\n";
    cout << "2 10\n";
    cout << "0 -10000\n";
    exit(0);
}

void specTest8()
{
    cout << 3 << endl;
    cout << "-1 0\n";
    cout << "0 0\n";
    cout << "-1 1\n";
    cout << "0 0\n";
    cout << 3 << endl;
    cout << "-1 5000\n";
    cout << "0 5000\n";
    cout << "0 10000\n";
    cout << "2 -9999\n";
    exit(0);
}

void specTest9()
{
    cout << 3 << endl;
    cout << "-1 0\n";
    cout << "0 0\n";
    cout << "-1 1\n";
    cout << "0 0\n";
    cout << 3 << endl;
    cout << "-1 5000\n";
    cout << "0 5000\n";
    cout << "0 10000\n";
    cout << "2 -10000\n";
    exit(0);
}

void maxTest20_25()
{
    Points pas(1, Point(1, 0));
    Vector v(1, 1);
    const int maxv = 100 * 1000000;
    while (pas.back().y + v.y <= maxv) {
        pas.pb(pas.back());
        pas.back().x += v.x;
        pas.back().y += v.y;
        ++v.y;
    }
    v.x = -1, v.y = -1;
    while (pas.back().y + v.y > 0) {
        pas.pb(pas.back());
        pas.back().x += v.x;
        pas.back().y += v.y;
        --v.y;
    }
    pas = convexHull(pas);
    cerr << "poly size : " << pas.size() << endl;
    Points sym = pas;
    forv(i, sym) {
        sym[i].x *= -1;
        sym[i].y *= -1;
    }
    print(pas);
    cout << rnd.next(-maxv, maxv) << " " << rnd.next(-maxv, maxv) << endl;
    print(sym);
    cout << rnd.next(-maxv, maxv) << " " << rnd.next(-maxv, maxv) << endl;
    exit(0);
}

int main(int argc, char* argv[])
{
    registerGen(argc, argv);
    if (argc != 5) {
        printUsage();
        return 1;
    }

    int test = atoi(argv[1]);
    switch (test) {
        case 1: test1();
        case 2: test2();
        case 3: test3();
        case 4: specTest4();
        case 5: specTest5();
        case 6: specTest6();
        case 7: specTest7();
        case 8: specTest8();
        case 9: specTest9();
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25: maxTest20_25();
    }

    int psize = atoi(argv[2]);
    int psize2 = atoi(argv[3]);
    int maxv = atoi(argv[4]);

    assert(maxv > 0);

    set<pair<int, int> > points;
    for (int i = 0; i < (psize + psize2) * 10; ++i) {
        pair<int, int> pt(rnd.next(-maxv, maxv), rnd.next(-maxv, maxv));
        while (points.count(pt)) {
            pt = make_pair(rnd.next(-maxv, maxv), rnd.next(-maxv, maxv));
        }
        points.insert(pt);
    }

    vector<pair<int, int> > tmp(all(points));
    Points pts(points.size());

    forv(i, pts) pts[i].x = tmp[i].first, pts[i].y = tmp[i].second;
    shuffle(all(pts));

    Vector vect(rnd.next(1, maxv), rnd.next(-maxv, maxv));
    Points first, second;

    forv(i, pts) {
        ll vp = vectProd(vect, Vector(pts[i].x, pts[i].y));
        if (vp > 0) {
            first.pb(pts[i]);
        } else if (vp < 0) {
            second.pb(pts[i]);
        }
    }

    first = convexHull(first);
    while (psize < first.size()) first.pop_back();

    second = convexHull(second);
    while (psize2 < second.size()) second.pop_back();
    cerr << first.size() << " " << second.size() << endl;

    print(first);
    Point v = nextPoint(maxv);
    cout << v.x << " " << v.y << endl;

    print(second);
    v = nextPoint(maxv);
    cout << v.x << " " << v.y << endl;

    return 0;
}
