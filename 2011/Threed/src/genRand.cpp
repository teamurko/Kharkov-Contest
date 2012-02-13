#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <cassert>
#include <ctime>
#include "testlib.h"

using namespace std;

#define sz(v) ((int) (v).size())
#define all(v) (v).begin(), (v).end()
#define mp make_pair
#define pb push_back
#define forn(i,n) for (int i=0; i<n; i++)

typedef long long ll;
typedef long long int64;
typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<string> vs;

template<typename T> T abs(T x) { return x>0 ? x : -x; }
template<typename T> T sqr(T x) { return x*x;          }

int myRand() {
	return rnd.next((1<<31)-1);
}

const int max_n=100*1000;

typedef double D;
const D eps=1e-8;
const D pi=2*asin(1.);
const int maxX=1000;

struct P {
	D x,y;
	P() {}
	P(D x, D y): x(x), y(y) {}
	P operator -(P a) {
		return P(x-a.x,y-a.y);
	}
	D operator *(P a) {
		return x*a.y-y*a.x;
	}
	D operator ^(P a) {
		return x*a.x+y*a.y;
	}
	D len() {
		return sqrt(sqr(x)+sqr(y));
	}
	void load() {
		scanf("%lf%lf",&x,&y);
	}
	void save() {
		printf("%.3lf %.3lf\n",x,y);
	}
	D len2() {
		return sqr(x)+sqr(y);
	}
};

struct P3 {
	D x,y,z;
	P3(D x=0, D y=0, D z=0): x(x), y(y), z(z) {}
};

int sgn(D x) {
	if (x>eps) return 1;
	if (x<-eps) return -1;
	return 0;
}

typedef vector<P> vp;
typedef vector<P3> vp3;

P C;

bool polarCmp(P a, P b) {
	a=a-C;
	b=b-C;
	D vct=a*b;
	if (abs(vct)>eps) return vct>0;
	return a.len2()<b.len2()-eps;
}

vp convexHull(vp &v) {
	forn(i,sz(v))
		if (v[i].y<v[0].y-eps || (abs(v[i].y-v[0].y)<eps && v[i].x<v[0].x-eps))
			swap(v[0],v[i]);
	C=v[0];
	sort(v.begin()+1,v.end(),polarCmp);
	vp res;
	forn(i,sz(v)) {
		while (sz(res)>1) {
			P last1=res[sz(res)-2];
			P last2=res[sz(res)-1];
			if ((last2-last1)*(v[i]-last1)<eps)
				res.pop_back();
			else
				break;
		}
		res.pb(v[i]);
	}
	return res;
}

void outVP(vp v) {
	cerr<<sz(v)<<endl;
	cout<<sz(v)<<endl;
	forn(i,sz(v)) printf("%.0lf %.0lf\n",v[i].x,v[i].y);
}

vp boundingBox(vp &v) {
	double minX=1e100,minY=1e100,maxX=-1e100,maxY=-1e100;
	forn(i,sz(v)) {
		minX=min(minX,v[i].x);
		maxX=max(maxX,v[i].x);
		minY=min(minY,v[i].y);
		maxY=max(maxY,v[i].y);
	}
	vp res;
	res.pb(P(minX,minY));
	res.pb(P(minX,maxY));
	res.pb(P(maxX,maxY));
	res.pb(P(maxX,minY));
	return res;
}

void genTest(vp3 v) {
	vp xy,yz,zx;
	forn(i,sz(v)) {
		xy.pb(P(v[i].x,v[i].y));
		yz.pb(P(v[i].y,v[i].z));
		zx.pb(P(v[i].z,v[i].x));
	}
	xy=convexHull(xy);
	yz=convexHull(yz);
	zx=convexHull(zx);
	//outVP(boundingBox(xy));
	outVP(xy);
	outVP(yz);
	outVP(zx);
}

vp3 genRandVP3(int n, int maxX) {
	vp3 res;
	forn(i,n) res.pb(P3(myRand()%(2*maxX)-maxX,myRand()%(2*maxX)-maxX,myRand()%(2*maxX)-maxX));
	return res;
}

vp3 genLargeVP3(int n, int maxX) {
	vp3 res;
	int a=maxX-myRand()%100;
	int b=maxX-myRand()%100;
	int c=maxX-myRand()%100;
	forn(i,n) {
		double al=myRand()%1000000/1000000.*2*pi;
		double x=a*cos(al);
		double y=b*sin(al);
		res.pb(P3(int(x),int(y),myRand()%10-5));
	}

	forn(i,n) {
		double al=myRand()%1000000/1000000.*2*pi;
		double x=b*cos(al);
		double y=c*sin(al);
		res.pb(P3(myRand()%10-5,int(x),int(y)));
	}

	forn(i,n) {
		double al=myRand()%1000000/1000000.*2*pi;
		double x=c*cos(al);
		double y=a*sin(al);
		res.pb(P3(int(y),myRand()%10-5,int(x)));
	}
	return res;
}

bool pointInsideTriangle(P p, P q1, P q2, P q3) {
	if (sgn((q2-q1)*(p-q1))*sgn((p-q1)*(q3-q1))<0) return false;
	if (sgn((q3-q2)*(p-q2))*sgn((p-q2)*(q1-q2))<0) return false;
	if (sgn((q1-q3)*(p-q3))*sgn((p-q3)*(q2-q3))<0) return false;
	return true;
}

