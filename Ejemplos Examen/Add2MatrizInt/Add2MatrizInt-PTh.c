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
#include <pthread.h>

#define PRINT 0
#define DEBUG 0

//To avoid repeating struct drand48_data.
typedef struct drand48_data StDrand48Data;

//I'll use the same structure to init and to sum the vector.
typedef struct th_args
{
 int **		pMatriz1;
 int **		pMatriz2;
 int **		pMatriz3;
 int 		Rows;
 int 		Cols;
 int		rows_start;
 int 		n_sub_rows;
 int 		ThreadId;
} Type_Th_Args;

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n cols>			");
 exit(0);
}

//-----------------------------------------------------------------------------
void HowtoShareVector(int ThisTh, int NTh, int NRows, int *start, int *n)
{
 int n_sub_rows= NRows/NTh;			//How many sub rows.
 int n_larger_threads= NRows%NTh;	//Threads with more rows
 int row_start;						//In which row a thread starts.

 if (ThisTh < n_larger_threads)
    { 
     n_sub_rows = n_sub_rows + 1;
     row_start = 0 + ThisTh * n_sub_rows;
    }  
 else 
    if (n_sub_rows > 0)
       row_start = n_larger_threads + ThisTh * n_sub_rows;
  else 
     { // this process has only zero elements
      row_start = -1;
      n_sub_rows  =  0;
     }

 *n		= n_sub_rows;
 *start	= row_start;
}

/*---------------------------------------------------------------------------*/
//Inicializar matriz con parámetros aleatorios
void * Init2MatrizInt(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int rows_start, rows_end;
 int Cols;
 int ** pMatriz1, ** pMatriz2;
 StDrand48Data RandData;
 long RandNumber;
 
 srand48_r(pMyData->ThreadId,&RandData);
 
 pMatriz1	= pMyData->pMatriz1;
 pMatriz2	= pMyData->pMatriz2;
 Cols           = pMyData->Cols;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
 #if (PRINT==1)
 printf("I am thread %d. Initiating rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 for (int i=rows_start;i<rows_end;i++)
{
 	for (int j=0;j<Cols;j++)
 	{
	  lrand48_r(&RandData,&RandNumber);
	  pMatriz1[i][j]=RandNumber%10;
	  lrand48_r(&RandData,&RandNumber);
	  pMatriz2[i][j]=RandNumber%10;
	 }
	}
 
 pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
//Busca el mínimo entre los parámetros de la matriz
void * Add2MatrizInt(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int rows_start, rows_end, Cols;
 int ** pMatriz1, ** pMatriz2, ** pMatriz3;
 
 pMatriz1	= pMyData->pMatriz1;
 pMatriz2	= pMyData->pMatriz2;
 pMatriz3	= pMyData->pMatriz3;
 Cols           = pMyData->Cols;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
 #if (PRINT==1)
 printf("I am thread %d. Doing rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 for (int i=rows_start; i<rows_end; i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		pMatriz3[i][j]=pMatriz1[i][j]+pMatriz2[i][j];
 	}
 }
 
 pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 int ** pMatriz1, ** pMatriz2, ** pMatriz3;
 int NThreads; 					//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
 void *status;					//Status from thread.

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
    
 if (!ExistArg("-nt",argc,argv))
    {
    fputs("Parameter -nt is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     NThreads = atoi(GetArg("-nt",argc,argv)); 
     if (NThreads <0 || NThreads > Rows)
        {
         puts("NThreads must be > 0 and >= Rows.");
         exit(1);
        }
    }  
    
 #if (PRINT==1)
 printf("Rows=%d.\n", Rows);
 printf("Cols=%d.\n", Cols);
 #endif
 //Init seed of random number generator
 //srand48(0);
 
 //Calloc de Getmen put data to zeroes
 pMatriz1  =  (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Matriz1");
 pMatriz2  =  (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Matriz2");
 pMatriz3  =  (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Matriz3");
 
 //Get mem for threads
 pThreads = (pthread_t *)    GetMem(NThreads, sizeof(pthread_t), 
                                    "Main: pThreads");
 pTh_Args = (Type_Th_Args *) GetMem(NThreads, sizeof(Type_Th_Args),
                                    "Main: pTh_Data");
 
 //Init values of Matriz1 and 2
 #if (DEBUG==1)
 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
 gettimeofday(&tv1, &tz);
 #endif
 
 //==========================================================================
 //Init2MAtriz in parallel.
   for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareVector(t,NThreads, Rows, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pMatriz1		= pMatriz1;
 	  pTh_Args[t].pMatriz2		= pMatriz2;
 	  pTh_Args[t].pMatriz3		= pMatriz3;
 	  pTh_Args[t].Rows		= Rows;
 	  pTh_Args[t].Cols		= Cols;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].ThreadId		= t;

      //By default threads are joinable.
      pthread_create(&pThreads[t],NULL, Init2MatrizInt,(void *)&pTh_Args[t]);
     }

 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void **) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }
     
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif

 #if (PRINT==1)
 PrintMInt(stdout,pMatriz1,Rows,Cols);
 PrintMInt(stdout,pMatriz2,Rows,Cols);
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 //===========================================================================
 //Add the matrix in parallel.

  for (int t=0; t<NThreads; t++) 
     {
          pTh_Args[t].pMatriz3		= pMatriz3;

      pthread_create(&pThreads[t],NULL,Add2MatrizInt, (void *)&pTh_Args[t]);
     }

 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void **) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }
     
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Matrizs initialization time            = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Time to add the Matrizs                = %12.4g sec\n", TSumV);
 printf("Time to initialize and add the Matrizs = %12.4g sec\n", 
         TInitV+TSumV);
 #endif
 
 #if (PRINT==1)
 PrintMInt(pMatriz3,Rows,Cols);
 #endif
 
 //Free allocated memory
 free((void *)pTh_Args);
 free((void *)pThreads);
 Free2D((void **)pMatriz1, Rows);
 Free2D((void **)pMatriz2, Rows);
 Free2D((void **)pMatriz3, Rows);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
