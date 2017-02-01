#include<stdio.h>

void xor(int a, int b)
{
 if (a == b)
 {
   printf("0");
 }
 else
 {
   printf("1");
 }
}

int main()
{
 int a , b;
 printf("\n enter a and b :");
 scanf("%d",&a);
 scanf("%d",&b);
 xor(a, b);
 return 0;
}
