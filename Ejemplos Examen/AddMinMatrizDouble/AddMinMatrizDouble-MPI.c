#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <mpi.h>

#define PRINT 0

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n columns>		");
 exit(0);
}

/*---------------------------------------------------------------------------*/
//Inicializar matriz con parámetros aleatorios
void InitMatrizDouble(double *pMatriz, const int Rows, const int Cols)
{
 for (int i=0;i<Rows*Cols;i++)
 	pMatriz[i]=drand48()*10;
}

/*---------------------------------------------------------------------------*/
//Suma todos los parámetros de la matriz
void SumMatrizDouble(double **pMatriz1, double **pMatriz2, double **pMatriz3, const int Rows, const int Cols)
{
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		pMatriz3[i][j]=pMatriz1[i][j] + pMatriz2[i][j];
 	}
 }
}

/*---------------------------------------------------------------------------*/
//Busca el mínimo entre los parámetros de la matriz
void MinMatrizDouble(double **pMatriz, const double Rows, const double Cols, double *pMin)
{
 double Min = pMatriz[0][0];

 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		if(pMatriz[i][j] < Min)
 		{
 		 	Min = pMatriz[i][j];
 		}
 	}
 }
 *pMin=Min;
}

/*---------------------------------------------------------------------------*/
void PrintMDouble (FILE * FOut, double * pMatriz, int Rows, int Cols)
{
 fprintf(FOut,"-----------------------------------------------------------\n");
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		fprintf(FOut,"%g, ",pMatriz[i*Cols+j]);
 	}
 printf("\n");
 }
 fprintf(FOut,"\n");
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 double * pMatriz1=NULL, * pMatriz2=NULL, * pMatriz3=NULL;
 double * pSubMatriz1=NULL, * pSubMatriz2=NULL, * pSubMatriz3=NULL;
 double Min = 10;
 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int * pPosSubV;			//Matriz with position each task starts in Matrizs.
 int * pSizeSubV;			//Matriz with size of subMatriz.
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

 //Init seed of randum number generator
 srand48(MyRank);

 //Calloc de Getmen put data to zeroes
 pMatriz1  = (double *) GetMem(Rows*Cols,sizeof(double),"Main:Matriz1");
 pMatriz2  = (double *) GetMem(Rows*Cols,sizeof(double),"Main:Matriz2");
 pMatriz3  = (double *) GetMem(Rows*Cols,sizeof(double),"Main:Matriz3");
 
 //Se llaman a las funciones en el orden de inicializar, imprimir, luego busca el mínimo y por ultimo lo muestra
 InitMatrizDouble (pMatriz1,Rows,Cols);
 InitMatrizDouble (pMatriz2,Rows,Cols);
 
 #if (PRINT==1)
 PrintMDouble(stdout,pMatriz1,Rows,Cols);
 PrintMDouble(stdout,pMatriz2,Rows,Cols);
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
 
 pSubMatriz1  = (double *) GetMem(pSizeSubV[MyRank],sizeof(double),
                               "SubMatriz1");
 pSubMatriz2  = (double *) GetMem(pSizeSubV[MyRank],sizeof(double),
                               "SubMatriz2");
 pSubMatriz3  = (double *) GetMem(pSizeSubV[MyRank],sizeof(double),
                               "SubMatriz3");

 //All do de Scatterv
 MPI_Scatterv(pMatriz1,   pSizeSubV, pPosSubV, MPI_DOUBLE, 
              pSubMatriz1,pSizeSubV[MyRank],MPI_DOUBLE, 
              0, MPI_COMM_WORLD);
 MPI_Scatterv(pMatriz2,    pSizeSubV, pPosSubV, MPI_DOUBLE, 
              pSubMatriz2, pSizeSubV[MyRank],MPI_DOUBLE, 
              0, MPI_COMM_WORLD);
 
 #if (PRINT==1)
 printf("Task %d scater Matrizs:\n", MyRank);
 PrintMDouble(stdout,pSubMatriz1,pSizeSubV[MyRank]/Cols,Cols);
 PrintMDouble(stdout,pSubMatriz2,pSizeSubV[MyRank]/Cols,Cols);
 #endif

 //Perform the partial component wise sum
 for (int i=0;i<pSizeSubV[MyRank];i++)
     pSubMatriz3[i]=pSubMatriz1[i]+pSubMatriz2[i];

 //Perform the partial component wise sum
  double localMin = 10;

 for (int i=0;i<pSizeSubV[MyRank];i++)
 {
 	if(pSubMatriz3[i] < localMin)
 	{
 		localMin = pSubMatriz3[i];
 	}
 }

 MPI_Gatherv(pSubMatriz3, pSizeSubV[MyRank], MPI_DOUBLE,
             pMatriz3,    pSizeSubV, pPosSubV, MPI_DOUBLE,
             0, MPI_COMM_WORLD);

 MPI_Reduce(&localMin, &Min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
             
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubMatriz3);
 free((void *)pSubMatriz2);
 free((void *)pSubMatriz1);

 if (MyRank==0)
    {
     #if (PRINT==1)
     PrintMDouble(stdout,pMatriz3,Rows,Cols);
     printf("\nMin: %f\n", Min);
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
