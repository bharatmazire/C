#include<stdio.h>


// declaration of functions
void ChangeCountByOne();

void main()
{
    // declaration of extern variable
    extern int count;
    
    // code
    printf("count befor ChangeCountByOne() is : %d\n",count);
    ChangeCountByOne();
    printf("count after ChangeCountByOne() is : %d\n\n",count);
}

// declaration and initialization of global variable
int count = 0;

void ChangeCountByOne()
{
    // code
    count = count + 1;
    printf("(in change count function)count = %d \n",count);
}
