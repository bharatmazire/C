//Write a program which will accept the level and to draw a pattern .
//*
//**
//***
//****
//*****

#include<stdio.h>  // including of standard  lib file for usage of printf and scanf

void DrawPattern1(int); // initilization of function with only one int parameter with void return type i.e. no parameter

int main()  			// start or entry point of code return type is int ; default return type again int
{
 int NoOfLevels;		// initilization of number of levels 
 printf("\n Enter The Number Of Levels You Want : "); 
 scanf("%d",&NoOfLevels); // accepting value for NoOfLevels (& if given coz we have to store that value at address of NoOfLevels)
 DrawPattern1(NoOfLevels); // call to function with parameter NoOfLevels
 return 0;			// as our main is int main so we are returning 0 as successful execution
}

void DrawPattern1(int num)  // actual function defination with int parameter and num value
{
 int i , j;					// initialize the i and j . For pattern as given above we need two loops one for going top to down and 								// one for going left to right 
 for(i = 1 ; i <= num ; i++) // this loop is for going top to down for so we need to print 5 levels (as example u can consider any 								// no. of levels) say 5 now so this loop will wun for 5 times
 							// this loop is just for to place j at next line and in next loop j will print respective *
 {
  for(j = 1 ; j <= i ; j++)	// this loop is to print * now we can see when level is 1 it will print 1 * so we give parameter to loop 								// like from 1 to i i.e. when level will be 1 it will print only 1 *
  							// when level will be 2 i.e. i == 2 : it will print **
  {
   printf("*");
  }
  printf("\n"); 			// this is at the end of i loop means for next * go to next line
 }
}
