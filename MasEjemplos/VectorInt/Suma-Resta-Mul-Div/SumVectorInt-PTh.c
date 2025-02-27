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

//I'll use the same structure to init and to sum the Vector.
typedef struct th_args
{
 int *	pVector1;
 int *	pVector2;
 int *	pVector3;
 int 		Rows;
 int		rows_start;
 int 		n_sub_rows;
 StDrand48Data   * pRandData;		//For drand48_r().
 int 		ThreadId;
} Type_Th_Args;


//---------------------------------------------------------------------------//
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -nt <n trheads>			");
 exit(0);
}

//---------------------------------------------------------------------------//
void HowtoShareVector(int ThisTh, int NTh, int NRows, int *startR, int *nR)
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
void * InitVectorIntPTh(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int rows_start, rows_end;
 int * pVector1;
 int * pVector2;
 StDrand48Data RandData;
 long RandNumber;
 
 srand48_r(pMyData->ThreadId,&RandData);
 
 pVector1	= pMyData->pVector1;
 pVector2	= pMyData->pVector2;
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
      		pVector1[i]=RandNumber%10;
      		pVector2[i]=RandNumber*3/2%10;  
	 }
 pthread_exit((void *)&(pMyData->ThreadId));
}


//---------------------------------------------------------------------------//
void * SumVectorIntPTh(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;

 int rows_start, rows_end;
 int * pVector1;
 int * pVector2;
 int * pVector3;
 
 pVector1	= pMyData->pVector1;
 pVector2	= pMyData->pVector2;
 pVector3	= pMyData->pVector3;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;

 #if (PRINT==1)
 printf("I am thread %d.\n", 
        pMyData->ThreadId);
 #endif
 for (int i=rows_start; i<rows_end; i++)
	 pVector3[i]=pVector1[i] + pVector2[i];
	 
 pthread_exit((void *)&(pMyData->ThreadId));
}

//---------------------------------------------------------------------------//

int main(int argc, char **argv)
{
 int Rows;
 int * pVector1;
 int * pVector2;
 int * pVector3;
 int NThreads; 					//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
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
 printf("MinVectorInt-PTh: Rows=%d, NThreads=%d \n", Rows, NThreads);
 #endif

 //Init seed of randum number generator
 //srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pVector1  = (int *) GetMem(Rows,sizeof(int),"Main:Vector1");
 pVector2  = (int *) GetMem(Rows,sizeof(int),"Main:Vector2");
 pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector3");
 
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
 //InitVector in parallel.
   for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareVector(t,NThreads, Rows, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pVector1		= pVector1;
 	  pTh_Args[t].pVector2		= pVector2;
 	  pTh_Args[t].pVector3		= pVector3;
 	  pTh_Args[t].pRandData		= pRandData;
 	  pTh_Args[t].Rows		= Rows;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].ThreadId		= t;

      //By default threads are joinable.
      pthread_create(&pThreads[t],NULL, InitVectorIntPTh,(void *)&pTh_Args[t]);
     }
     
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif

 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void *) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }

 #if (PRINT==1)
 PrintVInt(stdout,pVector1,Rows);
 PrintVInt(stdout,pVector2,Rows);
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
  //---------------------------------------------------------------------------//
 //Sum the Vector in parallel.

  for (int t=0; t<NThreads; t++) 
     {
 	  //pTh_Args[t].pSum = &Sum;
 	  //pTh_Args[t].pth_Sum_mutex = &pth_Sum_mutex;

      pthread_create(&pThreads[t],NULL,SumVectorIntPTh, (void *)&pTh_Args[t]);
     }
     
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar la Vector         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar la Vector               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y sumar la Vector = %12.4g sec\n", 
 TInitV+TSumV);
 #endif     
     
  //---------------------------------------------------------------------------//
 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void *) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }

  #if (PRINT==1)
 PrintVInt(stdout,pVector3,Rows);
 #endif

 //Free allocated memory
 free((void *)pTh_Args);
 free((void *)pThreads);
 free((void *)pVector1);
 free((void *)pVector2);
 free((void *)pVector3);
 
 return 0;
}
