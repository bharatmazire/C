// selection sort
// for each iteration, minimum number will come at initial position

#include<stdio.h>

int main()
{
    // declaration and initialization of variable
    int arr[10] = {1,2,10,12,31,12,1,4,6,4};
    
    // code
    for(int i = 0 ; i < 10 ; i ++)
    {
        for(int j = i + 1 ; j < 10 ; j++)
        {
            if(arr[i] > arr[j])
            {
                printf("Swapping between arr[%d]-> %d and arr[%d]-> %d \n",i,arr[i],j,arr[j]);
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        printf("Arrya state after %dth iteration is : ",i);
        for(int p = 0 ; p < 10 ; p++)
        {
            printf("%d ",p);
        }
        printf("\n");
    }
    
    printf("Sorted array is : \n");
    for(int i = 0 ; i < 10 ; i ++)
    {
        printf("%d ",arr[i]);
    }
    
    
    return(0);
}
