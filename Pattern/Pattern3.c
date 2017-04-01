// write a code for accept levels and draw a respetcive pattern
//****
//***
//**
//*
//**
//***
//****

#include<stdio.h>

void DrawOne(int);
void DrawTwo(int);

int main() {
 int NoOfLevel ;
 int symbol;

 printf("\n Enter The Level : ");
 scanf("%d",&NoOfLevel);

 DrawOne(NoOfLevel);
 DrawTwo(NoOfLevel);
 printf("\n----------------------------------------\n");
 DrawTwo(NoOfLevel);
 DrawOne(NoOfLevel);
 return 0;
}

void DrawOne(int num) {
 int i , j;
 for(i = 1; i <= num ; i++)
 {
  for(j = 1 ; j<=i;j++)
  {
   printf("$");
  }
   printf("\n");
 }
}

void DrawTwo(int num) {
 int i , j ;
 for(i = num ; i >0 ; i--)
 {
  for(j = 1 ; j <= i ; j ++)
   {
   printf("$");
 }
  printf("\n");
 }

}
