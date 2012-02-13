/*
Copy a test (with line feeds fixed)
*/
#include <stdio.h>
#include <string.h>

char buf[1000001];

main()
{
int i;
while (fgets (buf,1000000,stdin)!=NULL)
{
if ((strlen(buf)==1000000)&&(buf[1000000]>=32)) {printf ("%s",buf);continue;}
for (i=strlen(buf)-1;i>=0;i--) {if (buf[i]<=32) buf[i]=0; else break;}
printf ("%s\n",buf);
}
return (0);
}