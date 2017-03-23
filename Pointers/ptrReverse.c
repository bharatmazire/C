#include<stdio.h>

int main()
{
 char name[20];
 printf("\n Enter String : ");
 scanf("%s",name);
 char *ptr = name;
 while(*ptr != '\0')
 {
  ptr++;
 }
 while(ptr != name-1)
 {
  printf("%c",*ptr--);
 }
 printf("\n");
 return 0;
}
