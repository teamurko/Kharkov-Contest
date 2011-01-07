#include "..\correct.h"
#include <vector>
using namespace std;

const int MAXN = 100;
const int MAXC = 100;

int xs[MAXN], ys[MAXN], zs[MAXN];

int main()
{
	InputStream in("droids.in");
	int n = in.ReadInt();
	in.Assert(2<=n && n<=MAXN);
	in.RequireEoln(false);
	for (int i=0; i<n; i++){
		xs[i] = in.ReadInt();
		in.RequireSpace();
		ys[i] = in.ReadInt();
		in.RequireSpace();
		zs[i] = in.ReadInt();
		in.Assert(-MAXC<=xs[i] && xs[i]<=MAXC &&
			  -MAXC<=ys[i] && ys[i]<=MAXC &&
			  -MAXC<=zs[i] && zs[i]<=MAXC);
		for (int j=0; j<i; j++){
			in.Assert(xs[i]!=xs[j] || ys[i]!=ys[j] || zs[i]!=zs[j]);
		}
		in.RequireEoln(false);
	} 
	in.RequireEof(false);
	fprintf(stderr, "correct ");

	return 0;
}

