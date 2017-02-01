#include<stdio.h>

int turnOnBits(int,int,int);

int main()
{
 int number, position, no_of_bits;
 printf("\n enter the number , position and number of bits : ");
 scanf("%d %d %d",&number,&position,&no_of_bits);
 int result = turnOnBits(number,position,no_of_bits);
 printf("\n after turning % bits of %d position of number %d ,  generated nuber is : %d\n\n\n",no_of_bits,position,number,result);
return 0;
}

int turnOnBits(int no,int pos,int bits)
{
 int x = 1;
 x = x<<bits;
 x = x - 1;
 x = x << (pos-bits);
// for turn off
// x = ~x;
//return no & x;
 return no | x;

}
