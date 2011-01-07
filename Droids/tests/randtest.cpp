#include <stdio.h>
#include <stdlib.h>

int main ()
{
 int n;
 scanf ("%d", &n);
 printf ("%d\n", n);
 for (int i = 0; i < n; ++i)
  printf ("%d %d %d\n", rand () % 100, rand () % 100, rand () % 100);
 return 0;
}
