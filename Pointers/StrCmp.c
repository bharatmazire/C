// Write a program to accept two strings and compaire it without using inbuilt function

#include<stdio.h>

void check(char *ptr1 , char *ptr2)
{	
	int flag = 0;
	
	while(*ptr1+*ptr2)
	{
		if(*ptr1 != *ptr2)
		{	flag = 1;
			break;
		}
		ptr1++;
		ptr2++;
	}
	if(flag == 1)
		printf("\n not same \n");
	else
		printf("\n same \n");
}

int main()
{
	char str1[20] , str2[20];
	printf("\n Enter 1st String : ");
	scanf("%s",str1);
	printf("\n Enter 2nd String : ");
	scanf("%s",str2);
	char *ptr1 = str1;
	char *ptr2 = str2;
	check(ptr1 , ptr2);
	return 0;
}
