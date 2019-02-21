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



struct BUFFER * insert(struct BUFFER * insertHead, int Data, struct BUFFER ** freeHead)
{
	
	struct BUFFER *st = (struct BUFFER *)malloc(sizeof(struct BUFFER));
	st->iStatus = 0;
	st->iBlockNumber = Data;
	st->nextHashQueue = NULL;
	st->prevHashQueue = NULL;
	st->nextFreeList = st;
	st->prevFreeList = st;
	
	if (insertHead == NULL)
	{
		insertHead = st;
	}
	else
	{
		struct BUFFER *traverse;
		traverse = insertHead;
	
		while (traverse->nextHashQueue != NULL)
		{
			traverse = traverse->nextHashQueue;
		}
		
		traverse->nextHashQueue = st;
		st->prevHashQueue = traverse;
	}

	if (*freeHead == NULL)
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

	while (traverse != NULL)
	{
		printf(" %d :: %d ->", traverse->iBlockNumber,traverse->iStatus);
		traverse = traverse->nextHashQueue;
	}
	printf(" NULL\n");
}

void displayFree(struct BUFFER *head)
{
	struct BUFFER *traverse;
	traverse = head;

	if (head == NULL)
	{
		printf("No data\n");
	}
	else
	{
		printf(" %d :: %d ->", traverse->iBlockNumber, traverse->iStatus);
		traverse = traverse->nextFreeList;

		while (traverse != head)
		{
			printf(" %d :: %d ->", traverse->iBlockNumber, traverse->iStatus);
			traverse = traverse->nextFreeList;
		}
	}
}




void GetBlk(int db, struct BUFFER ** hashHead, struct BUFFER ** freeHead)
{
	printf("\nblock number : %d \n", db);
	int modVal = db % 3;
	printf("mod val of block number is : %d \n", modVal);

	struct BUFFER *traverse;
	traverse = hashHead[modVal];
	while (traverse->iBlockNumber != db && traverse->nextHashQueue != NULL)
	{
		traverse = traverse->nextHashQueue;
	}
	if (traverse->iBlockNumber == db)
	{
		printf("got buffer \n");
		// if buffer on free list, remove it from free list

		if (traverse->nextFreeList != NULL && traverse->prevFreeList != NULL)
		{
			traverse->prevFreeList->nextFreeList = traverse->nextFreeList;
			traverse->nextFreeList->prevFreeList = traverse->prevFreeList;
			traverse->prevFreeList = NULL;
			traverse->nextFreeList = NULL;
			traverse->iStatus = 1;
		}
	}
	else
	{
		printf("buffer not present in pool !!\n");
		printf("need to get new buffer from free list and use it !\n");

		// part for buffer not in pool






	}

}

int main()
{
	// initial part of buffer pool creation with some buffers
	struct BUFFER **HashListHead = (struct BUFFER **)malloc(3 * sizeof(struct BUFFER *));
	struct BUFFER *FreeListHead = NULL;

	for (int p = 0; p < 3; p++)
	{
		HashListHead[p] = NULL;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			HashListHead[i] = insert(HashListHead[i], i, &FreeListHead);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		printf("HashList %d :\n", i);
		displayHash(HashListHead[i]);
	}

	printf("FREE LIST:\n");
	displayFree(FreeListHead);


	// buffer pool created

	//......................

	int iDataBlock;
	int cnt = 1;
	while (cnt != 20)
	{
		int iDataBlock = rand()%10;
		GetBlk(iDataBlock, HashListHead, &FreeListHead);
		cnt++;

		for (int i = 0; i < 3; i++)
		{
			printf("HashList %d :\n", i);
			displayHash(HashListHead[i]);
		}

		printf("FREE LIST:\n");
		displayFree(FreeListHead);
		printf("\n\n");

		//_sleep(1000);
	}
		
	return 0;
}

