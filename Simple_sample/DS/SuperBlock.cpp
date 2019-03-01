#include<stdio.h>
#include <stdlib.h>

#define SIZE_OF_SUPERBLOCK 20
#define SIZE_OF_DILB 200

int iRememberedInode = 0;
int iDILB[SIZE_OF_DILB];			// for multi threading purpose


struct SUPERBLOCK
{
	int iSizeOfFS;
	int iNumOfFreeBlocksInFS;
	int iListOfFreeBlocksInFS[SIZE_OF_SUPERBLOCK];
	int iIndexOfNextFreeBlock;
	short sFreeBlockListLock;									// for locked or unlocked
	
	int iSizeOfDILB;
	int iNumOfFreeInodesInDILB;
	int iListOfFreeInodesInDILB[SIZE_OF_SUPERBLOCK];
	int iIndexOfNextFreeInode;
	short sFreeInodeListLock;									// for locked or unlocked
}SuperBlock;

void ialloc(struct SUPERBLOCK * SuperBlock)
{
	int iIsDone = 1;
	int iTempRememberInode = 0;
	int iCount = SIZE_OF_SUPERBLOCK - 1;
	
	while (iIsDone == 1)
	{
		if (SuperBlock->sFreeInodeListLock == 1)
		{
			printf("\n SUPER BLOCK IS LOCKED. \n \n SO SLEEP ... \n");
			_sleep(1000);
			continue;
		}
		if(SuperBlock->iIndexOfNextFreeInode == -1) //if (SuperBlock->iNumOfFreeInodesInDILB == 0)
		{
			printf("\n Super block is empty \n \n This Process need to fill it. \n");

			SuperBlock->sFreeInodeListLock = 1;
			
			iTempRememberInode = iRememberedInode + 1;
			while (iCount >= 0)
			{
				while(iDILB[iTempRememberInode] != 0)					// means file type is 0, so free
				{
					if (iTempRememberInode == iRememberedInode)
					{
						printf("\n ERROR \n \n No Free Inode \n");
						exit(-1);
					}
					else if (iTempRememberInode < SuperBlock->iSizeOfDILB)
						iTempRememberInode++;
					else
						iTempRememberInode = 0;

				}
				
				SuperBlock->iListOfFreeInodesInDILB[iCount] = iTempRememberInode;

				//SuperBlock->iListOfFreeInodesInDILB[iCount] = iTempRememberInode++;		// if u uncomment this, then comment below line of increment
				
				iTempRememberInode++;
				iCount--;
			}

			SuperBlock->sFreeInodeListLock = 0;

			iRememberedInode = iTempRememberInode;
			SuperBlock->iIndexOfNextFreeInode = SIZE_OF_SUPERBLOCK - 1;
			SuperBlock->iNumOfFreeInodesInDILB = SIZE_OF_SUPERBLOCK;
			
		}
		
		// get following inode
		printf("\nYou will get ' SuperBlock->iListOfFreeInodesInDILB[ %d ] ' which is ' %d ' \n", SuperBlock->iIndexOfNextFreeInode, SuperBlock->iListOfFreeInodesInDILB[SuperBlock->iIndexOfNextFreeInode]);
		printf("\nIGET()\n");
		printf("\n Write on Disk ASAP ...\n");
		
		iIsDone = 0;

		//SuperBlock->iListOfFreeInodesInDILB[SuperBlock->iIndexOfNextFreeInode];
		SuperBlock->iIndexOfNextFreeInode--;
		SuperBlock->iNumOfFreeInodesInDILB--;
		
		
	}
}


int main()
{
	
	int iNumOfProcess = 1;
	
	// code
	// initialize struct variables
	SuperBlock.iSizeOfDILB = SIZE_OF_DILB;
	SuperBlock.iNumOfFreeInodesInDILB = 0;	// initial stage .. later we are handling it in loop
	SuperBlock.iIndexOfNextFreeInode = SIZE_OF_SUPERBLOCK - 1;

	// initialize DILB with some random file type 0 or 1 : 0 means free
	for(int i = 0 ; i < SIZE_OF_DILB ; i++)
	{
		if(rand()%2)
		{
			SuperBlock.iNumOfFreeInodesInDILB = SuperBlock.iNumOfFreeInodesInDILB + 1;
			iDILB[i] = 1;
		}
		else
		{
			iDILB[i] = 0;
		}
	}	

	// initialize super block will all used
	for(int i = 0 ; i < SIZE_OF_SUPERBLOCK ; i++)
		SuperBlock.iListOfFreeInodesInDILB[i] = -1;				// intialize full list as free

	printf("\n INITIAL STATUS OF DILB AND SUPER BLOCK \n");
	printf("\n DILB : \n");
	for (int i = 0; i < SIZE_OF_DILB; i++)
	{
		//printf(" %d | ", iDILB[i]);
	}

	printf("\n SUPER-BLOCK \n");
	for (int i = 0; i < SIZE_OF_SUPERBLOCK; i++)
	{
		printf(" %d | ", SuperBlock.iListOfFreeInodesInDILB[i]);
	}

	printf("\n........................................\n");


	while(iNumOfProcess != 100)
	{
		printf("\n TOTAL NUMBER OF FREE INODES IN DILB : %d \n", SuperBlock.iNumOfFreeInodesInDILB);
		ialloc(&SuperBlock);

		printf("\n After Process %d\n\nCurrent DILB and Superblock structure is as follows : \n\n",iNumOfProcess);

	
		printf("\n DILB : \n");
		for (int i = 0; i < SIZE_OF_DILB; i++)
		{
			//printf(" %d | ", iDILB[i]);
		}

		printf("\n SUPER-BLOCK \n");
		for (int i = 0; i < SIZE_OF_SUPERBLOCK; i++)
		{
			printf(" %d | ", SuperBlock.iListOfFreeInodesInDILB[i]);
		}

		iNumOfProcess = iNumOfProcess + 1;
	}

	return(0);
}