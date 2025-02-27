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

//I'll use the same structure to init and to sum the Matriz.
typedef struct th_args
{
 int **		pMatriz;
 int 		Rows, Cols;
 int		rows_start;
 int 		n_sub_rows;
 int *		pSum;
 pthread_mutex_t * pth_Sum_mutex;	//To update *pSum in exclusive way.
 int 		ThreadId;
} Type_Th_Args;

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n columns>		");
 exit(0);
}

//-----------------------------------------------------------------------------
void HowtoShareMatriz(int ThisTh, int NTh, int NRows, int *start, int *n)
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
void * InitMatrizInt(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int rows_start, rows_end, Cols;
 int ** pMatriz;
 StDrand48Data RandData;
 long RandNumber;
 
 srand48_r(pMyData->ThreadId,&RandData);
 
 pMatriz	= pMyData->pMatriz;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 Cols           = pMyData->Cols;
 
 #if (PRINT==1)
 printf("I am thread %d. Initiating rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 for (int i=rows_start; i<rows_end; i++)
 {
 	for (int j=0; j<Cols; j++)
	 {
	  lrand48_r(&RandData,&RandNumber);
	  pMatriz[i][j]=RandNumber%10;
	 }
 }
 pthread_exit((void *)&(pMyData->ThreadId));
}
/*---------------------------------------------------------------------------*/
//Suma todos los parámetros de la matriz
void * SumMatrizInt(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int Sum=0;
 int rows_start, rows_end, Cols;
 int ** pMatriz;
 
 pMatriz	= pMyData->pMatriz;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 Cols           = pMyData->Cols;
 
 #if (PRINT==1)
 printf("I am thread %d. Doing rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 for (int i=rows_start; i<rows_end; i++)
 {
 	for (int j=0; j<Cols; j++)
 	{
 		Sum+=pMatriz[i][j];
 	}
 }
 
 pthread_mutex_lock(pMyData->pth_Sum_mutex);
 (*pMyData->pSum)+=Sum;
 pthread_mutex_unlock(pMyData->pth_Sum_mutex);
 
 pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 int ** pMatriz;
 int NThreads; 					//Number of threads.
 pthread_t	*pThreads=NULL; 	//Matriz of threads. Dynamic memory. 
 //Matriz of structs with data to threads. Dinamic memory allocation.
 Type_Th_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
 int Sum=0;
 pthread_mutex_t pth_Sum_mutex; //To update Sum
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

 #if (DEBUG==1)
 struct timeval tv1,tv2,tv3,tv4; 
 struct timezone tz;
 printf("MinMatrizInt: Rows=%d Cols=%d.\n\n", Rows, Cols);
 #endif

 //Init seed of randum number generator
 //srand48(0);
 
 /* Initialize mutex objects*/
 pthread_mutex_init(&pth_Sum_mutex, NULL);

 //Calloc de Getmen put data to zeroes
 pMatriz  = (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Matriz");
 
 //Get mem for threads
 pThreads = (pthread_t *)    GetMem(NThreads, sizeof(pthread_t), 
                                    "Main: pThreads");
 pTh_Args = (Type_Th_Args *) GetMem(NThreads, sizeof(Type_Th_Args),
                                    "Main: pTh_Data");
 #if (DEBUG==1)
 gettimeofday(&tv1, &tz);
 #endif
 //==========================================================================
 //InitMatriz in parallel.
   for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareMatriz(t,NThreads, Rows, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pMatriz		= pMatriz;
 	  pTh_Args[t].Rows		= Rows;
 	  pTh_Args[t].Cols		= Cols;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].ThreadId		= t;

      //By default threads are joinable.
      pthread_create(&pThreads[t],NULL, InitMatrizInt,(void *)&pTh_Args[t]);
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
 PrintMInt(stdout,pMatriz2,Rows,Cols);
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 //===========================================================================
 //Sum the Matriz in parallel.
  
  for (int t=0; t<NThreads; t++) 
     {
 	  pTh_Args[t].pSum	    = &Sum;
 	  pTh_Args[t].pth_Sum_mutex = &pth_Sum_mutex;

      pthread_create(&pThreads[t],NULL,SumMatrizInt, (void *)&pTh_Args[t]);
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
 printf("Tiempo de inicializar la matriz         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar la matriz               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y sumar la matriz = %12.4g sec\n", 
 TInitV+TSumV);
 #endif

 #if (PRINT==1)
 printf("SumMatrizInt=%d.\n",*pSum);
 #endif
 
 //Free allocated memory
 free((void *)pTh_Args);
 free((void *)pThreads);
 Free2D((void **)pMatriz, Rows);
 pthread_mutex_destroy(&pth_Sum_mutex);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
