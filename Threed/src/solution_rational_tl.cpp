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

typedef long long int64;
typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<string> vs;

template<typename T> T sqr(T x) { return x*x;          }

const int max_n=6*1000;
const int maxlength=20;

class bigint
{
public:
	int oper,length,a[maxlength];
	bigint(int=0);
	~bigint();
	int max(int a,int b);
	void check();
	void operator=(bigint m);
	void operator=(int m);
	void operator=(char *s);
	bool operator<(bigint m);
	bool operator<=(bigint m);
	bool operator>(bigint m);
	bool operator>=(bigint m);
	bool operator==(bigint m);
	bool operator!=(bigint m);
	bigint operator-();
	bigint operator+(bigint m);
	void operator+=(bigint m);
	bigint operator-(bigint m);
	void operator-=(bigint m);
	bigint operator*(bigint m);
	bigint operator*(int m);
	void operator*=(bigint m);
	void operator*=(int m);
	bigint operator/(bigint m);
	bigint operator/(int m);
	void operator/=(bigint m);
	void operator/=(int m);
	bigint operator%(bigint m);
	bigint operator%(int m);
	void operator%=(bigint m);
	void operator%=(int m);
};
bigint abs(bigint m);
bool read(bigint &m);
void write(bigint m);
void swrite(char *s,bigint m);
void writeln(bigint m);
bigint sqr(bigint m);
bigint sqrt(bigint m);
bigint gcd(bigint a,bigint b);
bigint lcm(bigint a,bigint b);

