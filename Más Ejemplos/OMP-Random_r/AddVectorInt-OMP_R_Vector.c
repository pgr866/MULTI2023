#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#ifdef _OPENMP
#  include <omp.h>
#endif

#define PRINT 0
#define DEBUG 1
#define FSHARING 0	//0: aligned_alloc, no false sharing but waste L1 memory. 
					//1: calloc, false sharing. 

//To avoid repeating struct drand48_data.
typedef struct drand48_data StDrand48Data;

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 exit(0);
}

/*---------------------------------------------------------------------------*/
void InitVectorInt(int *pVector, const int Rows)
{
 for (int i=0;i<Rows;i++)
      pVector[i]=lrand48()%100;
}

/*---------------------------------------------------------------------------*/
void PrintVectorInt(int *pVector, const int Rows)
{
 puts("-----------------------------------------");
 for (int i=0;i<Rows;i++)
     printf("%d,", pVector[i]);
 puts("");
}

/*---------------------------------------------------------------------------*/
void SumVectorInt(int *pVector, const int Rows, long long int *pSum)
{
 long long int Sum = 0;
 # ifdef _OPENMP
 #pragma omp parallel for default(none) shared(pVector,Rows) reduction(+:Sum) 
 #endif
 for (int i=0;i<Rows;i++)
     Sum+=pVector[i];
 
 //Sum was reduced by all threads in exclusive way.
 *pSum=Sum;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector;
 long long int Sum=0;
 
 #ifdef _OPENMP //Parallel
  #if (FSHARING==1)
  StDrand48Data * pRandData=NULL;//Vector of RandData, one for each thread.
  #endif 
  #if (FSHARING==0)
  //Vector of NThreads pointers to RandData, one for each thread.
  StDrand48Data ** ppRandData=NULL; 
  #endif
 #endif //Parallel

 if (ExistArg("-h",argc,argv))
    ParametersError();  

 if (!ExistArg("-r",argc,argv))
    {
    fputs("Parameter -r is necessary.\n",stderr);
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
 
 #ifndef _OPENMP
 printf("SumVectorInt sequential: rows=%d \n", Rows);
 //Init seed of randum number generator
 srand48(time(NULL));
 #endif

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 
 #ifdef _OPENMP //Parallel
  int NThreads=omp_get_max_threads();
  
  //--------------
  #if (FSHARING==1) 
   printf("SumVectorInt_OMP_R_Vector with %d threads, Rows=%d.\n", NThreads,Rows);
   printf("Using calloc for pRandData.\n");
   //Get contious memory for a vector of size Nthreads with StDrand48Data.
   pRandData= (StDrand48Data *) GetMem(NThreads, 
									   sizeof(StDrand48Data), 
									   "Main: pRandData");

   //First touch: *pRandData[MyThNum] goes to L1 where pthread MyThNum runs.
   #pragma omp parallel default(none) shared(pRandData)
   {
    int MyThNum=omp_get_thread_num();
    srand48_r(time(NULL)*MyThNum, &pRandData[MyThNum]);
   }
  #endif //FSHARING==1
  
  //--------------
  #if (FSHARING==0) 
   long int CacheLineSize=sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
   printf("SumVectorInt-OMP_R_Vector with %d threads, Rows=%d.\n", NThreads,Rows);
   printf("Using aligned_alloc for *ppRandata[i].\n");
   printf("Cache line size   = %ld.\n",CacheLineSize);
   printf("drand48_data size = %lu \n", sizeof(StDrand48Data));

   ////Get memory for a vector of size Nthreads with pointers to StDrand48Data.
   ppRandData= (StDrand48Data **) GetMem(NThreads, 
 										 sizeof(StDrand48Data *), 
										 "Main: pRandData");

   //Get aligned memory for each StDrand48Data.
   for(int i=0;i<NThreads;i++)
	ppRandData[i] = (StDrand48Data *) aligned_alloc(CacheLineSize, 
 					 sizeof(StDrand48Data));

   //First touch: **ppRandData[MyThNum] goes to L1 where pthread MyThNum runs.
   #pragma omp parallel default(none) shared(ppRandData)
   {
    int MyThNum=omp_get_thread_num();
    srand48_r(time(NULL)*MyThNum, ppRandData[MyThNum]);
   }
  #endif //FSHARING==0
 #endif

 #if (DEBUG==1)
 struct timeval tv1,tv2; struct timezone tz;
 gettimeofday(&tv1, &tz);
 #endif //DEBUG
 
 #ifndef _OPENMP //Sequential.
 InitVectorInt (pVector,Rows);
 #endif
 
 #ifdef _OPENMP
  #if (FSHARING==1) //pRandData is used.
   #pragma omp parallel default(none) shared(pVector, Rows, pRandData)
  #endif
  #if (FSHARING==0) //pRandData is used.
   #pragma omp parallel default(none) shared(pVector, Rows, ppRandData)
  #endif
   {
    int MyThNum=omp_get_thread_num();
    long RandNumber;
    #pragma omp for
    for (int i=0;i<Rows;i++){
     #if (FSHARING==1) //pRandData is used.
      lrand48_r(&pRandData[MyThNum],&RandNumber);
     #endif
     #if (FSHARING==0) //pRandData is used.
      lrand48_r(ppRandData[MyThNum],&RandNumber);
     #endif
     pVector[i]=RandNumber%10;
    }
   }
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 printf("T. Init Vector    = %g sec.\n",
       (tv2.tv_sec-tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6);
 #endif
 
 #if (PRINT==1)
 PrintVInt(stdout, pVector, Rows);
 #endif //PRINT
 
 #if (DEBUG==1)
 struct timeval tv3,tv4;
 gettimeofday(&tv3, &tz);
 #endif //DEBUG
 
 SumVectorInt(pVector,Rows, &Sum);
 
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 printf("T. Add Vector     = %g sec.\n",
       (tv4.tv_sec-tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6);
 #endif
 
 printf("SumVectorInt      = %lld.\n",Sum);

 //Free allocated memory
 free((void *)pVector);
 
 #ifdef _OPNMP
  #if (FSHARING==1) //pRandData was used.
  free((void *)pRandData);
  #endif
  #if (FSHARING==0) //ppRandData was used.
  for (int i=0;i<NThreads;i++)
	  free((void*)ppRandData[i]);
  free((void *)ppRandData);
  #endif
 #endif //Parallel

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
