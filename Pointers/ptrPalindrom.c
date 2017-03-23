// code to check palindrom of string

#include<stdio.h>

int main() {
 char name[20];
 printf("\n Enter the name to check for palindrom : ");
 scanf("%s",name);
 printf("\n Entered name is : %s \n",name);
 char *ptr1 = name;
 char *ptr2 = name;
 int count = 0;
 while (*ptr2 != '\0')
 {
   count++;
   ptr2++;
 }

 ptr2--;
 int i;
 int flag = 0;
 for (i = 0 ; i < count/2 +1 ; i++)
 {
  if (*ptr1 != *ptr2 )
  {
    printf("\n Not a palindrom !!\n");
    flag = 1;
    break;
  }
 else
  {
   ptr1++;
   ptr2--;
  }
 }
 if (flag == 0)
 {
  printf("\n its a palindrom ! \n");
 }
 return 0;
}
