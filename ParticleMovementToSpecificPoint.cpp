#include<stdio.h>
#include<stdlib.h>

#define LIMIT 20

// global variables
int X[LIMIT];
int Y[LIMIT];
int Z[LIMIT];


int main()
{
  // declaration of variables
  int XFinal = 0;
  int YFinal = 0;
  int ZFinal = 0;

  int TotalCheck[LIMIT] = {0};
  int TotalValid = 0;

  int isDone = 1;
  
  // declaration of function
  void display(void);

  //code
  for(int i = 0 ; i < LIMIT ; i++)
  {
    X[i] = rand() % 100;
    Y[i] = rand() % 100;
    Z[i] = rand() % 100;
  }
  
  display();
  
  while(isDone)
  {
    for(int i = 0 ; i < LIMIT ; i++)
    {
      if(X[i] != XFinal)
      {
        if(X[i] < XFinal)
          X[i] = X[i] + 1;
        else
          X[i] = X[i] - 1;
      }

      if(Y[i] != YFinal)
      {
        if(Y[i] < YFinal)
          Y[i] = Y[i] + 1;
        else
          Y[i] = Y[i] - 1;
      }

      if(Z[i] != ZFinal)
      {
        if(Z[i] < ZFinal)
          Z[i] = Z[i] + 1;
        else
          Z[i] = Z[i] - 1;
      }

      if(X[i] == XFinal && Y[i] == YFinal && Z[i] == ZFinal)
      {
        TotalCheck[i] = 1;
      }
    }
    
    //display();
    TotalValid = 0;
    for(int i = 0 ; i < LIMIT ; i++)
    {
      TotalValid = TotalValid + TotalCheck[i];
    }

    if(TotalValid == LIMIT)
      isDone = 0;
  
  }

  display();

  return(0);
}

void display(void)
{
  for(int i = 0 ; i < LIMIT ; i++)
  {
    printf("(%d, %d, %d) \n",X[i],Y[i],Z[i]);
  }
  printf("\n-------------------\n");
}

