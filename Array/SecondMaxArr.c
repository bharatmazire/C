#include<stdio.h>
#include<stdlib.h>

int SecondMax(int *ptr , int n)
{
	int max1 = ptr[0];
	int max2 = ptr[1];
	int i;
	if( max2 > max1 )
	{
		int temp = max2;
		max2 = max1;
		max1 = temp;
	}
	
	for(i = 2 ; i < n ; i ++)
	{
		if(max1 < ptr[i])
		{	
			int temp = max1;
			max1 = ptr[i];
			max2 = temp;
		}
	}
	return max2;	
}

int main()
{
	int n , i ; 
	printf("\n Enter Size of array : ");
	scanf("%d",&n);
	
	int *ptr = malloc(n * sizeof(int));
	
	for(i = 0 ; i < n ; i ++)
	{
		printf("\n Enter Val : ");
		scanf("%d",ptr+i);
	}
	int result = SecondMax(ptr , n );
	printf("\n 2nd minimum is : %d \n",result);

}
