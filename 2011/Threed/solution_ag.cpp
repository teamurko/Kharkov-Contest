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

const int max_n=100*1000;

typedef double D;
const D eps=1e-8;
const D pi=2*asin(1.);
const int maxX=1000*1000;

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
		assert(abs(x)<maxX-eps && abs(y)<maxX-eps);
	}
	void save() {
		printf("%.3lf %.3lf\n",x,y);
	}
	D len2() {
		return sqr(x)+sqr(y);
	}
};

int sgn(D x) {
	if (x>eps) return 1;
	if (x<-eps) return -1;
	return 0;
}

typedef vector<P> vp;

void load(vp &v) {
	int n;
	cin>>n;
	assert(1<=n && n<=max_n);
	v.resize(n);
	forn(i,n) v[i].load();
	double area=0;
	forn(i,n) area+=v[i]*v[(i+1)%n];
	if (area<-eps) reverse(all(v));
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

int main() {
	freopen("threed.in","r",stdin);
	freopen("threed.out","w",stdout);

	vp xy,yz,zx;
	load(xy);
	load(yz);
	load(zx);
	assert(isConvex(xy));
	assert(isConvex(yz));
	assert(isConvex(zx));
	if (checkAll(xy,yz,zx)) cout<<"Yes\n"; else cout<<"No\n";

	return 0;
}
