#include<stdio.h>

int TurnOffBits(int,int,int);       // prototype and funtion declaration ...
				    // no need to give variable name as their ';'

int TurnOnBits(int,int,int);

int ToggleBits(int,int,int);

int main()
{
 int number, position, no_of_bits;
 printf("\n enter number , position and number of bits to apply change ....");
 scanf("%d %d %d",&number,&position,&no_of_bits);
 printf("%d %d %d ",number,position,no_of_bits);

 int offResult = TurnOffBits(number,position,no_of_bits);
 printf("\n After turning of %d bits from position %d of %d is %d \n\n",no_of_bits,position,number,offResult);


 int onResult = TurnOnBits(number,position,no_of_bits);
 printf("\n After turning of %d bits from position %d of %d is %d \n\n",no_of_bits,position,number,onResult);

 int toggleResult = ToggleBits(number,position,no_of_bits);
 printf("\n After toggle of %d bits from position %d of %d is %d \n\n",no_of_bits,position,number,toggleResult);
 
 return 0;
}

int TurnOffBits(int num, int pos, int bits)
{
 int x = 1;
 x = x << bits;
 x = x - 1;
 x = x << (pos - bits);
 x = ~x;
 return num & x;
}

int TurnOnBits(int num, int pos, int bits)
{
 int x = 1;
 x = x << bits;
 x = x - 1;
 x = x << (pos - bits);
 return num | x;
}


int ToggleBits(int num, int pos, int bits)
{
 int x = 1;
 x = x << bits;
 x = x - 1;
 x = x << (pos - bits);
 return num ^ x;
}