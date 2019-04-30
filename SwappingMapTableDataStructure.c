#include<stdio.h>
#include<stdlib.h>

#define HSTART 1
#define HEND 10000

struct MAP_TABLE
{
  int iStart;
  int iUpto;
  struct MAP_TABLE * next;
};

int main()
{
  struct MAP_TABLE * head = NULL;
  struct MAP_TABLE *trav;
  struct MAP_TABLE *prev_trav;

  struct MAP_TABLE * new_entry = (struct MAP_TABLE *)malloc(sizeof(struct MAP_TABLE));
    
  new_entry->iStart = 1;
  new_entry->iUpto = 10000;
  new_entry->next = NULL;

  head = new_entry;

  int ch = 0;
  do
  {
    printf("\n1.malloc 2. free 3. display 4.exit\n\n");
    scanf("%d",&ch);
      
    // for 1 choice add
    if(ch == 1)
    {
      printf("\nadd\n");
      int start;
      int upto;
      int flag = 0;
      trav = head;

      printf("\nEnter amount of data : ");
      scanf("%d",&upto);

      while(trav->iUpto < upto && trav->next != NULL)
      {
        trav = trav->next;
      }
      if(trav->iUpto >= upto)
      {
        flag = 1;
      }
      if(flag == 1)
      {
        trav->iUpto = trav->iUpto - upto;
        trav->iStart = trav->iStart + upto;
      }
      else
      {
        printf("\nNo place for swap out\n");
      }
    }
  
    // for choice 2, free
    else if(ch == 2)
    {
      int freeStart;
      int freeUpto;
      int f_for_maintain_prev = 0;
      trav = head;
      prev_trav = NULL;

      struct MAP_TABLE * free_map = (struct MAP_TABLE *)malloc(sizeof(MAP_TABLE));

      printf("\nfree\n");

      printf("\nEnter start : \n");
      scanf("%d",&freeStart);

      printf("\nEnter upto : \n");
      scanf("%d",&freeUpto);

      free_map->iStart = freeStart;
      free_map->iUpto = freeUpto;
      free_map->next = NULL;

      while(trav && trav->iStart <= free_map->iStart){
        prev_trav = trav;
        trav = trav->next;
      }
      if(!trav) // trav == NULL;
      {
        prev_trav->next = free_map;
      }
      else
      {
        if(prev_trav) 
        {
          free_map->next = prev_trav->next;
          prev_trav-> next = free_map;
        }
        else 
        {
          free_map->next = head;
          head = free_map;
        }
      }

      trav = head;
      while(trav->next != NULL)
      {
        if(trav->iStart + trav->iUpto == trav->next->iStart)
        {
          trav->iUpto = trav->next->iUpto + trav->iUpto;
          trav->next = trav->next->next;
        }
        trav = trav->next;
      }
    }
    // for choice 3, display
    else if(ch == 3)
    {
      printf("\ndisplay\n");
      trav = head;
      //printf("\nstart : %d :: upto val : %d\n",head->iStart , head->iUpto);
      while(trav != NULL)
      {
        printf("\nstart : %d :: upto val : %d\n",trav->iStart , trav->iUpto);
        trav = trav->next;
      }
    }

    // for other choice
    else
    {
      trav = NULL;
      free(trav);
      printf("out\n\n");
    }
  }while(ch != 4);

  return(0);
}
