/*
Simple program to demonstrate 
1.example of struct
2.pass by ref
3.array of struct, pass by ref
*/
#include<stdio.h>
#include<stdlib.h>

struct ST_VAL
{
    int a;
    char c;
};

void fill_my_struct(struct ST_VAL ** s)
{
    s[0]->a = 10;
    s[0]->c = 'c';
}

int main()
{
    struct ST_VAL* s = (struct ST_VAL*)malloc(sizeof(struct ST_VAL) * 1);
    
    fill_my_struct(&s);
    
    printf("val : %d \n",s[0].a);
    printf("val : %c \n",s[0].c);
    
    return(0);
}
