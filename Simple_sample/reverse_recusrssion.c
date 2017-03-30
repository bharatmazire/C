#include <stdio.h>

void reverse(int num)
{
	if (num>=0)
	{
		printf("%d\n",num);
		reverse(num-1);	
	}
}

int main()
{
	int number;
	printf("\nEnter number : ");
	scanf("%d",&number);

	reverse(number);
	return 0;
}