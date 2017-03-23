#include<stdio.h>

int main()
{
 char name[20];
 printf("Enter String to find length : ");
 scanf("%s",name);
 printf("\nEntered String is : %s \n",name);
 char *ptr = name;
 int count = 0;
 while(*ptr != '\0')
 {
  count++;
  ptr++;
 }
 printf("\n Length is : %d\n",count);
 return 0;
}
