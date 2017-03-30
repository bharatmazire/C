#include<stdio.h>


int fibo(int num)
{
	if (num == 1|| num == 2)
		return 1;
	return fibo(n-1) + fibo(n-2);

}

int main()
{
	int position;
	printf("\nEnter the number position ");
	scanf("%d",position);
	
	int result = fibo(position);

	printf("fibonacii is : %d",result);

	return 0;
}