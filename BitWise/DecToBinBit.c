#include<stdio.h>

void Bin(int n)
{
	int arr[32];
	int i;
	for(i = 0 ; i < 32 ; i ++)
	{
		arr[i] = n & 1;
		n = n >> 1;
	}
	printf("\n");
	for(i = 31 ; i >=0 ; i--)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
}

int main()
{
	int number;
	printf("\n Enter Number : ");
	scanf("%d",&number);
	Bin(number);
	return 0;
}
