/*
for each point in X[] find its nearest on from final one Xfinal[]
do this while X[] not equal to Xfinal
same for Y[] and Yfinal
*/


#include<stdio.h>
#include<stdlib.h>

#define LIMIT 20
#define FINAL_LIMIT 10

int X[LIMIT], Y[LIMIT],small[LIMIT];
int Xfinal[FINAL_LIMIT], Yfinal[FINAL_LIMIT];

int main()
{
  // declaration of variables
  void initialize(void);
  void display(void);
  void findNear(void);

  int TotalCheck[LIMIT] = {0};
  int TotalValid = 0;

  int isDone = 1;

  // code
  initialize();
  findNear();
  display();
  
while(isDone)
{
  for(int i =0 ; i < LIMIT ; i++)
  {
    int finalX = Xfinal[small[i]];
    int finalY = Yfinal[small[i]];

    if(X[i] != finalX)
    {
      if(X[i] < finalX)
        X[i] = X[i] - 1;
      else
        X[i] = X[i] + 1;
    }
    if(Y[i] != finalY)
    {
      if(Y[i] < finalY)
        Y[i] = Y[i] - 1;
      else
        Y[i] = Y[i] + 1;
    }
    if(X[i] == finalX && Y[i] == finalY )
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

int diff(int X, int Xfinal)
{
  if(X > Xfinal)
    return(X - Xfinal);
  else
    return(Xfinal - X);
}

void initialize()
{
  // declaration of variables
  void display(void);
  void dispFinal(void);

  // code
  for(int i = 0 ; i < LIMIT ; i++)
  {
    X[i] = rand() % 30;
    Y[i] = rand() % 30;
  }
  display();

  for(int i = 0 ; i < FINAL_LIMIT ; i++)
  {
    Xfinal[i] = rand()%20;
    Yfinal[i] = rand()%20;
  }
  dispFinal();
}

void display(void)
{
  // code
  for(int i = 0 ; i < LIMIT ; i++)
  {
    printf("(%d , %d) : %d\n",X[i],Y[i],small[i]);
  }
  printf("\n-------------------\n");
}

void dispFinal(void)
{
  // code
  for(int i = 0 ; i < FINAL_LIMIT ; i++)
  {
    printf("(%d , %d) \n",Xfinal[i],Yfinal[i]);
  }
  printf("\n-------------------\n");
}

void findNear(void)
{
  int diff(int,int);
  for(int i = 0 ; i < LIMIT ; i++)
  {
    int totalSum = 9999999;
    for(int j = 0 ; j < FINAL_LIMIT ; j++)
    {
      int dif = diff(X[i],Xfinal[j]) + diff(Y[i],Yfinal[j]);
      
      if(dif < totalSum)
      {
        totalSum = dif;
        small[i] = j;
      }
    }      
  }
}
