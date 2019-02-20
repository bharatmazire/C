#include<stdio.h>
#include<stdlib.h>

#define BUFFER_SIZE 1024

struct BUFFER
{
  int iData;
  struct BUFFER *nextHashQueue;
  struct BUFFER *prevHashQueue;
  struct BUFFER *nextFreeList;
  struct BUFFER *prevFreeList;
};

struct BUFFER * insert(struct BUFFER * insertHead, int Data,struct BUFFER ** freeHead)
{
    struct BUFFER *st = (struct BUFFER *)malloc(sizeof(struct BUFFER));
    st->iData = Data;
    st->nextHashQueue = NULL;
    st->prevHashQueue = NULL;
    st->nextFreeList = NULL;
    st->prevFreeList = NULL;
    
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
        
        while(traverse->nextFreeList != NULL)
        {
            traverse = traverse->nextFreeList;
        }
        
        printf("%d \n",traverse->iData);
        traverse->nextFreeList = st;
        st->prevFreeList = traverse;
    }
    
    
    return(insertHead);
}

void display(struct BUFFER *head)
{
    struct BUFFER *traverse;
    traverse = head;
        
    while(traverse!= NULL)
    {
      printf(" %d ->",traverse->iData);
      traverse = traverse->nextHashQueue;
    }
    printf(" NULL\n");
}

void displayFree(struct BUFFER *head)
{
    struct BUFFER *traverse;
    traverse = head;
        
    while(traverse!= NULL)
    {
      printf(" %d ->",traverse->iData);
      traverse = traverse->nextFreeList;
    }
    printf(" NULL\n");
}

int main()
{
    struct BUFFER **HashListHead = (struct BUFFER **)malloc(3 * sizeof(struct BUFFER *));
    struct BUFFER *FreeListHead = NULL;
    
    int ch,data;
    int c = 0;
    for(int i = 0 ; i < 3 ; i++)
    {
        for(int j = 0 ; j < 3 ; j++)
        {
            //printf("add : %d\n",&FreeListHead);
            HashListHead[i] = insert(HashListHead[i],++c,&FreeListHead);
        }
    }
    
    for(int i = 0 ; i < 3 ; i++)
    {
        printf("HashList %d :\n",i);
        display(HashListHead[i]);
    }
    
    printf("FREE LIST:\n");
    displayFree(FreeListHead);

    
    /*
    do
    {
        printf("\n1.insert 2.display 3.exit\n");
        scanf("%d",&ch);
        
        switch(ch)
        {
            case 1:
            printf("\nEnter data to insert : ");
            scanf("%d",&data);
            //printf("\ncalling function with data : %d",data);
            head[0] = insert(head[0],data);
            break;
            case 2:display(head[0]);
            break;
            case 3:printf("\nbye\n");
            break;
        }
    }while(ch != 3);
    */
    
    return 0;
}
