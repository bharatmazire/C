#include<stdio.h>

int TwoRaiseTo(int i)
{	int j;
	int num = 1;
	for(j = 0;j < i ; j++)
	{
		num *= 2;
	}
	return num;
}

void Dec(int n)
{
	int i  = 0;
	int sum = 0; 
	
	while (n!=0) 
	{
		if(n % 10 == 1)
		{
			int j = TwoRaiseTo(i);
			sum += j;
		}
		n = n / 10;
		i++;
	}
	printf("\n Decimal number is: %d",sum);
}

int main()
{
	int number;
	printf("\n Enter Binary Number : ");
	scanf("%d",&number);
	Dec(number);
	printf("\n");
	return 0 ;
}
