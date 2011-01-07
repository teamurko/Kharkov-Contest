/*
Task by Ilya Nikolaevsky
solution by Ilya Nikolaevsky
Numeric integrate method
Main idea: solution depends only on R (radii of planet), r (raddi of beam) and d (distance of beam axis and center of planet)
			then use cylindric coordinate system and get the inner integral to obtain area equation with only one integration.
			then calculate it using Simpson formula. 

*/

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>

const long double eps = 1e-9;
const long double Pi = 3.1415926535897932384626433832795;


#define MAXN 500000

using namespace std;


class Point
{
public:
	long double x, y, z;
	long double len2()
	{
		return x*x + y*y + z*z;
	}
};


Point operator +(Point a, Point b)
{
	Point c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}


Point operator -(Point a, Point b)
{
	Point c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}


long double operator *(Point a, Point b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}




Point dir, pl;
long double R, r, d;

void Load()
{
	cin >> R >> pl.x >> pl.y >> pl.z;
	cin >> r >> dir.x >> dir.y >> dir.z;


	if (fabs(pl.x) > 1000 || fabs(pl.y) > 1000 || fabs(pl.z) > 1000 || fabs(R) > 1000)
	{
		cerr << "INCORRECT TESTS!\n";
	}
	if (fabs(dir.x) > 1000 || fabs(dir.y) > 1000 || fabs(dir.z) > 1000 || fabs(r) > 1000)
	{
		cerr << "INCORRECT TESTS!\n";
	}

	long double a, b, c;
	a = dir.len2();
	b = -2 * (pl * dir);
	c = pl.len2();
	long double t = -b / (2.0*a);          
	d = sqrt(fabs(t*t*a+t*b+c));

//	cerr << R << ' ' << r << ' ' << d << "\n";
}




long double f(long double x)
{
	if (x > R + eps || x < -R - eps) cerr  << "Botva1\n";
	
	if (x > R - eps || x < -R + eps) return Pi/2.0;
	
	long double t = (r*r-(x-d)*(x-d))/(R*R-x*x);

	if (t < -eps) cerr << "Botva2\n";
	
	t = sqrt(fabs(t));
	
	if (fabs(t) > 1 + eps) cerr << "Botva3\n";
	if (t > 1 - eps) return Pi / 2.0;

	return R*asin(t);  // *R
}


long double Integrate(long double a, long double b)
{
	long double p1, p2, p3;
	long double l = b - a;
	long double h = l / MAXN;
	p1 = a;
	p2 = a + h/2.0;
	p3 = a + h;
	long double res = 0;


//	cerr << "integrate from " << a << " to " << b << " got ";

	for (int i = 0; i < MAXN; i++)
	{
		res += f(p1) + 4 * f(p2) + f(p3);
		p1 += h; p2 += h; p3 += h;
	}
	
	res = res * h / 6.0;


//	cerr << res * 4 << "\n";

	return res * 4.0;

}


long double Solve()
{
	long double t;

	if (abs(d) < eps)
	{
		if (r > R - eps)
		{
			return 4*Pi*R*R;
		}
		else
		{
			return Integrate(-r+d, r+d);
		}
	}


	t = (R*R + d*d - r*r) / (2*d);

//	cerr << "t = " << t << "\n";

//	cerr << "0 = " << fabs(R*R - t*t - (r*r- (d-t)*(d-t))) << "\n";

	if (R > r + d - eps || r > R + d -eps || d > R + r - eps) // no intersections
	{
		if (d + R < r + eps) 
		{
			return 4*Pi*R*R;	
		}
		if (d + r < R + eps)
		{
			return Integrate(-r+d, r+d);
		}
		return 0;
	}


	return Integrate(d-r,t) + 2*Pi*R*(R-t);
}



int main()
{
	freopen("masterspark.in", "rt", stdin);
	freopen("masterspark.out", "wt", stdout);

	cout.setf(ios::fixed | ios::showpoint);
	cout.precision(6);

	Load();
	cout << Solve() << "\n";

	return 0;
}                                        
