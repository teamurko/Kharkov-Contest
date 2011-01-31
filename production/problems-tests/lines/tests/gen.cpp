#define _CRT_SECURE_NO_DEPRECATE
#pragma comment (linker, "/STACK:30000000")

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

#define EPS 1E-8
const int INF = (int)1E+9;

#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define forv(i, v) for (int i = 0; i < (int)(v.size()); ++i)
#define fors(i, s) for (int i = 0; i < (int)(s.length()); ++i)
#define all(a) a.begin(), a.end()
#define pb push_back
#define PII pair<int, int>
#define mp make_pair
#define VI vector<int>
#define VS vector<string>

FILE *f = 0;
int tst = 0;

void close() {
    if (f != 0) {
        fclose(f);
        f = 0;
    }
}

FILE *open() {
    close();
    char fname[10];
    sprintf(fname, "%.2d", ++tst);
    cerr << tst << endl;
    return f = fopen(fname, "wt");
}

int random(int value)
{
    return ((((unsigned int)rand()) << 15) ^ ((unsigned int)rand())) % value;
}

typedef vector<long long> point;

point operator + (const point& p1, const point& p2) {
    point p(p1.size());
    forv(i, p) p[i] = p1[i] + p2[i];
    return p;
}

point operator * (const point& p1, long long b) {
    point p(p1.size());
    forv(i, p) p[i] = p1[i] * b;
    return p;
}

point operator - (const point& p1, const point& p2) {
    point p(p1.size());
    forv(i, p) p[i] = p1[i] - p2[i];
    return p;
}

point randPoint(int n, int MAX = 300) {
    point p(n);
    forn(i, n) p[i] = random(MAX) + 1;
    return p;
}

long long scalar(const point& p1, const point& p2) {
    long long res = 0;
    forv(i, p1) res += p1[i] * p2[i];
    return res;
}

long long length2(const point& p1, const point& p2) {
    return scalar(p1 - p2, p1 - p2);
}

void outdata() {
}

void solve() {
}

void outPoint(const point &p) {
    forv(i, p) {
        if (i > 0) fprintf(f, " ");
        fprintf(f, "%d", (int)p[i]);
    }
    fprintf(f, "\n");
}

point perp(const point& v) {
    point r(v.size(), 0);
    int i = 0, j = 0;
    while (i == j) {
        i = random(v.size()), j = random(v.size());
    }
    r[i] = -v[j];
    r[j] = v[i];
    return r;
}

void randomTest(int sn = -1) {
    int n = sn == -1 ? random(50 - 2) + 2 : sn;
    point c1 = randPoint(n), c2 = randPoint(n),
          p1 = randPoint(n), p2 = randPoint(n);
    while (p1 == p2) {
        p1 = randPoint(n), p2 = randPoint(n);
    }
    while (c1 == c2) {
        c1 = randPoint(n), c2 = randPoint(n);
    }
    fprintf(f, "%d\n", n);
    outPoint(c1);
    outPoint(c2);
    outPoint(p1);
    outPoint(p2);
}

void hitrTest(int sn = -1) {
    int n = sn == -1 ? random(50 - 2) + 2 : sn;
    point c1 = randPoint(n), vc = randPoint(n, 30),
          p1 = randPoint(n), vp = vc;
    vc = point(n, 0);
    int x;
    vc[random(n)] = x = random(300) + 1;
    vp = point(n, 0);
    vp[random(n)] = x;
    fprintf(f, "%d\n", n);
    outPoint(c1);
    outPoint(c1 + vc);
    outPoint(p1);
    outPoint(p1 + vp);
}                    

void parallTest(int sn = -1) {
    int n = sn == -1 ? random(50 - 2) + 2 : sn;
    point c1 = randPoint(n), vc = randPoint(n, 30),
          p1 = randPoint(n), vp = vc;
    while (scalar(vc, vc) == 0) {
        vc = randPoint(n);
    }
    vp = vc * ((1 - 2 * random(2)) * (random(30) + 1));
    vc = vc * ((1 - 2 * random(2)) * (random(30) + 1));
    fprintf(f, "%d\n", n);
    outPoint(c1);
    outPoint(c1 + vc);
    outPoint(p1);
    outPoint(p1 + vp);
}

void sovpTest(int sn = -1) {
    int n = sn == -1 ? random(50 - 2) + 2 : sn;
    point c1 = randPoint(n), vc = randPoint(n, 30),
          p1 = randPoint(n), vp = vc;
    while (scalar(vc, vc) == 0) {
        vc = randPoint(n);
    }
    vp = vc * ((1 - 2 * random(2)) * (random(20) + 1));
    p1 = c1 + vc * ((1 - 2 * random(2)) * (random(30) + 1));
    vc = vc * ((1 - 2 * random(2)) * (random(20) + 1));
    fprintf(f, "%d\n", n);
    outPoint(c1);
    outPoint(c1 + vc);
    outPoint(p1);
    outPoint(p1 + vp);
}

void peresekTest(int sn = -1) {
    int n = sn == -1 ? random(50 - 2) + 2 : sn;
    point p1 = randPoint(n), p2 = randPoint(n),
          v = randPoint(n, 20);
    while (p1 == p2 || p1 == (p2 + v)) {
          p1 = randPoint(n), p2 = randPoint(n),
          v = randPoint(n, 20);
    }
    point v1 = v * ((1 - 2 * random(2)) * (random(3) + 1));
    point v2 = p1 + v1 - p2;
    v2 = v2 * ((1 - 2 * random(2)) * (random(3) + 1));
    fprintf(f, "%d\n", n);
    outPoint(p1);
    outPoint(p1 + v1);
    outPoint(p2);
    outPoint(p2 + v2);
}

void readdata() {
    open();
    sovpTest(3);
    parallTest(3);
    randomTest(3);
    peresekTest(3);
    fprintf(f, "%d\n", 0);
    close();
    forn(ii, 9) {
        open();
        forn(i, 10000) {
            int x = random(4);
            if (x == 0) sovpTest(); else
            if (x == 1) parallTest(); else
            if (x == 2) randomTest(); else
            peresekTest();
        }
        fprintf(f, "%d\n", 0);
        close();        
    }
    open();
    forn(i, 10000) {
        hitrTest();
    }
    fprintf(f, "%d\n", 0);
    close();        

}

int main() 
{
    readdata();
    solve();
    outdata();
    close();
    return 0;
}

