#include<stdio.h>

void check(char);

int main()
{
 char ch;
 printf("\n enter charachter : ");
 scanf("%c",&ch);
 check(ch);
 return 0;
}

void check(char ch)
{
 if (ch == 'a')
 	printf("\n vowel\n");
 else if (ch == 'e')
 	printf("\n vowel\n");
 else if (ch == 'i')
 	printf("\n vowel\n");
 else if (ch == 'o')
 	printf("\n vowel\n");
 else if (ch == 'u')
 	printf("\n vowel\n");
 else
 	printf("\n not vowel \n");
}
