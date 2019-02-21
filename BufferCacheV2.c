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



struct BUFFER * insert(struct BUFFER * insertHead, int BlockNumber, struct BUFFER ** freeHead)
{
	// initialization
	struct BUFFER *st = (struct BUFFER *)malloc(sizeof(struct BUFFER));
	
	// assigning values
	st->iStatus = 0;
	st->iBlockNumber = BlockNumber;
	st->nextHashQueue = NULL;
	st->prevHashQueue = NULL;
	st->nextFreeList = st;
	st->prevFreeList = st;
	
	// for hashlist
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

	// for freelist
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
		printf("\t\t BLK_NO = %d | STATUS =  %d ---> ", traverse->iBlockNumber,traverse->iStatus);
		traverse = traverse->nextHashQueue;
	}
	printf(" NULL \n");
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
		printf("\t\t BLK_NO = %d ---> ", traverse->iBlockNumber);
		traverse = traverse->nextFreeList;

		while (traverse != head)
		{
			printf(" BLK_NO = %d ---> ", traverse->iBlockNumber);
			traverse = traverse->nextFreeList;
		}
	}
}

void GetBlk(int db, struct BUFFER ** hashHead, struct BUFFER ** freeHead)
{
	printf("\nBlock Number is : %d \n", db);
	int modVal = db % 3;
	printf("Hash Value of %d is : %d \n",db, modVal);

	struct BUFFER *traverse;
	traverse = hashHead[modVal];
	while (traverse->iBlockNumber != db && traverse->nextHashQueue != NULL)
	{
		traverse = traverse->nextHashQueue;
	}
	if (traverse->iBlockNumber == db)
	{
		printf("\n\n GOT THE BUFFER \n\n");
		
		// if buffer on free list, remove it from free list

		if (traverse->nextFreeList != NULL && traverse->prevFreeList != NULL)
		{
			traverse->prevFreeList->nextFreeList = traverse->nextFreeList;
			traverse->nextFreeList->prevFreeList = traverse->prevFreeList;
			*freeHead = traverse->nextFreeList;
			traverse->prevFreeList = NULL;
			traverse->nextFreeList = NULL;
			traverse->iStatus = 1;
		}
	}
	else
	{
		printf("Buffer Not Present In Pool !!\n");
		printf("Getting buffer from Free List\n");


		struct BUFFER *t;
		t = *freeHead;
		if (t == NULL)
		{
			printf("No buffer in free list");
			printf("exiting");
			exit(0); // free and then exit
		}
		printf("Picking up 1st buffer from free list");

		t->prevFreeList->nextFreeList = t->nextFreeList;
		t->nextFreeList->prevFreeList = t->prevFreeList;
		*freeHead = t->nextFreeList;
		t->prevFreeList = NULL;
		t->nextFreeList = NULL;
		t->iStatus = 1;
		t->iBlockNumber = db;
	}

}

int main()
{
	// initialization
	struct BUFFER **HashListHead = (struct BUFFER **)malloc(3 * sizeof(struct BUFFER *));
	struct BUFFER *FreeListHead = NULL;

	int iDataBlock;
	int cnt = 1;

	// code

	// creating buffer pool
	for (int i = 0; i < 3; i++)
	{
		HashListHead[i] = NULL;
		for (int j = 0; j < 3; j++)
		{
			HashListHead[i] = insert(HashListHead[i], i + (3 * j), &FreeListHead);
		}
	}

	// printing buffer pool
	for (int i = 0; i < 3; i++)
	{
		printf("\nHashList %d :\n", i);
		displayHash(HashListHead[i]);
	}

	printf("FREE LIST:\n");
	displayFree(FreeListHead);

	printf("\n\n\n.........................................\n\n\n");
	

	// playing around

	while (cnt != 10)
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

