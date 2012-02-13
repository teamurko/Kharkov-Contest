#include <cstdio>
#include <cassert>
#include <cmath>
#include <algorithm>

using namespace std;

#define Less(a,b,c) ((a)<(b)-eps && (b)+eps<(c))
#define LessEq(a,b,c) ((a)<(b)+eps && (b)-eps<(c))

const int N = 1000;
const int MaxC = 100;
const double inf = 1e9;
const double eps = 1e-6;

struct Point {
    double x, y, z;
    void Init(double xx, double yy, double zz)
    {
        x = xx; y = yy; z = zz;
    }
    Point & Fall(Point & c)
    {
        c.x = x;
        c.y = y;
        c.z = 0;
        return c;
    }
} s, a, b, l, r;

struct Bar {
    Point p[5];
    Point fall[4];
} B[N];

bool Intersect(const Point & p0, const Point & p1, const Point & p2, const Point & pa, const Point & pb, double & t)
{
    double dx1 = p1.x - p0.x;
    double dy1 = p1.y - p0.y;
    double dz1 = p1.z - p0.z;

    double dx2 = p2.x - p0.x;
    double dy2 = p2.y - p0.y;
    double dz2 = p2.z - p0.z;

    double a1 = dy1 * dz2 - dz1 * dy2;
    double a2 = dx1 * dz2 - dz1 * dx2;
    double a3 = dx1 * dy2 - dy1 * dx2;

    double k = (pb.x - pa.x) * a1 - (pb.y - pa.y) * a2 + (pb.z - pa.z) * a3;
    double b = (p0.x - pa.x) * a1 - (p0.y - pa.y) * a2 + (p0.z - pa.z) * a3;

    if (fabs(k) < eps)
        return false;

    t = b / k;
    if (t < -eps)
        return false;
    if (t > 1 + eps)
        return false;

    return true;
}

int n;

double resT, t, h;

int Inside(const Point & a, const Point & l, const Point &r)
{
    if (Less(l.x, a.x, r.x) && Less(l.y, a.y, r.y) && Less(l.z, a.z, r.z))
        return 1;
    if (LessEq(l.x, a.x, r.x) && LessEq(l.y, a.y, r.y) && LessEq(l.z, a.z, r.z))
        return 2;
    return 0;
}

double mint, maxt;

void HitPlane(const Point & p, const Point & q, const Point & r, const Point & a, const Point & b)
{
    double t;
    if (Intersect(p, q, r, a, b, t))
    {
        Point c;
        c.Init(a.x+t*(b.x-a.x), a.y+t*(b.y-a.y), a.z+t*(b.z-a.z));
        if (Inside(c, p, r) != 0)
        {
            mint = min(mint, t);
            maxt = max(maxt, t);
        }
    }
}

bool HitBuilding(const Point & s, const Point & f, const Bar & b, const int KK)
{
    Point temp;
    mint=inf;
    maxt=-inf;
    HitPlane(b.fall[0], b.p[0], b.p[1], s, f);
    HitPlane(b.fall[0], b.p[0], b.p[3], s, f);
    HitPlane(b.fall[1], b.p[1], b.p[2], s, f);
    HitPlane(b.fall[3], b.p[3], b.p[2], s, f);
    HitPlane(b.p[0], b.p[1], b.p[2], s, f);

    temp.x = s.x + (f.x - s.x) * (maxt + mint) / 2;
    temp.y = s.y + (f.y - s.y) * (maxt + mint) / 2;
    temp.z = s.z + (f.z - s.z) * (maxt + mint) / 2;

    if (Inside(temp, b.fall[0], b.p[2]) == KK)
        return true;
    return false;
}

void TryShoot(double t)
{
    Point c;
    c.Init(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y), a.z + t * (b.z - a.z));

    int i;
    for (i = 0; i < n && !HitBuilding(s, c, B[i], 1); ++i);

    if (i == n && resT > t)
        resT = t;
}

int main()
{
    scanf("%lf%lf%lf", &s.x, &s.y, &s.z);
    assert(abs(s.x) <= MaxC && abs(s.y) <= MaxC && 0 <= s.z && s.z <= MaxC);

    scanf("%lf%lf", &a.x, &a.y);
    assert(abs(a.x) <= MaxC && abs(a.y) <= MaxC);

    scanf("%lf%lf", &b.x, &b.y);
    assert(abs(b.x) <= MaxC && abs(b.y) <= MaxC);

    a.z = b.z = 0;
    
    scanf("%d", &n);
    assert(0 <= n  && n <= N);
    for (int i = 0; i < n; ++i)
    {
        scanf("%lf%lf%lf%lf%lf", &l.x, &l.y, &r.x, &r.y, &h);

        assert(abs(l.x) <= MaxC && abs(l.y) <= MaxC);
        assert(abs(r.x) <= MaxC && abs(r.y) <= MaxC);
        assert(l.x < r.x && l.y < r.y);
        assert(0 < h && h <= MaxC);

        B[i].p[0].Init(l.x, l.y, h);
        B[i].p[1].Init(r.x, l.y, h);
        B[i].p[2].Init(r.x, r.y, h);
        B[i].p[3].Init(l.x, r.y, h);
        B[i].p[4] = B[i].p[0];
        for (int j = 0; j < 4; ++j)
            B[i].fall[j].Init(B[i].p[j].x, B[i].p[j].y, -1);
    }

    for (int i = 0; i < n; ++i)
    {
        assert(Inside(s, B[i].fall[0], B[i].p[2]) == 0);
        assert(Inside(a, B[i].fall[0], B[i].p[2]) == 0);
        assert(Inside(b, B[i].fall[0], B[i].p[2]) == 0);

        for (int j = i+1; j < n; ++j)
           assert(max(B[i].p[0].x, B[j].p[0].x) > min(B[i].p[2].x, B[j].p[2].x)
               || max(B[i].p[0].y, B[j].p[0].y) > min(B[i].p[2].y, B[j].p[2].y));
    }

    for (int i = 0; i < n; ++i)
    {
        assert(!HitBuilding(a, b, B[i], 2));
    }

    resT = inf;

    TryShoot(0);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (Intersect(s, B[i].p[j], B[i].p[j+1], a, b, t) && t < resT)
                TryShoot(t);
            if (Intersect(s, B[i].fall[j], B[i].p[j], a, b, t) && t < resT)
                TryShoot(t);
        }
    }

    if (fabs(resT - inf) < eps)
        printf("Impossible\n");
    else
        printf("%.9lf %.9lf\n", a.x+resT*(b.x-a.x), a.y+resT*(b.y-a.y));
    return 0;
}
