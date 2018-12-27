// insertion sort

#include<stdio.h>

int main()
{
    int arr[10] = {10,9,8,7,6,5,4,3,2,1};
    
    for(int i = 0 ; i < 10 ; i++)
    {
        int temp = arr[i];
        int j = i;
        
        while(j > 0 && arr[i -  1 ] > temp)
        {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = temp;
    }
    for(int i = 0 ; i < 10 ; i++)
        printf("%d ",arr[i]);
    
    return(0);
}
