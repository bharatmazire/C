#include<stdio.h>
#include<stdlib.h>

#define BUFFER_SIZE 1024

struct BUFFER
{
  int iStatus; // 0 : free / 1 : locked / 2 : delayed write
  int iBlockNumber;
  char cData[BUFFER_SIZE];
  struct BUFFER *nextHashQueue;
  struct BUFFER *prevHashQueue;
  struct BUFFER *nextFreeList;
  struct BUFFER *prevFreeList;
};

struct BUFFER * insert(struct BUFFER * insertHead, int Data,struct BUFFER ** freeHead)
{
    struct BUFFER *st = (struct BUFFER *)malloc(sizeof(struct BUFFER));
    st->iStatus = 0;
    st->iBlockNumber = Data;
    st->nextHashQueue = NULL;
    st->prevHashQueue = NULL;
    st->nextFreeList = st;
    st->prevFreeList = st;
    
    if(insertHead == NULL)
    {
        insertHead = st;
    }
    else
    {
        struct BUFFER *traverse;
        traverse = insertHead;
        
        while(traverse->nextHashQueue != NULL)
        {
            traverse = traverse->nextHashQueue;
        }
        
        traverse->nextHashQueue = st;
        st->prevHashQueue = traverse;
    }
    
    if(*freeHead == NULL)
    {
        *freeHead = st;
    }
    else
    {
        struct BUFFER *traverse;
        traverse = *freeHead;
        st->nextFreeList = traverse->nextFreeList->prevFreeList;
        st->prevFreeList = traverse->prevFreeList;
        traverse->prevFreeList->nextFreeList = st;
        traverse->prevFreeList = st;
       
    }
    return(insertHead);
}

void displayHash(struct BUFFER *head)
{
    struct BUFFER *traverse;
    traverse = head;
        
    while(traverse!= NULL)
    {
      printf(" %d :: %d ->",traverse->iBlockNumber,traverse->iStatus);
      traverse = traverse->nextHashQueue;
    }
    printf(" NULL\n");
}

void displayFree(struct BUFFER *head)
{
    struct BUFFER *traverse;
    traverse = head;

    
    if(head == NULL)
    {
        printf("No data\n");
    }
    else
    {
        printf(" %d :: %d ->",traverse->iBlockNumber,traverse->iStatus);
        traverse = traverse->nextFreeList;
        
         while(traverse!= head)
        {
            printf(" %d :: %d ->",traverse->iBlockNumber,traverse->iStatus);
            traverse = traverse->nextFreeList;
        }
    }
}

int main()
{
    struct BUFFER **HashListHead = (struct BUFFER **)malloc(3 * sizeof(struct BUFFER *));
    struct BUFFER *FreeListHead = NULL;
    
    int c = 0;
    for(int i = 0 ; i < 3 ; i++)
    {
        for(int j = 0 ; j < 3 ; j++)
        {
            HashListHead[i] = insert(HashListHead[i],++c,&FreeListHead);
        }
    }
    
    for(int i = 0 ; i < 3 ; i++)
    {
        printf("HashList %d :\n",i);
        displayHash(HashListHead[i]);
    }
    
    printf("FREE LIST:\n");
    displayFree(FreeListHead);

    return 0;
}
