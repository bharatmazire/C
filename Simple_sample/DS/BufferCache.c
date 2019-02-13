#include <iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define DATASIZE 1024

struct BUFFER
{
    int iBuffDeviceNumber;
    int iBuffBlockNumber;
    int iBuffStatus;
    
    char cData[DATASIZE];
 
    struct BUFFER * nextHash;
    struct BUFFER * prevHash;
    struct BUFFER * nextFree;
    struct BUFFER * prevFree;
};


int main()
{
    int iBufferPoolSize = 9;  // hardcoded 9 for initial purpose
    int iNumberOfHashQuese = 3; // hardcoded 3 for initial purpose

    int iInitialNoOfBuffInEachQueue = iBufferPoolSize / iNumberOfHashQuese;

    //int iTotalNumberOfHeads = iNumberOfHashQuese + 1; // + 1 for head of freelist

    struct BUFFER * ArrayOfHashList[iNumberOfHashQuese] = {NULL};
  
    struct BUFFER * FreeListHead = NULL;

    for(int i = 0 ; i < iNumberOfHashQuese ; i ++)
    {
        // for ArrayOfHashList[i] : 0,1,2
        for(int j = 0 ; j < iInitialNoOfBuffInEachQueue ; j++)
        {
            struct BUFFER * buff = (struct BUFFER *)malloc(sizeof(struct BUFFER));
            buff->iBuffDeviceNumber = 0;
            buff->iBuffBlockNumber = -1;
            buff->iBuffStatus = 0;
            
            if(j == 0)
            {
                buff->nextHash = NULL;
                buff->prevHash = NULL;
                buff->nextFree = ArrayOfHashList[i];
                buff->prevFree = ArrayOfHashList[i];
                
                ArrayOfHashList[i] = buff;
            }
            else
            {
                struct BUFFER * traverseHash = (struct BUFFER *)malloc(sizeof(struct BUFFER));
                traverseHash = ArrayOfHashList[i];
                while(traverseHash->nextHash != NULL)
                {
                    traverseHash = traverseHash->nextHash;    
                }
                
                struct BUFFER * traverseFree = (struct BUFFER *)malloc(sizeof(struct BUFFER));
                traverseFree = FreeListHead;
                
                buff->nextHash = NULL;
                buff->prevHash = traverseHash;
                buff->nextFree = FreeListHead->prevFree;
                buff->prevFree = FreeListHead->prevFree->nextFree
                
            }
        }
    }


    printf("Hello World !!");
    return(0);
}
