/*
Progam that generate two Matrizs (1 and 2) of random ints in [0,9] and
performs Matriz3[i]=Matriz1[i]+Matriz2[i]
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <mpi.h>

#define PRINT 0
#define DEBUG 0

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n cols>			");
 exit(0);
}

/*---------------------------------------------------------------------------*/
//Inicializar matriz con parámetros aleatorios
void InitMatrizInt(int *pMatriz, const int NElements)
{
 for (int i=0;i<NElements;i++)
 	pMatriz[i]=lrand48()%10;
}

/*---------------------------------------------------------------------------*/
//Busca el mínimo entre los parámetros de la matriz
void Add2MatrizInt(int ** pMatriz1, int ** pMatriz2, int ** pMatriz3, const int Rows, const int Cols)
{
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		pMatriz3[i][j]=pMatriz1[i][j]+pMatriz2[i][j];
 	}
 }
}

/*---------------------------------------------------------------------------*/
//Print a matriz of size Rows
void PrintMatrizInt(int *pMatriz, const int Rows, const int Cols, char * String)
{
 printf("%s -----------------------------------------\n", String);
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 	printf("%2d,", pMatriz[i * Cols + j]);
 	}
     printf("\n");
 }
 puts("");
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 int * pMatriz1=NULL, * pMatriz2=NULL, * pMatriz3=NULL;
 int * pSubMatriz1=NULL, * pSubMatriz2=NULL, * pSubMatriz3=NULL;
 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int * pPosSubV;			//Vector with position each task starts in Vectors.
 int * pSizeSubV;			//Vector with size of subvector.
 int Rem; 				//Remainder
 int AddPos;			//Adding positions.
 
 //MPI Initialization
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

 if (MyRank==0)	//Task 0--
    {
    
	 if (ExistArg("-h",argc,argv))
    	ParametersError();  
	
	 if (!ExistArg("-r",argc,argv))
	    {
	    fputs("Parameter -r is neccesary.\n",stderr);
	    ParametersError();
	    }
	 else
	    {
	     Rows = atoi(GetArg("-r",argc,argv));
	     if (Rows <1)
	        {
	         puts("Rows<1");
	         exit(1);
	        }
	    } 
	 if (!ExistArg("-c",argc,argv))
	    {
	    fputs("Parameter -c is neccesary.\n",stderr);
	    ParametersError();
	    }
	 else
	    {
	     Cols = atoi(GetArg("-c",argc,argv)); 
	     if (Cols <=3)
	        {
	         puts("Col<=3");
	         exit(1);
	        }
	    }
	 #if (PRINT==1)
	 printf("Rows=%d.\n", Rows);
	 printf("Cols=%d.\n", Cols);
	 #endif
	 //Init seed of random number generator
	 srand48(MyRank);
	
	 //Calloc de Getmen put data to zeroes
	
	 pMatriz1  =  (int *) GetMem(Rows*Cols,sizeof(int),"Main:Matriz1");
	 pMatriz2  =  (int *) GetMem(Rows*Cols,sizeof(int),"Main:Matriz2");
	 pMatriz3  =  (int *) GetMem(Rows*Cols,sizeof(int),"Main:Matriz3");
 
	 //Init values of Matriz1 and 2
	 #if (DEBUG==1)
 	 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
	 gettimeofday(&tv1, &tz);
	 #endif
 	 InitMatrizInt (pMatriz1,Rows*Cols);
	 InitMatrizInt (pMatriz2,Rows*Cols);
	 #if (DEBUG==1)
	 gettimeofday(&tv2, &tz);
	 #endif
	
	 #if (PRINT==1)
	 PrintMatrizInt(pMatriz1,Rows,Cols,"Matriz1:");
 	 PrintMatrizInt(pMatriz2,Rows,Cols,"Matriz2:");
	 #endif
	 }
 
 //All tasks 
 MPI_Bcast(&Rows,1,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Bcast(&Cols,1,MPI_INT,0,MPI_COMM_WORLD);
 //Get men and initialize DisplacV and SizeSubV
 pPosSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:DisplacV");
 pSizeSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:SizeSubV");
 
 Rem=(Rows*Cols)%NTasks; 
 AddPos=0;
 for (int i=0; i<NTasks; i++) 
     {
      pSizeSubV[i] = (Rows*Cols)/NTasks;
      if (Rem > 0) 
         {
          pSizeSubV[i]++;
          Rem--;
         }

     pPosSubV[i] = AddPos;
     AddPos += pSizeSubV[i];
    }

 #if (PRINT==1)
 printf("Task %d out of %d, responsible elements: [%d,%d].\n",
      MyRank,NTasks,pPosSubV[MyRank],pPosSubV[MyRank]+pSizeSubV[MyRank]-1);
 #endif
 
 pSubMatriz1  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector1");
 pSubMatriz2  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector2");
 pSubMatriz3  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector3");

 //All do de Scatterv
 MPI_Scatterv(pMatriz1,   pSizeSubV, pPosSubV, MPI_INT, 
              pSubMatriz1,pSizeSubV[MyRank],MPI_INT, 
              0, MPI_COMM_WORLD);
 MPI_Scatterv(pMatriz2,    pSizeSubV, pPosSubV, MPI_INT, 
              pSubMatriz2, pSizeSubV[MyRank],MPI_INT, 
              0, MPI_COMM_WORLD);
 
 #if (PRINT==1)
 printf("Task %d scater vectors:\n", MyRank);
 PrintMatrizInt(pSubMatriz1,pSizeSubV[MyRank],Cols,"SubVector1:");
 PrintMatrizInt(pSubMatriz2,pSizeSubV[MyRank],Cols,"SubVector2:");
 #endif

 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif

 //Perform the partial component wise sum
 for (int i=0;i<pSizeSubV[MyRank];i++)
     pSubMatriz3[i]=pSubMatriz1[i]+pSubMatriz2[i];

 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Matrizs initialization time            = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Time to add the Matrizs                = %12.4g sec\n", TSumV);
 printf("Time to initialize and add the Matrizs = %12.4g sec\n", 
         TInitV+TSumV);
 #endif

 MPI_Gatherv(pSubMatriz3, pSizeSubV[MyRank], MPI_INT,
             pMatriz3,    pSizeSubV, pPosSubV, MPI_INT,
             0, MPI_COMM_WORLD);
 
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubMatriz3);
 free((void *)pSubMatriz2);
 free((void *)pSubMatriz1);

 if (MyRank==0)
    {
     #if (PRINT==1)
     PrintMatrizInt(pMatriz3,Rows,Cols,"Matriz3:");
     #endif
     free((void *)pMatriz3);
     free((void *)pMatriz2);
     free((void *)pMatriz1);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
