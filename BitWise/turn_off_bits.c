// WAP to turn off the bit or set of bits

#include<stdio.h>

int TurnOffBits(int,int,int);       // prototype and funtion declaration ...
				    // no need to give variable name as their ';'
int main()
{
 int number, position, no_of_bits;
 printf("\n enter number , position and number of bits to TURN OFF ....");
 scanf("%d %d %d",&number,&position,&no_of_bits);
 printf("%d %d %d ",number,position,no_of_bits);

 int result = TurnOffBits(number,position,no_of_bits);
 printf("\n After turning of %d bits from position %d of %d is %d \n\n",no_of_bits,position,number,result);

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
// return num & (~(((1 << bits)-1)<<(pos-bits)));
}
