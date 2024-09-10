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
//Init values of a vector of size Rows.
void InitVectorDouble(double *pVector, const int Rows)
{ 
 for (int i=0;i<Rows;i++)
     pVector[i]=drand48()*10;
}

/*---------------------------------------------------------------------------*/
//Busca el mínimo entre los parámetros de la matriz
void MinMatrizDouble(double **pMatriz, const int Rows, const int Cols, double *pMin)
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
//Print a vector of size Rows
void PrintVectorDouble(double *pVector, const int Rows, const int Cols, char * String)
{
    printf("%s -----------------------------------------\n", String);
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            printf("%f,", pVector[i * Cols + j]);
        }
        printf("\n");
    }

    puts("");
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 double * pMatriz   =NULL;
 double * pSubMatriz=NULL;
 double Min = 10.0;
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
	
	#if (DEBUG==1)
        struct timeval tv1,tv2,tv3,tv4; 
 	struct timezone tz;
	printf("MinMatrizDouble: Rows=%f Cols=%f.\n\n", Rows, Cols);
	#endif
	
	//Init seed of randum number generator
	srand48(MyRank);
	
	//Calloc de Getmen put data to zeroes
	pMatriz  = (double *) GetMem(Rows*Cols,sizeof(double),"Main:Matriz");
	 
	#if (DEBUG==1)
	gettimeofday(&tv1, &tz);
	#endif
	 
	//Se llaman a las funciones en el orden de inicializar, imprimir, luego busca el mínimo y por ultimo lo muestra
	InitVectorDouble (pMatriz,Rows*Cols);
	 
	#if (DEBUG==1)
	gettimeofday(&tv2, &tz);
	#endif
	 
	#if (PRINT==1)
	PrintVectorDouble(pMatriz,Rows,Cols,"Matriz");
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
 
 pSubMatriz = (double *) GetMem(pSizeSubV[MyRank],sizeof(double),"SubMatriz");
 
 //All do de Scatterv
 MPI_Scatterv(pMatriz, pSizeSubV, pPosSubV, MPI_DOUBLE, 
              pSubMatriz, pSizeSubV[MyRank], MPI_DOUBLE, 
              0, MPI_COMM_WORLD);
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif

 //Perform the partial component wise sum
 double LocalMin = 10.0;
 
 for (int i=0;i<pSizeSubV[MyRank];i++)
 {
 	if(pSubMatriz[i] < LocalMin)
 		{
 		 	LocalMin = pSubMatriz[i];
 		}
 }
     
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar la matriz         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar la matriz               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y sumar la matriz = %12.4g sec\n", 
 TInitV+TSumV);
 #endif
            
 MPI_Reduce(&LocalMin, &Min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
 
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubMatriz);

 if (MyRank==0)
    {
     #if (PRINT==1)
     printf("\nEl valor minimo es: %f.\n", Min);
     #endif
     free((void *)pMatriz);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