int bigint::max(int a,int b)
{
	return (a>b)?a:b;
}
bigint::bigint(int v)
{
	(*this)=v;
	this->check();
}
bigint::~bigint()
{
}
void bigint::check()
{
	for (;length>0 && a[length]==0;length--);
	if (length==0)
		oper=1;
}
void bigint::operator=(bigint m)
{
	oper=m.oper;
	length=m.length;
	memcpy(a,m.a,maxlength*sizeof(int));
	this->check();
}
void bigint::operator=(int m)
{
	oper=(m>0)?1:-1;
	m=abs(m);
	memset(a,0,maxlength*sizeof(int));
	for (length=0;m>0;m=m/10000)
		a[++length]=m%10000;
	this->check();
}
void bigint::operator=(char *s)
{
	int i,L;
	(*this)=0;
	if (s[0]=='-' || s[0]=='+')
	{
		if (s[0]=='-')
			oper=-1;
		L=strlen(s);
		for (i=0;i<L;i++)
			s[i]=s[i+1];
	}
	L=strlen(s);
	length=(L+3)/4;
	for (i=0;i<L;i++)
		a[(L-i+3)/4]=a[(L-i+3)/4]*10+(s[i]-48);
	this->check();
}
bool bigint::operator<(bigint m)
{
	if (oper!=m.oper)
		return oper<m.oper;
	if (length!=m.length)
		return oper*length<m.length*oper;
	for (int i=length;i>=1;i--)
		if (a[i]!=m.a[i])
			return a[i]*oper<m.a[i]*oper;
	return false;
}
bool bigint::operator<=(bigint m)
{
	return !(m<(*this));
}
bool bigint::operator>(bigint m)
{
	return m<(*this);
}
bool bigint::operator>=(bigint m)
{
	return !((*this)<m);
}
bool bigint::operator==(bigint m)
{
	return (!((*this)<m)) && (!(m<(*this)));
}
bool bigint::operator!=(bigint m)
{
	return ((*this)<m) || (m<(*this));
}
bigint bigint::operator-()
{
	bigint c=(*this);
	c.oper=-c.oper;
	c.check();
	return c;
}
bigint abs(bigint m)
{
	bigint c=m;
	c.oper=abs(c.oper);
	c.check();
	return c;
}
bigint bigint::operator+(bigint m)
{
	if (m.length==0)
		return (*this);
	if (length==0)
		return m;
	if (oper==m.oper)
	{
		bigint c;
		c.oper=oper;
		c.length=max(length,m.length)+1;
		for (int i=1,temp=0;i<=c.length;i++)
			c.a[i]=(temp=(temp/10000+a[i]+m.a[i]))%10000;
		c.check();
		return c;
	}
	return (*this)-(-m);
}
bigint bigint::operator-(bigint m)
{
	if (m.length==0)
		return (*this);
	if (length==0)
		return (-m);
	if (oper==m.oper)
	{
		bigint c;
		if (abs(*this)>=abs(m))
		{
			c.oper=oper;
			c.length=length;
			for (int i=1,temp=0;i<=length;i++)
				c.a[i]=((temp=(-int(temp<0)+a[i]-m.a[i]))+10000)%10000;
			c.check();
			return c;
		}
		return -(m-(*this));
	}
	return (*this)+(-m);
}
bool read(bigint &m)
{
	char s[maxlength*4+10];
	if (scanf("%s",&s)==-1)
		return false;
	for (int i=0;s[i];i++)
		if (!(s[i]>='0' && s[i]<='9' || (s[i]=='+' || s[i]=='-') && i==0))
			return false;
	m=s;
	return true;
}
void swrite(char *s,bigint m)
{
	int L=0;
	if (m.oper==-1)
		s[L++]='-';
	sprintf(s+L,"%d",m.a[m.length]);
	for (;s[L]!=0;L++);
	for (int i=m.length-1;i>=1;i--)
	{
		sprintf(s+L,"%04d",m.a[i]);
		L+=4;
	}
	s[L]=0;
}
void write(bigint m)
{
	if (m.oper==-1)
		printf("-");
	printf("%d",m.a[m.length]);
	for (int i=m.length-1;i>=1;i--)
		printf("%04d",m.a[i]);
}
void writeln(bigint m)
{
	write(m);
	printf("\n");
}
bigint bigint::operator*(bigint m)
{
	bigint c;
	c.oper=oper*m.oper;
	c.length=length+m.length;
	for (int i=1;i<=m.length;i++)
	{
		int number=m.a[i],j,temp=0;
		for (j=1;j<=length;j++)
			c.a[i+j-1]+=number*a[j];
		if (i%10==0 || i==m.length)
			for (j=1;j<=c.length;j++)
				c.a[j]=(temp=(temp/10000)+c.a[j])%10000;
	}
	c.check();
	return c;
}
bigint bigint::operator*(int m)
{
	if (m<0)
		return -((*this)*(-m));
	if (m>100000)
		return (*this)*bigint(m);
	bigint c;
	c.length=length+2;
	c.oper=oper;
	int t=0;
	for (int i=1;i<=c.length;i++)
		c.a[i]=(t=(t/10000+a[i]*m))%10000;
	c.check();
	return c;
}
bigint bigint::operator/(bigint m)
{
	if (m.length==0)
	{
		printf("Division by zero.\n");
		exit(0);
	}
	if (abs(*this)<abs(m))
		return 0;
	bigint c,left;
	c.oper=oper/m.oper;
	m.oper=1;
	c.length=length-m.length+1;
	left.length=m.length-1;
	memcpy(left.a+1,a+length-left.length+1,left.length*sizeof(int));
	for (int i=c.length;i>=1;i--)
	{
		left=left*10000+a[i];
		int head=0,tail=10000,mid;
		while (head+1<tail)
		{
			mid=(head+tail)/2;
			if (m*mid<=left)
				head=mid;
			else
				tail=mid;
		}
		c.a[i]=head;
		left-=m*head;
	}
	c.check();
	return c;
}
bigint bigint::operator/(int m)
{
	if (m<0)
		return -((*this)/(-m));
	if (m>100000)
		return (*this)/bigint(m);
	bigint c;
	c.oper=oper;
	c.length=length;
	int t=0;
	for (int i=c.length;i>=1;i--)
		c.a[i]=(t=(t%m*10000+a[i]))/m;
	c.check();
	return c;
}
bigint bigint::operator %(bigint m)
{
	return (*this)-((*this)/m)*m;
}
bigint bigint::operator%(int m)
{
	if (m<0)
		return -((*this)%(-m));
	if (m>100000)
		return (*this)%bigint(m);
	int t=0;
	for (int i=length;i>=1;i--)
		t=(t*10000+a[i])%m;
	return t;
}
bigint sqr(bigint m)
{
	return m*m;
}
bigint sqrt(bigint m)
{
	if (m.oper<0 || m.length==0)
		return 0;
	bigint c,last,now,templast;
	c.length=(m.length+1)/2;
	c.a[c.length]=int(sqrt((double)m.a[c.length*2]*10000+m.a[c.length*2-1])+1e-6);
	templast.length=c.length*2;
	templast.a[c.length*2-1]=(c.a[c.length]*c.a[c.length])%10000;
	templast.a[c.length*2]=(c.a[c.length]*c.a[c.length])/10000;
	templast.check();
	for (int i=c.length-1;i>=1;i--)
	{
		last=templast;
		int head=0,tail=10000,mid,j,temp;
		while (head+1<tail)
		{
			mid=(head+tail)/2;
			now=last;
			now.a[2*i-1]+=mid*mid;
			for (j=i+1;j<=c.length;j++)
				now.a[i+j-1]+=mid*c.a[j]*2;
			now.length++;
			for (j=2*i-1,temp=0;j<=now.length;j++)
				now.a[j]=(temp=(temp/10000+now.a[j]))%10000;
			now.check();
			if (now<=m)
			{
				templast=now;
				head=mid;
			}
			else
				tail=mid;
		}
		c.a[i]=head;
	}
	c.check();
	return c;
}
bigint gcd(bigint a,bigint b)
{
	return (b==0)?a:gcd(b,a%b);
}
bigint lcm(bigint a,bigint b)
{
	return a*b/gcd(a,b);
}
void bigint::operator+=(bigint m)
{
	(*this)=(*this)+m;
}
void bigint::operator-=(bigint m)
{
	(*this)=(*this)-m;
}
void bigint::operator*=(bigint m)
{
	(*this)=(*this)*m;
}
void bigint::operator/=(bigint m)
{
	(*this)=(*this)/m;
}
void bigint::operator%=(bigint m)
{
	(*this)=(*this)%m;
}
void bigint::operator*=(int m)
{
	(*this)=(*this)*m;
}
void bigint::operator/=(int m)
{
	(*this)=(*this)/m;
}
void bigint::operator%=(int m)
{
	(*this)=(*this)%m;
}

