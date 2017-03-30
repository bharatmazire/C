#include<stdio.h>

int main()
{
 int no;
 printf("\n enter number : ");
 scanf("%d",&no);
 if (no % 2 == 0)
 {
  printf("\n %d  is even\n",no);
 }
 else
 {
  printf("\n %d is odd \n",no);
 }
 return 0;
}
