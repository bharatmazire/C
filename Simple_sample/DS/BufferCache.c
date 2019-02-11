#include <iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define DATASIZE 1024

struct BUFFER
{
  int iDeviceNumber;
  int iBlockNumber;
  int iStatus;

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
      struct BUFFER * buff = (struct BUFFER *)malloc(sizeof)(struct BUFFER *);
    }
  }


  printf("Hello World !!");
  return(0);
}
