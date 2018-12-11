#include<stdio.h>

void main()
{
    //declaration and initialization of variables
    int number = 13;
    char ch = 'P';
    char str[] = "AstroMediComp";
    long num = 30121995L;
    unsigned int b = 7;
    float f_num = 3012.1995f;
    double d_pi = 3.14159265358979323846;
    
    // code
    printf("number in decimal format : %d\n",number);
    printf("number in octal format : %o\n",number);
    printf("number in hexadecimal : %X\n",number);
    printf("number in hexadecimal : %x\n",number);
    printf("number in char : %c\n",number);
    printf("ch in char : %c\n",ch);
    printf("str in str : %s\n",str);
    printf("num in long integer : %ld\n",num);
    printf("unsigned int : %u\n",b);
    printf("float num : %f\n",f_num);
    printf("float num : %4.2f\n",f_num);
    printf("float num : %2.5f\n",f_num);
    printf("d_pi using g : %g\n",d_pi);
    printf("d_pi using e : %e\n",d_pi);
    printf("d_pi using E : %E\n",d_pi);
    printf("d_pi using a : %a\n",d_pi);
    printf("d_pi using A : %A\n",d_pi);

    
}
