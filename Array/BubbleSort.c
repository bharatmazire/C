#include<stdio.h>
#include<stdlib.h>

int * Sort(int *ptr , int n)
{
	int i;
 	for (int j = 0 ; j < n-1 ; j ++)
 	{
  		int max = ptr[0];
  		int fl ;
  		printf("\n");
 		for(i = 0 ; i < n ; i++)
 		{
  			printf("%d ",ptr[i]);
 		}
 		for (i = 0; i < n-j ; i++)
 		{
  			printf("\nmax is %d and ptr[%d] is %d",max,i,ptr[i]);
  			if (max < ptr[i])
   			{
   				max = ptr[i];
   				fl = i;
   			}
 		}
 		int m = n-j-1;
 		ptr[fl] = ptr[m];
 		ptr[m] = max;
 		printf("\n\tptr[0] %d  and ptr[%d]%d\n",ptr[0],m,ptr[m]);
	}	 
 	return ptr;
}


int main()
{
	int n;
 	printf("\n Enter Size of Array you want :");
 	scanf("%d",&n);
 	int * ptr = malloc(n*sizeof(int));
 	int i;
 	for(i = 0 ; i < n ; i++)
 	{
  		printf("\nEnter %d th value : ",i);
  		scanf("%d",ptr+i);
 	}
 	printf("\n Entered array is : ");
  	for(i = 0 ; i < n ; i++)
 	{
  		printf("%d ",ptr[i]);
 	}
 
 	int * ptr2 = Sort(ptr,n);
 	printf("\n sorted array is : ");
 	for(i = 0 ; i < n ; i++)
 	{
  		printf("%d ",ptr2[i]);
 	}
 	free(ptr);
 	return 0;
}
