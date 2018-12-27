// insertion sort

#include<stdio.h>

int main()
{
    // declaration 
    int arr[10] = {10,9,8,7,6,5,4,3,2,1};
    
    
    // code
    for(int i = 0 ; i < 10 ; i++)
    {
        int temp = arr[i];
        int j = i;
        
        printf("\nJ : %d and temp = %d\n",j,arr[i]);
        
        while(j > 0 && arr[i -  1 ] > temp)
        {
            arr[j] = arr[j - 1];
            j--;
        
            printf("\n :: ");
            for(int ip = 0 ; ip < 10 ; ip++)
                printf("%d ",arr[ip]);
        }
        
        printf("\n\n :>: ");
        for(int ip = 0 ; ip < 10 ; ip++)
            printf("%d ",arr[ip]);
        
        printf("\nJ = %d arr[%d] = %d replace by temp = %d\n",j,j,arr[j],temp);
        arr[j] = temp;
    }
    
    printf("\n\n");
    for(int i = 0 ; i < 10 ; i++)
        printf("%d ",arr[i]);
    
    return(0);
}
