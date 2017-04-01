// Write a c program to accept the levels form user and draw respective pattern
//****
//***
//**
//*

#include<stdio.h>

void DrawPattern2(int);

int main()
{
 int NoOfLevels;
 printf("\n Enter The Number Of Levels : ");
 scanf("%d",&NoOfLevels);
 DrawPattern2(NoOfLevels);
 return 0;
}

void DrawPattern2(int num)
{
 int i , j ;
 for(i = num ; i > 0 ; i--)
 {
  for(j = 1 ; j<=i ; j++)
  {
   printf("*");
  }
  printf("\n");
 }
}
