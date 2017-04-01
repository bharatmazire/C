#include<stdio.h>
#include<stdlib.h>

void Sqr(int *ptr , int n )
{	
	int i;
	printf("\n Square of entered array is : ");
	for(i = 0 ; i < n ; i ++)
	{
		printf(" %d ",((*ptr+i)* (*ptr+i)));
	}
}

int main()
{
	int n ; 
	printf("\n Enter Size of array you want : ");
	scanf("%d",&n);
	int * ptr = malloc(n * sizeof(int));
	int i;
	for(i = 0; i < n ; i++)
	{
		printf("\n Enter Val : ");
		scanf("%d",ptr+i);
	}
	printf("\n Entered array is : ");
	for(i = 0; i < n ; i++)
	{
		printf(" %d ",*(ptr+i));
	
	}
	Sqr(ptr , n);
	return 0;
}
