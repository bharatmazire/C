#include<stdio.h>
#include<stdlib.h>

int SecondMin(int * ptr ,int n )
{
	int min1 = ptr[0];
	int min2 = ptr[1];
	int i;
	
	if (min1 < min2 )
	{
		int temp = min1;
		min1 = min2;
		min2 = temp;
	}
	
	for(i = 2 ; i <  n ; i ++)
	{
		if(min1 > ptr[i])
		{
			int temp = min1;
			min1 = ptr[i];
			min2 = temp;
		}
		else if(min2 > ptr[i])
			min2 = ptr[i];
	}
	return min2;
}

int main()
{
	int n , i ; 
	printf("\n Enter size of array : ");
	scanf("%d",&n);
	
	int *ptr = malloc(n * sizeof(int));
	
	for(i = 0 ; i < n ; i ++)
	{
		printf("\n Enter Val : ");
		scanf("%d",ptr+i);
	}
	
	int result = SecondMin(ptr , n );
	printf("\n 2nd minimum is : %d \n",result);
	
	return 0;
}
