#include<stdio.h>

int main()
{
 int a = 5;
 printf("%d",++a);
// printf("%d",++5);       // it will generate error as increment work on lvalue
 return 0;
}
