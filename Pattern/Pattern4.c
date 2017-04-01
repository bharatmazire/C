/*
draw pattern
   *
  ***
 *****
*******
*/

#include<stdio.h>

void Draw(int , int );
void DrawRev(int , int);

int main()
{
 int NoLevel;

 printf("\n Enter no of levels : ");
 scanf("%d",&NoLevel);
 int count = NoLevel;
 int j = 1;
 for(int i = 0 ; i<NoLevel ; i++)
 {
  Draw(j,--count);
  j = j+2;
 }
 j = j-2;
 for(int i = 0;i<NoLevel;i++)
 {
 	j = j-2;
 	DrawRev(j,++count);
 }
 return 0;
}

void Draw(int val, int count)
{
 for(int i = 0;i<count; i++)
 {
  printf(" ");
 }
 for(int i = 0;i<val ; i++)
 {
  printf("*");
 }
  printf("\n");
}

void DrawRev(int val, int count)
{
  for(int i = 0;i<count; i++)
 {
  printf(" ");
 }
 for(int i = 0;i<val ; i++)
 {
  printf("*");
 }
  printf("\n");
}
