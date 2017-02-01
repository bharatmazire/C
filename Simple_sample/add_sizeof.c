#include<stdio.h>

int main()
{
 int i = 10;
 int j = sizeof(i++);
 printf("%d\n",i);
 printf("%d\n",j);
 return 0;
}
