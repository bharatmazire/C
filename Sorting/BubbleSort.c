// bubble sort 

#include<stdio.h>

int main()
{
    int arr[10] = {10,2,4,1,13,5,7,3,2,1};
    
    for(int i = 0 ; i < 10 ; i ++)
    {
        for(int j = 1 ; j < 10 - i ; j++)
        {
            printf("Swapping between %d and %d\n",arr[j-1],arr[j]);
            if(arr[j-1] > arr[j])
            {
                int temp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = temp;
            }
        }
        printf("For %dth iteration array status is : ",i+1);
        for(int p = 0 ; p < 10 ; p ++)
        {
            printf("%d ",arr[p]);
        }
        printf("\n");
    }
    
    printf("Final sorted array : \n");
    for(int i = 0 ; i < 10 ; i ++)
    {
        printf("%d ",arr[i]);
    }
    return(0);
}
