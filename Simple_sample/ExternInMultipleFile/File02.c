#include<stdio.h>                                                                                                                                                    
                                                                                                                                                                     
extern int global_count;                                                                                                                                             
                                                                                                                                                                     
void CountByTwo()                                                                                                                                                    
{                                                                                                                                                                    
        printf("global_count increased by two\n");                                                                                                                   
        global_count = global_count + 2;                                                                                                                             
}                                                                                                                                                                    
  
