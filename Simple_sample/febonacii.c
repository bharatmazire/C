#include<stdio.h>

void febonacii(int num)
{
	int num1 = 1 , num2 = 1;
	printf(" %d",num2);
	int new;
	for(int i = 0 ; i < num ; i++)
	{
		
		new = num1 + num2;
		printf(" %d",new);
		int temp = num2;
		num2 = new;
		num1 = temp;
	}
	printf("\n");


}

int main()
{
	int number;
	printf("\nenter number : ");
	scanf("%d",&number);

	febonacii(number);

	return 0;
}