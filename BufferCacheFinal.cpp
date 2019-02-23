#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

#define BUFFER_SIZE 1024
#define HASHVAL 3

struct BUFFER
{
	int iStatus;						// 0 : free / 1 : locked / 2 : delayed write
	int iBlockNumber;
	char cData[BUFFER_SIZE];
	struct BUFFER *nextHashQueue;
	struct BUFFER *prevHashQueue;
	struct BUFFER *nextFreeList;
	struct BUFFER *prevFreeList;
};

typedef struct TreadParam
{
	int db;
	int processNum;
	struct BUFFER ** hashHead;
	struct BUFFER ** freeHead;
}THREAD_PARAM,*THPARAM;

#pragma region INITIAL_STAGE
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

#pragma endregion


void GetBlk(int db, struct BUFFER ** hashHead, struct BUFFER ** freeHead)
{
	printf("\nBlock Number is : %d \n", db);
	int modVal = db % HASHVAL;
	printf("Hash Value of %d is : %d \n",db, modVal);

	struct BUFFER *traverse;
	traverse = hashHead[modVal];

	while (hashHead[modVal] != NULL && traverse->iBlockNumber != db && traverse->nextHashQueue != NULL)
	{
		traverse = traverse->nextHashQueue;
	}
	if (hashHead[modVal] != NULL && traverse->iBlockNumber == db)
	{
		printf("\n\n GOT THE BUFFER IN BUFFER POOL \n\n");
		
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
		

		
		// remove it from old hash queue and put it on new
		// we have traverse at end of hashqueue[modbval] so use it
		if (t->prevHashQueue == NULL)
		{
			printf("\n *** 1st free buffer in hashqueue ");
			// this is 1st free buffer on hash queue
			int val = (t->iBlockNumber) % HASHVAL;
			printf("\nval : %d", val);
			hashHead[val] = t->nextHashQueue;
			if (t->nextHashQueue != NULL)
			{
				t->nextHashQueue->prevHashQueue = NULL;
			}
		}
		else
		{
			t->prevHashQueue->nextHashQueue = t->nextHashQueue;
			t->nextHashQueue->prevHashQueue = t->prevHashQueue;
		}
		
		t->prevHashQueue = traverse;
		t->nextHashQueue = NULL;
		if (traverse != NULL)
			traverse->nextHashQueue = t;
		else
			hashHead[modVal] = t;
		t->iBlockNumber = db;		// so that we can use prev info of buffer in between
	}
}

