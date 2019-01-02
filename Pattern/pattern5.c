// pattern

#include<stdio.h>

// function declarations
void RightTriangle(int);
void MirroredRightTriangle(int);
void InvertedRightTriangle(int);
void InvertedMirroredRightTriangle(int);
void NumberPattern1(int);
void NumberPattern2(int);
void Pyramid(int);

int main()
{
    // local variable declarations
    int n ;
    
    // code
    scanf("%d",&n);
    
    printf("\n\n");
    
    // call to function
    RightTriangle(n);
    printf("\n\n");
    MirroredRightTriangle(n);
    printf("\n\n");
    InvertedRightTriangle(n);
    printf("\n\n");
    InvertedMirroredRightTriangle(n);
    printf("\n\n");
    NumberPattern1(n);
    printf("\n\n");
    NumberPattern2(n);
    printf("\n\n");
    //Pyramid(n)
    
    return(0);
}


void NumberPattern2(int n)
{
    int num = n - 1 + n;
    int arr[num];
    int arr1[n][num];
    int num1 = n;
    for(int i = 0 ; i < num - n + 1 ; i ++,num1--)
    {
        for(int j = i ; j < num-i ; j++)
        {
            arr[j] = num1;    
        }
        for(int k = 0 ; k < num ; k++)
        {
            printf("%d ",arr[k]);
            arr1[i][k] = arr[k];
        }
            
        printf("\n");
    }
    for(int p = n - 2 ; p >= 0 ; p--)
    {
        for(int pp = 0 ; pp < num ; pp ++)
        {
            printf("%d ",arr1[p][pp]);
        }
        printf("\n");
    }
    
}


void NumberPattern1(int n)
{
    for(int i = 1 ; i <= n ; i ++)
    {
        for(int j = 1 ; j <= i ; j++)
        {
            printf("%d",j);
        }
        for(int j = n - i ; j > 0 ; j--)
        {
            printf(" ");
        }
        for(int j = n - i ; j > 0 ; j--)
        {
            printf(" ");
        }
        for(int j = i ; j > 0 ; j--)
        {
            printf("%d",j);
        }
        printf("\n");
    }
}

void InvertedMirroredRightTriangle(int n)
{
    for(int i = n ; i > 0 ; i --)
    {
        for(int j = 0 ; j < n - i ; j ++)
        {
            printf(" ");
        }
        for(int j = i ; j > 0 ; j --)
        {
            printf("*");
        }
        printf("\n");
    }
}

void InvertedRightTriangle(int n)
{
    for(int i = n ; i > 0 ; i --)
    {
        for(int j = i ; j > 0 ; j --)
        {
            printf("*");
        }
        printf("\n");
    }
}

void RightTriangle(int n)
{
    for(int i = 1 ; i <= n ; i++)
    {
        for(int j = 0 ; j < i ; j ++)
        {
            printf("*");
        }
        printf("\n");
    }
}

void MirroredRightTriangle(int n)
{
    for(int i = 1 ; i <= n ; i ++)
    {
        for(int k = 0 ; k < n - i ; k ++)
        {
            printf(" ");
        }
        for(int j = 0 ; j < i ; j ++)
        {
            printf("*");
        }
        printf("\n");
    }
}