typedef bigint ll;
ll zero;

struct D {
	ll nom, den;
	void norm() { if (den<0) nom*=-1, den*=-1; }
	D(ll nom, ll den): nom(nom), den(den) { norm(); }
//	double toDouble() { return double(nom)/double(den); }
	D(ll nom=0): nom(nom), den(1) {}
	D operator *(D a) { return D(nom*a.nom, den*a.den);	}
	D operator /(D a) {	return D(nom*a.den, den*a.nom);	}
	D operator +(D a) {	return D(nom*a.den+den*a.nom,den*a.den); }
	D operator -(D a) {	return D(nom*a.den-den*a.nom,den*a.den); }
	bool operator <(D a) { return nom*a.den<den*a.nom; }
	bool operator >(D a) { return nom*a.den>den*a.nom; }
	bool operator ==(D a) { return nom*a.den==den*a.nom; }
	bool operator !=(D a) { return nom*a.den!=den*a.nom; }
	bool operator <=(D a) { return nom*a.den<=den*a.nom; }
//	void saveDouble() { cerr<<toDouble()<<endl; }
//	void save() { cerr<<"nom="<<nom<<" den="<<den<<endl; }
	void load() {
		int x;
		scanf("%d",&x);
		nom=x;
		den=1;
	}
//	double d;
//	void load() { scanf("%lf",&d); }
//	D(double d=0): d(d) {}
//	D(ll a, ll b) { d=double(a)/double(b); }
//	D operator *(D a) { return D(d*a.d); }
//	D operator /(D a) { return D(d/a.d); }
//	D operator +(D a) { return D(d+a.d); }
//	D operator -(D a) { return D(d-a.d); }
//	bool operator <(D a) { return d<a.d-1e-8; }
//	bool operator >(D a) { return d>a.d-1e-8; }
//	bool operator ==(D a) { return abs(d-a.d)<1e-8; }
//	bool operator <=(D a) { return d<a.d+1e-8; }
//	bool operator !=(D a) { return abs(d-a.d)>1e-8; }
//	double toDouble() { return d; }
//	void save() { cerr<<d; }
};

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
	void load() {
		x.load();
		y.load();
	}
	void save() {
		cerr<<"x=";
//		x.save();
		cerr<<" y=";
//		y.save();
		cerr<<endl;
	}
	D len2() {
		return sqr(x)+sqr(y);
	}
};

int sgn(D x) {
	if (x>zero) return 1;
	if (x<zero) return -1;
	return 0;
}

typedef vector<P> vp;

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
		if ((p-q[0])*(q[m]-q[0])>zero)
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
	if (vct!=zero) return vct>zero;
	return a.len2()<b.len2();
}

vp convexHull(vp &v) {
	forn(i,sz(v))
		if (v[i].y<v[0].y || (v[i].y==v[0].y && v[i].x<v[0].x))
			swap(v[0],v[i]);
	C=v[0];
	sort(v.begin()+1,v.end(),polarCmp);
	vp res;
	forn(i,sz(v)) {
		while (sz(res)>1) {
			P last1=res[sz(res)-2];
			P last2=res[sz(res)-1];
			if ((last2-last1)*(v[i]-last1)<=zero)
				res.pop_back();
			else
				break;
		}
		res.pb(v[i]);
	}
	return res;
}

void load(vp &v) {
	int n;
	cin>>n;
	assert(1<=n && n<=max_n);
	v.resize(n);
	forn(i,n) v[i].load();
	v=convexHull(v);
}

D getY(P p1, P p2, D x) {
	D dx=p2.x-p1.x;
	D dy=p2.y-p1.y;
	D d=x-p1.x;
	D t=d/dx;
	return p1.y+dy*t;
}

D getY(vp &xy, int at1, int at2, D x) {
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

vector<D> getYs(vp &xy, D x) {
	vector<D> res;
	D minX=xy[0].x,maxX=xy[0].x;
	int atMin=0,atMax=0;
	forn(i,sz(xy)) {
		if (xy[i].x==x)
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
	if (x==minX || x==maxX) return res;
	res.clear();
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
	D minY1=xy[0].y,minY2=yz[0].x,maxY1=xy[0].y,maxY2=yz[0].x;
	forn(i,sz(xy)) {
		if (xy[i].y<minY1) minY1=xy[i].y;
		if (xy[i].y>maxY1) maxY1=xy[i].y;
	}
	forn(i,sz(yz)) {
		if (yz[i].x<minY2) minY2=yz[i].x;
		if (yz[i].x>maxY2) maxY2=yz[i].x;
	}
	if (minY1!=minY2 || maxY1!=maxY2) return false;
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

int main() {
	freopen("threed.in","r",stdin);
	freopen("threed.out","w",stdout);

	vp xy,yz,zx;
	load(xy);
	load(yz);
	load(zx);
	if (checkAll(xy,yz,zx)) cout<<"Yes\n"; else cout<<"No\n";

	return 0;
}