DWORD MyThreadProc(LPVOID param)
{
	// initialization of variables
	THREAD_PARAM *th = (THPARAM)param;
	struct BUFFER ** hashHead = th->hashHead;
	int db = th->db;
	struct BUFFER ** freeHead = th->freeHead;
	int procNum = th->processNum;

	struct BUFFER *UsedBuff; // required for future in (free list element add method)

	// code
	printf("\nFor Process : %d , Block Number is : %d \n",procNum, db);
	int modVal = db % 3;
	printf("\nFor Process : %d , Hash Value of %d is : %d \n",procNum, db, modVal);

	struct BUFFER *traverse;
	traverse = hashHead[modVal];

	while (hashHead[modVal] != NULL && traverse->iBlockNumber != db && traverse->nextHashQueue != NULL)
	{
		traverse = traverse->nextHashQueue;
	}
	if (hashHead[modVal] != NULL && traverse->iBlockNumber == db)
	{
		printf("\n\n Process %d ,  GOT THE BUFFER \n\n",procNum);

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
		printf("For Process %d, Buffer Not Present In Pool !!\n",procNum);
		printf("For Process %d, Getting buffer from Free List\n",procNum);


		struct BUFFER *t;
		t = *freeHead;
		if (t == NULL)
		{
			printf("For Process %d, No buffer in free list",procNum);
			printf("For Process %d, sleeping for some time....",procNum);
			_sleep(rand()%10000);
			MyThreadProc(param);
			return(0);
			//exit(0); // free and then exit
		}
		printf("For Process %d, Picking up 1st buffer from free list",procNum);

		t->prevFreeList->nextFreeList = t->nextFreeList;
		t->nextFreeList->prevFreeList = t->prevFreeList;
		*freeHead = t->nextFreeList;
		t->prevFreeList = NULL;
		t->nextFreeList = NULL;
		t->iStatus = 1;
		//t->iBlockNumber = db;

		// remove it from old hash queue and put it on new
		// we have traverse at end of hashqueue[modbval] so use it
		if (t->prevHashQueue == NULL)
		{
			printf("\n *** 1st free buffer in hashqueue ");
			// this is 1st free buffer on hash queue
			int val = (t->iBlockNumber) % HASHVAL;
			printf("\nval : %d", val);
			hashHead[val] = t->nextHashQueue;
			if (t->nextHashQueue != NULL)
			{
				t->nextHashQueue->prevHashQueue = NULL;
			}
		}
		else
		{
			t->prevHashQueue->nextHashQueue = t->nextHashQueue;
			t->nextHashQueue->prevHashQueue = t->prevHashQueue;
		}

		t->prevHashQueue = traverse;
		t->nextHashQueue = NULL;
		if (traverse != NULL)
			traverse->nextHashQueue = t;
		else
			hashHead[modVal] = t;
		t->iBlockNumber = db;		// so that we can use prev info of buffer in between
		//UsedBuff = t;
	}

	printf("For Process %d, process is working on block ....",procNum);
	_sleep(rand() % 1000);
	printf("For Process %d, putting buffer on freelist !!",procNum);
	
	

	//struct BUFFER *UsedBuff;
	UsedBuff = hashHead[modVal];
	while (UsedBuff->iBlockNumber != db && UsedBuff->nextHashQueue != NULL)
	{
		UsedBuff = UsedBuff->nextHashQueue;
	}
	if (UsedBuff->iBlockNumber == db)
	{
		if (*freeHead == NULL)
		{
			*freeHead = UsedBuff;
		}
		else
		{
			struct BUFFER *forFree;
			forFree = *freeHead;
			UsedBuff->nextFreeList = forFree->nextFreeList->prevFreeList;
			UsedBuff->prevFreeList = forFree->prevFreeList;
			forFree->prevFreeList->nextFreeList = UsedBuff;
			forFree->prevFreeList = UsedBuff;
		}
		UsedBuff->iStatus = 0;
	}
	else
	{
		printf("For Process %d, some thing is wrong !!!",procNum);
	}
	return (0);
}

int main()
{
	// initialization
	struct BUFFER **HashListHead = (struct BUFFER **)malloc(3 * sizeof(struct BUFFER *));
	struct BUFFER *FreeListHead = NULL;

	int iDataBlock;
	int cnt = 1;

	// code

#pragma region INITIAL_CALL
	// creating buffer pool
	printf("\n \t INITIAL STAGE \n \t ");
	for (int i = 0; i < HASHVAL; i++)
	{
		HashListHead[i] = NULL;
		for (int j = 0; j < HASHVAL; j++)
		{
			HashListHead[i] = insert(HashListHead[i], i + (HASHVAL * j), &FreeListHead);
		}
	}

	// printing buffer pool
	printf("\n \t PRINTING INITIAL BUFFER POOL STATUS ...\n\n");
	for (int i = 0; i < HASHVAL; i++)
	{
		printf("\nHashList %d :\n", i);
		displayHash(HashListHead[i]);
	}

	printf("FREE LIST:\n");
	displayFree(FreeListHead);

#pragma endregion

	printf("\n\n\n.........................................\n\n\n");
	

	// playing around

	while (cnt != 10)
	{
		int iDataBlock = rand();

		THREAD_PARAM thread_param;
		thread_param.db = iDataBlock;
		thread_param.freeHead = &FreeListHead;
		thread_param.hashHead = HashListHead;
		thread_param.processNum = cnt;

		//GetBlk(iDataBlock, HashListHead, &FreeListHead);				// for single threaded
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MyThreadProc, (LPVOID)&thread_param, 0, NULL);
		

		printf("\n FOR PROCESS %d ... \n",cnt);
		for (int i = 0; i < HASHVAL; i++)
		{
			printf("HashList %d :\n", i);
			displayHash(HashListHead[i]);
		}

		printf("FREE LIST:\n");
		displayFree(FreeListHead);
		printf("\n\n");

		_sleep(1000);
		cnt++;
	}
	

	_sleep(30000);
	printf("FINAL BUFFER POOL STRUCTURE ....\n\n");
	for (int i = 0; i < 3; i++)
	{
		printf("HashList %d :\n", i);
		displayHash(HashListHead[i]);
	}

	printf("FREE LIST:\n");
	displayFree(FreeListHead);
	printf("\n\n");

	return 0;
}

