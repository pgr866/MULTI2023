/*
Progam returns the maximum of the elements of a vector of ints
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <pthread.h>

#define DEBUG 0
#define PRINT 0

//To avoid repeating struct drand48_data.
typedef struct drand48_data StDrand48Data;

//I'll use the same structure to init and to Max the vector.
typedef struct th_args
{
 int *		pVector;
 int 		Rows;
 int		rows_start;
 int 		n_sub_rows;
 int *		pMax;
 pthread_mutex_t * pth_Max_mutex;	//To update *pMax in exclusive way.
 int 		ThreadId;
} Type_Th_Args;

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
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
//Init values of a vector of size Rows.
void * InitVectorInt(void * pTh_Args)
{ 
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int rows_start, rows_end;
 int * pVector;
 StDrand48Data RandData;
 long RandNumber;
 
 srand48_r(pMyData->ThreadId,&RandData);
 
 pVector	= pMyData->pVector;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
 #if (PRINT==1)
 printf("I am thread %d. Initiating rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 for (int i=rows_start; i<rows_end; i++)
	 {
	  lrand48_r(&RandData,&RandNumber);
	  pVector[i]=RandNumber%100;
	  
	 }
 pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
//Print a vector of size Rows
void PrintVectorInt(int *pVector, const int Rows, char * String)
{
 printf("%s -----------------------------------------\n", String);
 for (int i=0;i<Rows;i++)
     printf("%2d,", pVector[i]);
 puts("");
}

/*---------------------------------------------------------------------------*/
//Max of elements of a vector
void * MaxVectorInt(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int rows_start, rows_end;
 int * pVector;
 
 pVector	= pMyData->pVector;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
 int Max=pVector[0];
 
 #if (PRINT==1)
 printf("I am thread %d. Doing rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 for (int i=rows_start; i<rows_end; i++)
	 Max = Max > pVector[i] ? Max : pVector[i];
	 
 pthread_mutex_lock(pMyData->pth_Max_mutex);
 if (Max > (*pMyData->pMax))
 {
 (*pMyData->pMax)=Max;
 }
 pthread_mutex_unlock(pMyData->pth_Max_mutex);
 
 pthread_exit((void *)&(pMyData->ThreadId));

}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector, Max;
 int NThreads; 					//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
 pthread_mutex_t pth_Max_mutex; //To update Max
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


 printf("Rows=%d.\n", Rows);

 //Init seed of random number generator
 //srand48(time(NULL));

 /* Initialize mutex objects*/
 pthread_mutex_init(&pth_Max_mutex, NULL);

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 //Get mem for threads
 pThreads = (pthread_t *)    GetMem(NThreads, sizeof(pthread_t), 
                                    "Main: pThreads");
 pTh_Args = (Type_Th_Args *) GetMem(NThreads, sizeof(Type_Th_Args),
                                    "Main: pTh_Data");
 
 #if (DEBUG==1)
 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
 gettimeofday(&tv1, &tz);
 #endif
 
 //==========================================================================
 //InitVector in parallel.
   for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareVector(t,NThreads, Rows, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pVector		= pVector;
 	  pTh_Args[t].Rows		= Rows;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].ThreadId		= t;

      //By default threads are joinable.
      pthread_create(&pThreads[t],NULL, InitVectorInt,(void *)&pTh_Args[t]);
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
 PrintVectorInt(pVector,Rows,"Vevtor1");
 #endif
 
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 //===========================================================================
 //Max the vector in parallel.

  for (int t=0; t<NThreads; t++) 
     {
 	  pTh_Args[t].pMax		= &Max;
 	  pTh_Args[t].pth_Max_mutex 	= &pth_Max_mutex;

      pthread_create(&pThreads[t],NULL,MaxVectorInt, (void *)&pTh_Args[t]);
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
 printf("Tiempo de inicializar el vector        = %12.4g sec\n", TInitV);
 double TMaxV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de max del vector               = %12.4g sec\n", TMaxV);
 printf("Tiempo de inicializar y max del vector = %12.4g sec\n", 
         TInitV+TMaxV);
 #endif
 
 #if (PRINT==1)
 printf("Max=%d.\n",Max);
 #endif
 
 //Free allocated memory
 free((void *)pTh_Args);
 free((void *)pThreads);
 free((void *)pVector);
 pthread_mutex_destroy(&pth_Max_mutex);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
