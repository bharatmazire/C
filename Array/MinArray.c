#include<stdio.h>
#include<stdlib.h>

int Min(int *ptr , int n)
{	
	int i ; 
	for(i = 0 ; i < n ; i ++)
	{
		printf("\n%d th value is %d",i,ptr[i]);
	}
	printf("\n");

	int min = ptr[0];
	for (i = 1 ; i < n ; i ++)
	{
		if(min > ptr[i])
		{
			min = ptr[i];
		}
	}		
	
	return min;
}

int main()
{
	int n ;
	printf("\n Enter the size of array you want :");
	scanf("%d",&n);
	
	int *ptr = malloc(n * sizeof(int));
	
	printf("\n Accepting array values...");
	int i;
	for(i = 0 ; i < n ; i ++)
	{
		printf("\n Enter %dth value ",i);
		scanf("%d",ptr+i);
	}
	
	int result = Min(ptr , n);
	printf("\n Minimul of array is : %d ",result);
	printf("\n");
	free(ptr);
	return 0;
}
