/* WAP to accept number from user , number of bits, position and you should return number of selected bits from position */

#include<stdio.h>

int main()
{
 int number , position , bits , result ;
 printf("\n Enter Number , Position , Bits , Result : \n");
 scanf("%d %d %d",&number,&position,&bits);
 printf("\n Accepted Numbers is : %d , Position is : %d  and Bits are : %d\n",number,position,bits);

 result = selected_bit_return(number,position,bits)


 return 0;
}
