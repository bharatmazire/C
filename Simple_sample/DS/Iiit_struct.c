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
  int iLock;
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
  inode->iLock = 0;

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

void Proc(int iInodeNum, struct INODE **hashHead, struct INODE **FreeHead)
{
  int isDone = 0;
  int modVal = iInodeNum % SIZE_OF_HASH;


  while(isDone != 1)
  {
    if(hashHead[modVal] == NULL)
    {
      printf("\n No Inode Present on HASH LIST \n");
      // need to handle this condition ahead
      
    }
    else
    {
      struct INODE *trav = hashHead[modVal];
      while(trav->iInodeNum != iInodeNum && trav->nextHash != NULL)
      {
        trav = trav->nextHash;
      }
      if(trav->iInodeNum == iInodeNum )
      {
        if(trav->iLock == 0)
        {
          printf("\n Got The Unlocked Inode ! \n");
          printf("\n Increment the inode reference count \n");
          trav->iRefCount += 1;
        }
        else
        {
          printf("\nInode already locked !!\n");
          printf("\nInode Locked\n");
          // here you should sleep
          continue;      
        }
      }
      else if(trav->nextHash == NULL)
      {
        printf("\n Inode not present on HashQ \n");
        printf("\n Need To pick inode from free inode list \n");
        if(*FreeHead == NULL)
        {
          printf("\n No Inode on Free list \n ");
          continue;       // or return error
        }
        else
        {
          printf("\n Free List contains INODES \n");
          printf("\n Pick 1st Inode From list \n");
          if((*FreeHead)->prevHash == NULL)
          {
            printf("\n FreeHead 1st Inode is first on Hash List \n");
            hashHead[((*FreeHead)->iInodeNum)%SIZE_OF_HASH] = (*FreeHead)->nextHash;
            if((*FreeHead)->nextHash != NULL)
              (*FreeHead)->nextHash->prevHash = NULL;
          }
          else
          {
              (*FreeHead)->prevHash->nextHash = (*FreeHead)->nextHash;
          }
          (*FreeHead)->nextHash = NULL;
          (*FreeHead)->prevHash = trav;
          trav->nextHash = (*FreeHead);
        }
      }
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

    
    //Proc(iInodeNum,HashQ,&FreeList); 
    







  }

  //printf("\n H e l l o \n");
  return(0);
}
