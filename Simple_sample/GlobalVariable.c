#include<stdio.h>

// declaration and initialization of global variable
int count = 0;

// declaration of functions
void ChangeCountByOne();
void ChangeCountByTwo();

void main()
{
    // code
    printf("count befor ChangeCountByOne() is : %d\n",count);
    ChangeCountByOne();
    printf("count after ChangeCountByOne() is : %d\n\n",count);
    
    printf("count befor ChangeCountByTwo() is : %d\n",count);
    ChangeCountByTwo();
    printf("count after ChangeCountByTwo() is : %d\n",count);
}

void ChangeCountByOne()
{
    // code
    count = count + 1;
    printf("count = %d \n",count);
}

void ChangeCountByTwo()
{
    // code
    count = count + 2;
    printf("count = %d \n",count);
}
