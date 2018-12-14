#include<stdio.h>                                                                                                                                                    
                                                                                                                                                                     
// function declaration                                                                                                                                              
void CountByOne();                                                                                                                                                   
void CountByTwo();                                                                                                                                                   
                                                                                                                                                                     
// declaration and initialization of global variable                                                                                                                 
int global_count = 0;                                                                                                                                                
                                                                                                                                                                     
int main()                                                                                                                                                           
{                                                                                                                                                                    
        // code                                                                                                                                                      
        printf("Value of global_count : %d\n",global_count);                                                                                                         
        CountByOne();                                                                                                                                                
        printf("Value of global_count : %d\n",global_count);                                                                                                         
        CountByTwo();                                                                                                                                                
        printf("Value of global_count : %d\n",global_count);                                                                                                         
        return(0);                                                                                                                                                   
}                                                                                                                                                                    
     
