#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main()
{
  // declaration of variables
  int iNumberOfPiece;
  
  int *Source = NULL;
  int *Auxilary = NULL;
  int *Destination = NULL;

  int iTotalNumberOfIteration;

  int topStack[3] = {0,0,0};
  // topStack[0] = for source top
  // topStack[1] = for Auxilary top
  // topStack[2] = for destination top
  // initially set all top at 0
  // we can set top seperate but just for learning purpose i am making it in array

  // code

  printf("\nEnter the number of piece you want : \n");
  scanf("%d",&iNumberOfPiece);

  Source = (int *)malloc(sizeof(int) * iNumberOfPiece);
  Auxilary = (int *)malloc(sizeof(int) * iNumberOfPiece);
  Destination = (int *)malloc(sizeof(int) * iNumberOfPiece);

  iTotalNumberOfIteration = pow(2,iNumberOfPiece);

  for(int i = 0 ; i < iNumberOfPiece; i++)
  {
    Source[i] = iNumberOfPiece-1-i;
    topStack[0] = i;
  }

  for(int i = 0 ; i < iNumberOfPiece ;i++)
  {
    printf("%d ",Source[i]);
  }

 for(int i = 1 ; i <= iTotalNumberOfIteration ; i++)
 {
   if((i % 3) == 1)
   {
     // legal move between source and destination
    



   }
   else if((i % 3) == 2)
   {
     // legal move between source and Auxilary
   }
   else if((i % 3) == 0)
   {
     // legal move between Auxilary and destination
   }
   else
   {
     printf("\nYou will(/should) not come here !!\n");
   }
 
 
 }

  return(0);
}
