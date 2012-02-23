#include "../common/geometry.h"
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

Points naiveMinkovskySum(const Points& polyOne, const Points& polyTwo)
{
    Points points;
    points.reserve(polyOne.size() * polyTwo.size());
    forv(i, polyOne) {
        forv(j, polyTwo) {
            points.pb(polyOne[i]);
            points.back().x += polyTwo[j].x;
            points.back().y += polyTwo[j].y;
        }
    }
    return convexHull(points);
}

void normalize(Points& points)
{
    int id = 0;
    forv(i, points) {
        if (lexComp(points[i], points[id])) id = i;
    }
    rotate(points.begin(), points.begin() + id, points.end());
}

void print(const Points& ps)
{
    forv(i, ps) {
        cerr << ps[i].x << " " << ps[i].y << endl;
    }
}

Points minkovskySum(Points polyOne, Points polyTwo)
{
    REQUIRE(!polyOne.empty() && !polyTwo.empty(),
            "Polygons should not be empty.");
    normalize(polyOne);
    normalize(polyTwo);
    Points sum;
    sum.pb(*polyOne.begin());
    sum.back().x += polyTwo.begin()->x;
    sum.back().y += polyTwo.begin()->y;
    int oni = 0, twi = 0;
    int n = polyOne.size();
    int m = polyTwo.size();
    while (true) {
        Vector one(polyOne[oni], polyOne[(oni + 1) % n]);
        Vector two(polyTwo[twi], polyTwo[(twi + 1) % m]);
        ll vp = vectProd(one, two);
        if (vp >= 0) {
            oni = (oni + 1) % n;
        }
        if (vp <= 0) {
            twi = (twi + 1) % m;
        }
        Point np(polyOne[oni].x + polyTwo[twi].x,
                 polyOne[oni].y + polyTwo[twi].y);
        if (np == *sum.begin()) {
            break;
        }
        sum.pb(np);
    }
    return sum;
}

Frac intersect(const Vector& v, const Point& a, const Point& b)
{
    ll d = det(v.x, a.x-b.x, v.y, a.y-b.y);
    if (d == 0) return Frac(1, 0);
    Frac t1(det(a.x, a.x-b.x, a.y, a.y-b.y), d);
    Frac t2(det(v.x, a.x, v.y, a.y), d);
    if (t1.x < 0 || t2.x < 0 || t2.x > t2.y) return Frac(1, 0);
    return t1;
}

Frac min(const Frac& a, const Frac& b)
{
    if (a.x * b.y < b.x * a.y) {
        return a;
    }
    else {
        return b;
    }
}

void readAsteroid(Points& points, Vector& vector)
{
    int n;
    cin >> n;
    points.resize(n);
    forn(i, n) {
        cin >> points[i].x >> points[i].y;
    }
    cin >> vector.x >> vector.y;
}

void printUsage(const char* binary)
{
    cerr << binary << " " << "sol | check" << endl;
}

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }
    Vector one, two;
    Points ao, at;
    readAsteroid(ao, one);
    readAsteroid(at, two);
    forv(i, at) {
        at[i].x = -at[i].x;
        at[i].y = -at[i].y;
    }
    cout.precision(10);
    cout << fixed;

    string action(argv[1]);

    Points sum = minkovskySum(ao, at);
    if (action == "check") {
        REQUIRE(sum == naiveMinkovskySum(ao, at),
            "Polygons sum methods give different results");
    }
    else {
        REQUIRE(action == "sol", "unknown action: sol | check should be "
                                 << "but " << action << " given");
    }

    Vector v(two.x - one.x, two.y - one.y);
    /*
    size_t nonneg = 0, nonpos = 0;
    Point origin;
    forv(i, sum) {
        ll vp = vectProd(v, Vector(origin, sum[i]));
        if (vp >= 0) {
            ++nonneg;
        }
        if (vp <= 0) {
            ++nonpos;
        }
    }
    if (nonneg == 0 || nonpos == 0) {
        cout << "No collision" << endl;
    }
    */
    Frac ans(1, 0);
    forv(i, sum) {
        ans = min(ans, intersect(v, sum[i], sum[(i + 1) % sum.size()]));
    }
    if (ans.y > 0) {
        cout << ans.x << "/" << ans.y << endl;
    } else {
        cout << "No collision" << endl;
    }

    return 0;
}
