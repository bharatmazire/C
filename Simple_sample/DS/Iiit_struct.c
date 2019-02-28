#include<stdio.h>
#include<stdlib.h>
#define SIZE_OF_IIT 9
#define SIZE_OF_HASH 3
#define NUM_OF_PROC 10


struct INODE
{
  int iFileType;
  int iPermission;
  int iNumOfLinks;
  int iFileSize;
  int iInodeNum;
  int iRefCount;
  struct INODE *nextHash;
  struct INODE *prevHash;
  struct INODE *nextFree;
  struct INODE *prevFree;
};

struct INODE IIT[SIZE_OF_IIT];  // global table
int igIITIndex = 0;

struct INODE * insert(int iInodeNum, struct INODE *HashHead, struct INODE **FreeHead)
{
  struct INODE *inode = (struct INODE *)malloc(sizeof(struct INODE));
  
  inode->prevHash = NULL;
  inode->nextHash = NULL;
  inode->nextFree = inode;
  inode->prevFree = inode;
  inode->iInodeNum = iInodeNum;
  inode->iFileType = 0;
  inode->iRefCount = 0;   // if ref Count is 0 : put it on freelist

  // for hash head
  if(HashHead == NULL)
  {
    printf("\n Hash Head is NULL !!\n");
    HashHead = inode;
  }
  else
  {
    struct INODE *travHash = NULL;
    travHash = HashHead;

    while(travHash->nextHash != NULL)
    {
      travHash = travHash->nextHash;
    }

    travHash->nextHash = inode;
    inode->prevHash = travHash;
  }

  // for free list
  if(*FreeHead == NULL)
  {
    printf("\n Free list head is NULL \n");
    *FreeHead = inode;
  }
  else
  {
    inode->nextFree = (*FreeHead);
    inode->prevFree = (*FreeHead)->prevFree;
    (*FreeHead)->prevFree->nextFree = inode;
    (*FreeHead)->prevFree = inode;
  }
  
  IIT[igIITIndex] = *inode;
  igIITIndex++;
  
  return(HashHead);
}

void display(int iChoice , struct INODE * travHead)
{
  if(travHead == NULL)
  {
    printf("\n NO DATA \n");
  }
  else
  {
    if(iChoice == 1)
    {
      while(travHead != NULL)
      {
        printf(" %d : %d -> ",travHead->iInodeNum,travHead->iRefCount);
        travHead = travHead->nextHash;
      }
      printf(" NULL ");
    }
    else if(iChoice == 2)
    {
      struct INODE *head;
      head = travHead;

      printf(" %d -> ",travHead->iInodeNum);
      travHead = travHead->nextFree;
      while(travHead != head)
      {
        printf(" %d -> ",travHead->iInodeNum);
        travHead = travHead -> nextFree;
      }
    }
    else
    {
      printf("Wrong Choice");
    }
  }
}



int main()
{
  //struct INODE * IIT = (struct INODE *)malloc(SIZE_OF_IIT * sizeof(struct INODE));
  
  struct INODE **HashQ = (struct INODE **)malloc(SIZE_OF_HASH * sizeof(struct INODE *));
  struct INODE *FreeList = NULL;

  //printf("Hello");
  for(int i = 0 ; i < SIZE_OF_HASH ; i ++)
  {
    HashQ[i] = NULL;
    for(int j = 0 ; j < SIZE_OF_HASH ; j++)
    {
      HashQ[i] = insert( i + (SIZE_OF_HASH * j), HashQ[i], &FreeList);
    }
  }

  printf("\n Hash List \n");
  for(int i = 0 ; i < SIZE_OF_HASH ; i++)
  {
    printf("HASH[%d] : ",i);
    display(1,HashQ[i]);
    printf("\n");
  }  
  printf("\n Free List \n");
  display(2,FreeList);


  printf("\n\n IIT \n");
  for(int i = 0 ; i < SIZE_OF_IIT ; i++)
  {
    printf(" %d ",IIT[i].iInodeNum);
  }

  printf("\n\n");

  for(int iNumOfProc = 0 ; iNumOfProc < NUM_OF_PROC ; iNumOfProc++)
  {
    int iInodeNum = rand()%20;








  }

  //printf("\n H e l l o \n");
  return(0);
}
