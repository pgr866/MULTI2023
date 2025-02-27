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
 puts("\t  -c <n columns>		");
 exit(0);
}

/*---------------------------------------------------------------------------*/
//Inicializar matriz con parámetros aleatorios
void InitMatrizInt(int *pMatriz, const int Rows, const int Cols)
{
 for (int i=0;i<Rows*Cols;i++)
 	pMatriz[i]=lrand48()%10;
}
/*---------------------------------------------------------------------------*/
//Suma todos los parámetros de la matriz
void SumMatrizInt(int **pMatriz1, int **pMatriz2, int **pMatriz3, const int Rows, const int Cols)
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
void PrintMatrizInt (FILE * FOut, int * pMI, int Rows, int Cols)
{
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		fprintf(FOut,"%d, ",(int)pMI[i * Cols + j]);
 	}
 }
 fprintf(FOut,"\n");
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 int * pMatriz   =NULL;
 int * pSubMatriz=NULL;
 int Sum=0;
 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int * pPosSubV;			//Vector with position each task starts in Vectors.
 int * pSizeSubV;			//Vector with size of subvector.
 int Rem; 				//Remainder
 int AddPos;			//Adding positions.
 
 #if (DEBUG==1)
	 struct timeval tv1,tv2,tv3,tv4; 
 	 struct timezone tz;
 #endif
 
 
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

 	 #if (DEBUG==1)
	 printf("MinMatrizInt: Rows=%d Cols=%d.\n\n", Rows, Cols);
	 #endif
	
	 //Init seed of randum number generator
	 srand48(MyRank);
	
	 //Calloc de Getmen put data to zeroes
	 pMatriz  = (int *) GetMem(Rows*Cols,sizeof(int),"Main:Matriz");
	 
	 #if (DEBUG==1)
	 gettimeofday(&tv1, &tz);
	 #endif
	 
	 //Se llaman a las funciones en el orden de inicializar, imprimir, luego busca el mínimo y por ultimo lo muestra
	 InitMatrizInt (pMatriz,Rows,Cols);
	
	 #if (DEBUG==1)
	 gettimeofday(&tv2, &tz);
	 #endif
	 
	 #if (PRINT==1)
	 PrintMatrizInt(stdout,pMatriz,Rows,Cols);
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
 
 pSubMatriz  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector");

 //All do de Scatterv
 MPI_Scatterv(pMatriz,   pSizeSubV, pPosSubV, MPI_INT, 
              pSubMatriz,pSizeSubV[MyRank],MPI_INT, 
              0, MPI_COMM_WORLD);
 
 #if (PRINT==1)
 printf("Task %d scater vectors:\n", MyRank);
 PrintMatrizInt(stdout,pSubMatriz,pSizeSubV[MyRank]/Cols, Cols);
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 int localSum=0;
 //Perform the partial component wise sum
 for (int i=0;i<pSizeSubV[MyRank];i++)
     localSum+=pSubMatriz[i];

 MPI_Reduce(&localSum, &Sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
 
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar la matriz         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar la matriz               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y sumar la matriz = %12.4g sec\n", 
 TInitV+TSumV);
 #endif
 
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubMatriz);

 if (MyRank==0)
    {
     #if (PRINT==1)
     PrintMatrizInt(stdout,pMatriz,Rows,Cols);
     #endif
     free((void *)pMatriz);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
