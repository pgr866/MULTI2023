
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#define PRINT 1
#define DEBUG 1

//To avoid repeating struct drand48_data.
typedef struct drand48_data StDrand48Data;

//I'll use the same structure to init and to sum the Matriz.
typedef struct th_args
{
 double **	pMatriz;
 int 		Rows;
 int 		Cols;
 int		rows_start;
 int 		n_sub_rows;
 double *		pMin;
 StDrand48Data   * pRandData;		//For drand48_r().
 pthread_mutex_t * pth_Min_mutex;	//To update *pMin in exclusive way.
 int 		ThreadId;
} Type_Th_Args;


//---------------------------------------------------------------------------//
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n cols>			");
 puts("\t  -nt <n trheads>			");
 exit(0);
}

//---------------------------------------------------------------------------//
void HowtoShareMatriz(int ThisTh, int NTh, int NRows, int NCols, int *startR, int *nR)
{
 int n_sub_rows= NRows/NTh;			//How many sub rows.
 int n_larger_threadsR= NRows%NTh;	//Threads with more rows
 int row_start = 0;						//In which row a thread starts.
 					

 if (ThisTh < n_larger_threadsR)
    { 
     n_sub_rows = n_sub_rows + 1;
     row_start = 0 + ThisTh * n_sub_rows;
    }  
 else 
    if (n_sub_rows > 0)
       row_start = n_larger_threadsR + ThisTh * n_sub_rows;
  else 
     { // this process has only zero elements
      row_start = -1;
      n_sub_rows  =  0;
     }

 *nR = n_sub_rows;
 *startR = row_start;
 
}

//---------------------------------------------------------------------------//
void * InitMatrizDoublePTh(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int rows_start, rows_end;
 double ** pMatriz;
 double Random;
 struct drand48_data RandData;
 srand48_r(pMyData->ThreadId, &RandData);
 
 pMatriz	= pMyData->pMatriz;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
 #if (PRINT==1)
 printf("I am thread %d. Initiating rows [%d,%d] and cols [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1,
        0,
        pMyData->Cols);
 #endif
 for (int i=rows_start; i<rows_end; i++)
	 {
		for(int j=0; j<pMyData->Cols; j++){
			   	drand48_r(&RandData,&Random);
      				pMatriz[i][j]=Random*10;
	  	}
	  
	 }
 pthread_exit((void *)&(pMyData->ThreadId));
}

//---------------------------------------------------------------------------//
void * MinMatrizDoublePTh(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 double Min = 10;
 int rows_start, rows_end;
 double ** pMatriz;
 
 pMatriz	= pMyData->pMatriz;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;

 #if (PRINT==1)
 printf("I am thread %d. Doing rows [%d,%d] and cols [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1,
        0,
        pMyData->Cols);
 #endif
   
 for (int i=rows_start; i<rows_end; i++)
 {
 	for (int j=0;j<pMyData->Cols;j++)
 	{
 		if(pMatriz[i][j] < Min)
 		{
 		 	Min = pMatriz[i][j];
 		}
 	}
 }
	 
 pthread_mutex_lock(pMyData->pth_Min_mutex);
 if((*pMyData->pMin)>Min){ (*pMyData->pMin)=Min; }
 pthread_mutex_unlock(pMyData->pth_Min_mutex);
 pthread_exit((void *)&(pMyData->ThreadId));
}

//---------------------------------------------------------------------------//

int main(int argc, char **argv)
{
 int Rows;
 int Cols;
 double ** pMatriz;
 int NThreads; 					//Number of threads.
 pthread_t	*pThreads=NULL; 	//Matriz of threads. Dynamic memory. 
 //Matriz of structs with data to threads. Dinamic memory allocation.
 Type_Th_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
 double Min=15;
 pthread_mutex_t pth_Min_mutex; //To update Min
 void *status;					//Status from thread.
 StDrand48Data *pRandData=NULL; 
 
 struct timeval tv1,tv2,tv3,tv4; 
 struct timezone tz;	


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
     if (Cols <1)
        {
         puts("Cols<1");
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
 printf("MinMatrizDouble-PTh: Rows=%d, Cols=%d, NThreads=%d \n", Rows, Cols, NThreads);
 #endif

 //Init seed of randum number generator
 //srand48(time(NULL));

 /* Initialize mutex objects*/
 pthread_mutex_init(&pth_Min_mutex, NULL);

 //Calloc de Getmen put data to zeroes
 pMatriz  = (double **) GetMem2D(Rows,Cols,sizeof(double),"Main:Matriz");
 
 //Get mem for pRandData
 pRandData=(StDrand48Data *) GetMem(NThreads, sizeof(StDrand48Data), "pRandomData");
 
 //Get mem for threads
 pThreads = (pthread_t *)    GetMem(NThreads, sizeof(pthread_t), 
                                    "Main: pThreads");
 pTh_Args = (Type_Th_Args *) GetMem(NThreads, sizeof(Type_Th_Args),
                                    "Main: pTh_Data");
 
 #if (DEBUG==1)
 gettimeofday(&tv1, &tz);
 #endif
 
 //---------------------------------------------------------------------------//
 //InitMatriz in parallel.
   for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareMatriz(t,NThreads, Rows, Cols, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pMatriz		= pMatriz;
 	  pTh_Args[t].pRandData		= pRandData;
 	  pTh_Args[t].Rows		= Rows;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].ThreadId		= t;
 	  pTh_Args[t].Cols		= Cols;

      //By default threads are joinable.
      pthread_create(&pThreads[t],NULL, InitMatrizDoublePTh,(void *)&pTh_Args[t]);
     }
     
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif

 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void **) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }

 #if (PRINT==1)
 PrintMDpuble(stdout,pMatriz,Rows,Cols);
 #endif

 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
  //---------------------------------------------------------------------------//
 //Minimo de la matriz en paralelo.

  for (int t=0; t<NThreads; t++) 
     {
 	  pTh_Args[t].pMin = &Min;
 	  pTh_Args[t].pth_Min_mutex = &pth_Min_mutex;

      pthread_create(&pThreads[t],NULL,MinMatrizDoublePTh, (void *)&pTh_Args[t]);
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
     
  //---------------------------------------------------------------------------//
 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void **) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }

 //////////printf("\nSumMatrizDouble=%f.\n",Sum);
 printf("\nMinMatrizDouble=%f.\n",Min);
  
 //Free allocated memory
 free((void *)pTh_Args);
 free((void *)pThreads);
 free((void **)pMatriz);
 pthread_mutex_destroy(&pth_Min_mutex);

 return 0;
}
