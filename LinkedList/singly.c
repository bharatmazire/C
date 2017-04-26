#include<stdio.h>
#include<stdlib.h>
#include"singly.h"

STUDENT_T * Create(const int n)
{	// for creating link list of n nodes
	STUDENT_T *head = NULL;
	for (int i = 0 ; i < n ; i ++)
	{
		int rollNo;
		printf("\n Enter the roll number : ");
		scanf("%d",&rollNo);
		Insert(&head , i+1 , rollNo);
	}
	return head;
}

void Insert(STUDENT_T ** head, const int pos , const int rollNo)
{
	//  for inserting new node to any position
	if(head)
	{
		// create new student 
		STUDENT_T * st = (STUDENT_T *)malloc(sizeof(STUDENT_T));
		st->rollNo = rollNo;
		st->next = NULL;

		if(pos == 1) // inserting new node at 1st position
		{
			if(head != NULL)
			{
				st->next = *head;
			}
			*head = st;
		} // position one
		else
		{
			// for any position other than 1
			if(*head != NULL)
			{
				STUDENT_T * traverse = *head;
				int count = pos;
				while(traverse ->next != NULL && count - 2 != 0)
				{
					traverse = traverse->next;
					count--;
				}

				if(count -2 != 0)
				{
					printf("\n you enter wrong input but its okay i am adding at last position ");
				}
				st->next = traverse->next;
				traverse->next = st;
			}
			else
			{
				*head = st;
			}
		}

	}
}

void Display(STUDENT_T *head)
{	
	// to display all nodes in link linst
	printf("\n");
	if(head)
	{
		do
		{
			printf("RollNo = %d -> ",head->rollNo);
			head = head->next;
		}while(head);
	}
}

STUDENT_T * Delete(STUDENT_T * head , const int pos)
{
	// to delte node at perticular position
	if(head)
	{
		STUDENT_T * deleteStudent = NULL;
		if(pos==1)
		{
			deleteStudent = head;
			head = head->next;
		}
		else
		{
			int count = pos;
			STUDENT_T * traverse = head;
			while(traverse && count -2 != 0)
			{
				traverse = traverse -> next;
				count--;
			}
			if(traverse)
			{
				deleteStudent = traverse->next;
				traverse->next = traverse->next->next;
			}
			else
			{
				printf("\n invalid position ");
			}
		}
		if(deleteStudent)
			free(deleteStudent);
	}
	return head;
}

void Search(STUDENT_T *head , const int Value)
{
	// to search perticular value in link list
	int key = Value;
	int flag = 1;
	if(head)
	{
		do
		{
			if(head->rollNo == key)
			{
				flag = 0;
				break;
			}
			head = head->next;
		}while(head);
	}
	if(flag == 0)
		printf("\n Value found ");
	else
	{
		printf("\nValue not foound ");
	}
}

STUDENT_T * FreeLL(STUDENT_T * head)
 {
 	// to free the allocated memory
 	STUDENT_T * temp;
 	while(head)
	{
	    temp = head;
	    head = head->next;
	    free(temp);
	}
	head = NULL;
	return head;
}

int main()
{
	STUDENT_T * head = NULL;
	printf("\nWelcome to Link-List");
	int noOfStudents,choice,position,rollNo;
	do
	{
		printf("\n\nSelect on choice");
		printf("\n1.Create \n2.Display \n3.Insert \n4.Delete \n5.Search \n6.FreeAllMemory \n7.Exit \n your choice : ");
		scanf("%d",&choice);
		if(choice < 1 || choice > 7)
		{
			printf("\n Wrong choice ");
			choice = 7;
		}
		switch(choice)
		{
			case 1: if(head == NULL)
					{
						printf("\n please enter the number of you want to create ");
						scanf("%d",&noOfStudents);
						head = Create(noOfStudents);
					}
					else
						printf("\nHead is not NULL it means Link List is already created .");
					break;
			
			case 2: printf("\n Displyaing Link List ");
					if(head != NULL)
						Display(head);
					else
						printf("\nNo Linklist added ");
					break;
			
			case 3: printf("\nInsert ");
					printf("\nEnter Position to add : ");
					scanf("%d",&position);
					printf("\nEnter RollNumber to add : ");
					scanf("%d",&rollNo);
					Insert(&head,position,rollNo);
					break;
			
			case 4: printf("\nDelete node");
					printf("\n Enter the position to delete node : ");
					scanf("%d",&position);
					head = Delete(head,position);
					break;
			
			case 5: printf("\nSearch Function ");
					printf("\nEnter The value / roll nnumber to search : ");
					int val;
					scanf("%d",&val);
					Search(head,val);
					break;
			
			case 6: printf("\nFree All memory");
					head = FreeLL(head);
					break;
		}
	}while(choice != 7);

	printf("\n");
	return 0;
}