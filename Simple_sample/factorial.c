#include<stdio.h>

int fact(int num)
{
	if (num == 0)
		return 1;
	if (num == 1)
	{
		return num;
	}
	else
	{
		num = num * fact(num-1);
	}

}


// this is another way of recurssion w=to write factorial program.... dhinckak dichak.
int fac(int num)
{

	if(num == 0 || num == 1)
	{
		return 1;
	}
	else
	{
		return num * fac(num-1);
	}
}


int facto(int num)				// its without recurssion
{
	int (no == 0 || no == 1)
	{
		return 1;
	}
	int res == 1;

	for (int i = 2; i < no; i++)
	{
		res = res*i
	}
	return res

}

int main()
{
	int number ; 

	// number acceptace stage
	printf("\nenter number to find factorial : ");
	scanf("%d",&number);
	
	//number validation stage
	if (number < 0)
	{
		printf("\nwrong number entered \n");
		return 0;
	}
	
	//call to processing
	int factorial = fact(number);
	
	//output display
	printf("factorial is : %d \n",factorial);
	return 0;

}