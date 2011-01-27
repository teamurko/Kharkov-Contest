#include <iostream>
#include <cstdlib>

using namespace std;


int main(int argc, char * argv[])
{
	if (argc < 5)
	{
		cerr << "USAGE: gen R r d seed";
		exit(1);
	}


	int R, r, d;
	int x, y, z, xx, yy, zz, t;
	R = atoi(argv[1]);
	r = atoi(argv[2]);
	d = atoi(argv[3]);
	srand(atoi(argv[4]));
	if (R == 0)
	{
		R = rand() % 1000 + 1;
		x = rand() % 300 + 301 - 1000;
		y = rand() % 300 + 301 - 1000;
		z = rand() % 300 + 301 - 1000;
		r = rand () % 100 + 1;
		xx = x + rand() % (r + R);
		yy = y + rand() % (r + R);
		zz = z + rand() % (r + R);

	}
	else
	{
		x = 500;
		y = d;
		z = 0;
		xx = 1000;
		yy = 0;
		zz = 0;

		if (rand () % 239 < 120)
		{
			t = x; x = y; y = t;
			t = xx; xx = yy; yy = t;	
		}
		if (rand () % 239 < 120)
		{
			t = x; x = z; z = t;
			t = xx; xx = zz; zz = t;	
		}
		if (rand () % 239 < 120)
		{
			t = z; z = y; y = t;
			t = zz; zz = yy; yy = t;	
		}


	}




	if (abs(x) > 1000 || abs(y) > 1000 ||abs(z) > 1000 ||abs(xx) > 1000 ||abs(yy) > 1000 ||abs(zz) > 1000 ||abs(R) > 1000 || abs(r) > 1000) cerr << "BOTVA!\n";

	cout << R << ' ' << x << ' ' << y << ' ' << z << "\n";
	cout << r << ' ' << xx << ' ' << yy << ' ' << zz << "\n";

	return 0;
}