bool pointInsidePolygon(P p, vp &q) {
	int l=1, r=sz(q);
	while (r-l>1) {
		int m=(l+r)/2;
		if ((p-q[0])*(q[m]-q[0])>0)
			r=m;
		else
			l=m;
	}
	return pointInsideTriangle(p,q[0],q[l],q[(l+1)%sz(q)]);
}

bool polygonInsidePolygon(vp &p, vp &q) {
	forn(i,sz(p)) if (!pointInsidePolygon(p[i],q)) return false;
	return true;
}

double getY(P p1, P p2, D x) {
	D dx=p2.x-p1.x;
	D dy=p2.y-p1.y;
	D d=x-p1.x;
	D t=d/dx;
	return p1.y+dy*t;
}

double getY(vp &xy, int at1, int at2, D x) {
	int maxAdd=at2-at1;
	if (maxAdd<0) maxAdd+=sz(xy);
	int l=0, r=maxAdd;
	while (r-l>1) {
		int m=(l+r)/2;
		int e=(at1+m)%sz(xy);
		if (sgn(xy[e].x-x)*sgn(xy[at1].x)<0)
			r=m;
		else
			l=m;
	}
	int e1=(at1+l)%sz(xy);
	int e2=(at1+l+1)%sz(xy);
	return getY(xy[e1],xy[e2],x);
}

vector<double> getYs(vp &xy, D x) {
	vector<double> res;
	D minX=1e100,maxX=-1e100;
	int atMin=-1,atMax=-1;
	forn(i,sz(xy)) {
		if (abs(xy[i].x-x)<eps)
			res.pb(xy[i].y);
		if (xy[i].x<minX) {
			minX=xy[i].x;
			atMin=i;
		}
		if (xy[i].x>maxX) {
			maxX=xy[i].x;
			atMax=i;
		}
	}
	if (abs(x-minX)<eps) return res;
	if (abs(x-maxX)<eps) return res;
	res.pb(getY(xy,atMin,atMax,x));
	res.pb(getY(xy,atMax,atMin,x));
	return res;
}

vp sym(vp &xy) {
	vp res=xy;
	forn(i,sz(res)) swap(res[i].x,res[i].y);
	return res;
}

bool check(vp &xy, vp &yz, vp &zx) {
	double minY1=1e100,minY2=1e100,maxY1=-1e100,maxY2=-1e100;
	forn(i,sz(xy)) {
		minY1=min(minY1,xy[i].y);
		maxY1=max(maxY1,xy[i].y);
	}
	forn(i,sz(yz)) {
		minY2=min(minY2,yz[i].x);
		maxY2=max(maxY2,yz[i].x);
	}
	if (abs(minY1-minY2)>eps) return false;
	if (abs(maxY1-maxY2)>eps) return false;
	vp ZX;
	forn(i,sz(xy)) {
		vector<D> z=getYs(yz,xy[i].y);
		forn(t,sz(z)) ZX.pb(P(z[t],xy[i].x));
	}
	vp yx=sym(xy);
	forn(i,sz(yz)) {
		vector<D> x=getYs(yx,yz[i].x);
		forn(t,sz(x)) ZX.pb(P(yz[i].y,x[t]));
	}
	ZX=convexHull(ZX);
	return polygonInsidePolygon(zx,ZX);
}

bool checkAll(vp &xy, vp &yz, vp &zx) {
	if (!check(xy,yz,zx)) return false;
	if (!check(yz,zx,xy)) return false;
	if (!check(zx,xy,yz)) return false;
	return true;
}

bool isConvex(vp &v) {
	D sumAngles=0;
	forn(i,sz(v)) {
		P p1=v[i];
		P p2=v[(i+1)%sz(v)];
		P p3=v[(i+2)%sz(v)];
		P v1=p2-p1;
		P v2=p3-p2;
		if (v1.len()<eps) return false;
		if (v2.len()<eps) return false;
		if (v1*v2<eps) return false;
		D c=(v1^v2)/v1.len()/v2.len();
		D an=acos(c);
		sumAngles+=an;
	}
	return abs(sumAngles-2*pi)<1;
}

void genTestNo(vp3 v) {
	vp xy,yz,zx;
	forn(i,sz(v)) {
		xy.pb(P(v[i].x,v[i].y));
		yz.pb(P(v[i].y,v[i].z));
		zx.pb(P(v[i].z,v[i].x));
	}
	xy=convexHull(xy);
	yz=convexHull(yz);
	zx=convexHull(zx);
	for(;;) {
		cerr<<"try\n";
		if (!checkAll(xy,yz,zx)) break;
		int r=myRand()%3;
		vp &v=r==0 ? xy : r==1 ? yz : zx;
		P &p=v[myRand()%sz(v)];
		int dx=myRand()%3-1;
		int dy=myRand()%3-1;
		p.x+=dx;
		p.y+=dy;
		v=convexHull(v);
	}
	outVP(xy);
	outVP(yz);
	outVP(zx);
}

int hash(char *s) {
	int res=0;
	forn(i,strlen(s)) res=res*458489+s[i];
}

int main(int argc, char **argv) {
	registerGen(argc,argv);
	int n,maxX;
	sscanf(argv[1],"%d",&n);
	sscanf(argv[2],"%d",&maxX);
	genTest(genRandVP3(n,maxX));
	return 0;
}