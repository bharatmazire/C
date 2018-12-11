#include<stdio.h>


// declaration of functions
void ChangeCountByOne();

void main()
{
    // declaration of variable
    int count;
    
    // code
    printf("count befor ChangeCountByOne() is : %d\n",count);
    ChangeCountByOne();
    printf("count after ChangeCountByOne() is : %d\n\n",count);
}

// declaration and initialization of global variable
extern int count = 0;

void ChangeCountByOne()
{
    // code
    count = count + 1;
    printf("count = %d \n",count);
}
