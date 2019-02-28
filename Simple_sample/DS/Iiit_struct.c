#include<stdio.h>
#include<stdlib.h>
#define SIZE_OF_IIT 9
#define SIZE_OF_HASH 3

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

struct INODE * insert(int iInodeNum, struct INODE *HashHead, struct INODE **FreeHead)
{
  struct INODE *inode = (struct INODE *)malloc(sizeof(struct INODE));
  inode->prevHash = NULL;
  inode->nextHash = NULL;
  inode->nextFree = inode;
  inode->prevFree = inode;
  inode->iInodeNum = iInodeNum;
  inode->iFileType = 0;
  inode->iRefCount = 0;

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
    *FreeHead = inode;
  }
  else
  {
    inode->nextFree = (*FreeHead);
    inode->prevFree = (*FreeHead)->prevFree;
    (*FreeHead)->prevFree->nextFree = inode;
    (*FreeHead)->prevFree = inode;
  }
  return(HashHead);
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
  return(0);
}
