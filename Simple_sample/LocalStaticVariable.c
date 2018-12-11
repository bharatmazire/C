#include<stdio.h>

// declaration of functions
void ChangeCount();

void main()
{
    // declaration of variable
    int a = 5;
    
    // code
    printf("a = %d\n",a);
    
    ChangeCount();
    ChangeCount();
}

void ChangeCount()
{
    // declare and initialize local variable 
    static int count = 0;
    
    // code
    printf("before increment count = %d \n",count);
    count = count + 1;
    printf("after increment count = %d \n",count);
}
