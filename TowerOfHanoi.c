#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define NUM 3

int main()
{
	// declaration of variables
	int iNumOfPiece = NUM;		// handle this on keypress


	int *Src = NULL;
	int *Aux = NULL;
	int *Des = NULL;

	int iTotalNumOfItr;

	int SrcTop = -1;
	int AuxTop = -1;
	int DesTop = -1;

	// code
	iTotalNumOfItr = pow(2,iNumOfPiece) - 1;

	Src = (int *)malloc(sizeof(int) * iNumOfPiece);
	Aux = (int *)malloc(sizeof(int) * iNumOfPiece);
	Des = (int *)malloc(sizeof(int) * iNumOfPiece);

	for(int i = 0 ; i < iNumOfPiece ; i++)
	{
		Src[i] = iNumOfPiece - i;
		SrcTop++;
		Aux[i] = 99;
		Des[i] = 99;
	}

	printf("\n I N I T I A L  S T A G E \n");
	
	printf("\nPrinting Stacks\n");
	for(int i = iNumOfPiece-1 ; i >= 0 ; i--)
	{
		printf("S : %d | ",Src[i]);
		printf("A : %d | ",Aux[i]);
		printf("D : %d | ",Des[i]);
		printf("\n");
	}

	printf("\nPrinting Stack pointer\n");
	printf("S : %d | A : %d | D : %d\n",SrcTop, AuxTop , DesTop);

	for(int i = 1 ; i <= iTotalNumOfItr ; i++)
	{
		// 0 : a - d
		// 1 : s - d
		// 2 : s - a
		
		int iMod = i % 3;
		printf("\n---------------------------------------\n");
		printf("\n i = %d and iMod = %d\n",i , iMod);
		switch(iMod)
		{
			case 0:
				printf("\nLegal move between Aux and Des\n");
				if(AuxTop == -1)
				{
					AuxTop++;
					Aux[AuxTop] = Des[DesTop];
					Des[DesTop] = 99;
					DesTop--;
				}
				else if(DesTop == -1)
				{
					DesTop++;
					Des[DesTop] = Aux[AuxTop];
					Aux[AuxTop] = 99;
					AuxTop--;
				}
				else if(Aux[AuxTop] < Des[DesTop])
				{
					printf("\nAux %d to Des %d \n",Aux[AuxTop],Des[DesTop]);

					DesTop++;
					
					int temp = Des[DesTop];
					Des[DesTop] = Aux[AuxTop];
					Aux[AuxTop] = temp;
					//DesTop++;
					AuxTop--;
				}
				else
				{
					printf("\nDes %d to Aux %d \n",Des[DesTop],Aux[AuxTop]);
					
					AuxTop++;

					int temp = Aux[AuxTop];
					Aux[AuxTop] = Des[DesTop];
					Des[DesTop] = temp;
					//AuxTop++;
					DesTop--;
				}
				printf("\nPrinting Stacks\n");
				for(int i = iNumOfPiece-1 ; i >= 0 ; i--)
				{
					printf("S : %d | ",Src[i]);
					printf("A : %d | ",Aux[i]);
					printf("D : %d | ",Des[i]);
					printf("\n");
				}

				printf("\nPrinting Stack pointer\n");
				printf("S : %d | A : %d | D : %d\n",SrcTop, AuxTop , DesTop);
				break;

			case 1:
				printf("\nLegal move between Src and Des\n");
				if(SrcTop == -1)
				{
					SrcTop++;
					Src[SrcTop] = Des[DesTop];
					Des[DesTop] = 99;
					DesTop--;
				}
				else if(DesTop == -1)
				{
					DesTop++;
					Des[DesTop] = Src[SrcTop];
					Src[SrcTop] = 99;
					SrcTop--;
				}
				else if(Src[SrcTop] < Des[DesTop])
				{
					printf("\nSrc %d to Des %d \n",Src[SrcTop],Des[DesTop]);
					
					DesTop++;

					int temp = Des[DesTop];
					Des[DesTop] = Src[SrcTop];
					Src[SrcTop] = temp;
					//DesTop++;
					SrcTop--;
				}
				else
				{
					printf("\nDes %d to Src %d \n",Des[DesTop],Src[SrcTop]);
					
					SrcTop++;

					int temp = Src[SrcTop];
					Src[SrcTop] = Des[DesTop];
					Des[DesTop] = temp;
					//SrcTop++;
					DesTop--;
				}
				printf("\nPrinting Stacks\n");
				for(int i = iNumOfPiece-1 ; i >= 0 ; i--)
				{
					printf("S : %d | ",Src[i]);
					printf("A : %d | ",Aux[i]);
					printf("D : %d | ",Des[i]);
					printf("\n");
				}

				printf("\nPrinting Stack pointer\n");
				printf("S : %d | A : %d | D : %d\n",SrcTop, AuxTop , DesTop);


				break;

			case 2:
				printf("\nLegal move between Src and Aux\n");
				if(SrcTop == -1)
				{
					SrcTop++;
					Src[SrcTop] = Aux[AuxTop];
					Aux[AuxTop] = 99;
					AuxTop--;
				}
				else if(AuxTop == -1)
				{
					AuxTop++;
					Aux[AuxTop] = Src[SrcTop];
					Src[SrcTop] = 99;
					SrcTop--;
				}
				else if(Src[SrcTop] < Aux[AuxTop])
				{
					printf("\nAux %d to Src %d \n",Aux[AuxTop],Src[SrcTop]);
					
					AuxTop++;

					int temp = Aux[AuxTop];
					Aux[AuxTop] = Src[SrcTop];
					Src[SrcTop] = temp;
					//AuxTop++;
					SrcTop--;
				}
				else
				{
					printf("\nSrc %d to Aux %d \n",Src[SrcTop],Aux[AuxTop]);
					
					SrcTop++;

					int temp = Src[SrcTop];
					Src[SrcTop] = Aux[AuxTop];
					Aux[AuxTop] = temp;
					//SrcTop++;
					AuxTop--;
				}
				printf("\nPrinting Stacks\n");
				for(int i = iNumOfPiece-1 ; i >= 0 ; i--)
				{
					printf("S : %d | ",Src[i]);
					printf("A : %d | ",Aux[i]);
					printf("D : %d | ",Des[i]);
					printf("\n");
				}

				printf("\nPrinting Stack pointer\n");
				printf("S : %d | A : %d | D : %d\n",SrcTop, AuxTop , DesTop);

				break;

			default:
				printf("\nYou Should not come here (You will not come over here ;p)\n");
				
				break;
		}
	}

	printf("\nPrinting Stacks after operations\n");
	for(int i = iNumOfPiece-1 ; i >= 0 ; i--)
	{
		printf("S : %d | ",Src[i]);
		printf("A : %d | ",Aux[i]);
		printf("D : %d | ",Des[i]);
		printf("\n");
	}

	printf("\nPrinting Stack pointer\n");
	printf("S : %d | A : %d | D : %d\n",SrcTop, AuxTop , DesTop);
	return(0);
}
