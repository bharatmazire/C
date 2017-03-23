#include<stdio.h>

int main()
{
 char a[20];
 printf("Enter The String !!!");
 scanf("%s",a);						// it may work of scanf("%s",&a); but name of array itself is a address of array 
 char *ptr = &a[0]; 				// it may work for char *ptr = a;  coz array name is start address of array
 while (*ptr != '\0')
 {
  printf("%c",*ptr);    
  ptr++;               // or printf("%c",*ptr++);
 }
 return 0;
}
