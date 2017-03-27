#include<stdio.h>


int ArrMax(int *ptr ,int n)
{
 int max = *ptr;

 int i;
 for(i = 1 ; i < n ; i++)
 {
  if(max < ptr[i])
  {
   max = ptr[i];
  }
 }
 return max;
}

int main()
{
 int a [10];
 int i;
 for(i = 0 ; i < 10 ; i ++)
 {
  printf("Enter Val : ");
  scanf("%d",&a[i]);
 }
 int max = ArrMax(a,10);
 printf("MAX is : %d \n",max);
 return 0;
}
