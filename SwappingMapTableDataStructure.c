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
      
      if(ch == 1)
      {
        printf("\nadd\n");
        int start;
        int upto;
        int flag = 0;
        trav = head;

        printf("\nEnter amount of data : ");
        scanf("%d",&upto);

        printf("\nupto is %d",upto);
        printf("\ntrav->upto is %d",trav->iUpto);

        while(trav->iUpto < upto && trav->next != NULL)
        {
          printf("\nupto is %d",upto);
          printf("\ntrav->upto is %d",trav->iUpto);
          trav = trav->next;
        }
        if(trav->iUpto >= upto)
        {
          flag = 1;
        }
        if(flag == 1)
        {
          //do something
          trav->iUpto = trav->iUpto - upto;
          trav->iStart = trav->iStart + upto;
        }
        else
        {
          printf("\nNo place for swap out\n");
        }
      }
      else if(ch == 2)
      {
        printf("\nfree\n");
      }
        
      else if(ch == 3)
      {
        printf("\ndisplay\n");
        trav = head;
        printf("\nstart : %d :: upto val : %d\n",head->iStart , head->iUpto);
        while(trav->next != NULL)
        {
          printf("\nstart : %d :: upto val : %d\n",trav->iStart , trav->iUpto);
          trav = trav->next;
        }
        trav = NULL;
        free(trav);
      }
      else
      {
        printf("out\n\n");
      }
    }while(ch != 4);

    return(0);
}
