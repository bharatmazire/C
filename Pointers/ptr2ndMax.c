#include<stdio.h>

int SecondMax(int a[] , int n)
{
 int i;
 int max1 = 0 , max2 = 0;
 max1 = a[0];
 
 for(i = 1 ; i < n ; i++)
 {
  if(a[i] > max1)
  {
 	if(max1 > max2)
 	 max2 = max1; 
    max1 = a[i];
  } 
  else
  {
   if(a[i] > max2)
    max2 = a[i];
  }
 }
 
 return max2;
 
}

int main()
{
 int n;
 printf("\n Enter size of array : ");
 scanf("%d",&n);
 int arr[n];
 int i;
 for (i = 0 ; i < n ; i++)
 {
   printf("Enter Val : ");
   scanf("%d",&arr[i]);
 }
 
 int MaxTwo = SecondMax(arr , n);
 printf("%d",MaxTwo);
 return 0;
}
