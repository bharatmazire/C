#include<stdio.h>

int CountOneInNumber(int);
int CountZeroInNumber(int);



int main()
{
	int number , resultOn , resultOff;
	printf("\n Enter the number : ");
	scanf("%d",&number);
	
	resultOn = CountOneInNumber(number);
	
	printf("\n Number of One in %d are %d\n",number,resultOn);

	resultOff = CountZeroInNumber(number);
	
	printf("\n Number of Zero in %d are %d\n",number,resultOff);

}

int CountOneInNumber(int number)
{
	int x = 1;
	int count = 0;
	
	if(number)
	{
		while(x < number)
		{
			if(number & x)
				count ++;
			x = x<<1;
		}
	}
	
	return count;
}

int CountZeroInNumber(int number)
{
	int x = 1;
	int count = 0;

	while(x)
	{
		if((number & x) == 0)
			count ++;
		x = x<<1;
	}

	return count;
}
