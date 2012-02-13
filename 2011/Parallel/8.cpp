#include <stdio.h>
#include <math.h>

#define C 10

typedef long double real;
#define real_in "%Lf"
#define real_out "%.18Lf"
#define rabs fabsl
#define rsqrt sqrtl

real vec[C][C], point[C];
int n, m;

int way[C];

int w;
real a[C][C], b[C][C], p[C], q[C];

real sdist;
real cur_koeff[C], koeff[C];
int where_koeff[C];

real calc_vec_slen(real *v)
{
	int i;
	real sd = 0;
	for (i = 0; i < m; i++) sd += v[i] * v[i];
	return sd;
}

void find_dist()
{
	int i, j, k, ii;
	real t, r;
	for (i = 0; i < w; i++) for (k = 0; k < w; k++)
	{
		b[i][k] = 0;
		for (j = 0; j < m; j++) b[i][k] += a[i][j] * a[k][j];		
	}
	for (i = 0; i < w; i++)
	{
		q[i] = 0;
		for (j = 0; j < m; j++) q[i] += a[i][j] * p[j];
	}
	for (k = 0; k < w; k++)
	{
		ii = k;
		for (i = k+1; i < w; i++)
		{
			if (rabs(b[i][k]) > rabs(b[ii][k])) ii = i;
		}
		if (ii > k)
		{
			for (j = 0; j < w; j++)
			{
				t = b[k][j]; b[k][j] = b[ii][j]; b[ii][j] = t;
			}
			t = q[k]; q[k] = q[ii]; q[ii] = t;
		}
		t = b[k][k];
		if (rabs(t) < 1e-20)
		{
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			return;
		}
		t = 1 / t;
		for (j = 0; j < w; j++) b[k][j] *= t;
		q[k] *= t;
		for (i = k+1; i < w; i++)
		{
			t = -b[i][k];
			for (j = 0; j < w; j++) b[i][j] += t * b[k][j];
			q[i] += t * q[k];
		}
	}
	for (k = w-1; k >= 0; k--)
	{
		for (i = 0; i < k; i++) q[i] -= b[i][k] * q[k];
	}
	for (i = 0; i < w; i++)
	{
		if (q[i] < 0) q[i] = 0;
		else if (q[i] > 1) q[i] = 1;
	}
	r = 0;
	for (i = 0; i < m; i++)
	{
		t = -p[i];
		for (j = 0; j < w; j++) t += q[j] * a[j][i];
		r += t*t;
	}
	for (j = 0; j < w; j++) cur_koeff[where_koeff[j]] = q[j];
	if (sdist > r)
	{
		sdist = r;
		for (i = 0; i < n; i++) koeff[i] = cur_koeff[i];
	}
}

int main()
{
	int i, j, k, h, t;
	real g, gg;
	//freopen("input.txt", "rt", stdin);
	scanf("%d%d", &m, &n);
	for (i = 0; i < m; i++) scanf(real_in, &point[i]);
	for (k = 0; k < n; k++) for (i = 0; i < m; i++)
	{
		scanf(real_in, &vec[k][i]);
	}
	sdist = calc_vec_slen(point);
	for (i = 0; i < n; i++) koeff[i] = 0;
	h = 1;
	for (i = 0; i < n; i++) h *= 3;	
	for (j = 0; j < h; j++)
	{
		for (t = 0; t < m; t++) p[t] = -point[t];
		k = j; w = 0;
		for (i = 0; i < n; i++)
		{
			cur_koeff[i] = 0;
			way[i] = k % 3; k /= 3;
			if (way[i] == 2)
			{
				for (t = 0; t < m; t++) a[w][t] = vec[i][t];
				where_koeff[w] = i;
				w++;
			}
			else if (way[i] == 1)
			{
				for (t = 0; t < m; t++)	p[t] -= vec[i][t];
				cur_koeff[i] = 1;
			}
		}
		find_dist();
	}
	printf(real_out"\n", rsqrt(sdist));
	for (i = 0; i < n; i++)
	{
		printf(real_out"%c", koeff[i], (i==n-1) ? '\n' : ' ');
	}
	gg = 0;
	for (i = 0; i < m; i++)
	{
		g = point[i];
		for (j = 0; j < n; j++) g += vec[j][i] * koeff[j];
		gg += g*g;
	}
	printf(real_out"\n", rsqrt(gg));
	return 0;
}
