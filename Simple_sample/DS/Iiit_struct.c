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
  struct INODE *nextHash;
  struct INODE *prevHash;
  struct INODE *nextFree;
  struct INODE *prevFree;
};

struct INODE * insert(int )
{

}

int main()
{
  //struct INODE * IIT = (struct INODE *)malloc(SIZE_OF_IIT * sizeof(struct INODE));
  struct INODE IIT[SIZE_OF_IIT];
  struct INODE **HashQ = (struct INODE **)malloc(SIZE_OF_HASH * sizeof(struct INODE *));
  struct INODE *FreeList = NULL;

  //printf("Hello");
  for(int i = 0 ; i < SIZE_OF_HASH ; i ++)
  {
    for(int j = 0 ; j < SIZE_OF_HASH ; j++)
    {
      HashQ[i] = insert( i*SIZE_OF_HASH, HashQ[i], &FreeList, IIT);
    }
  }
  return(0);
}
