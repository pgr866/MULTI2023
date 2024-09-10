#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <omp.h>

#define PRINT 1
#define DEBUG 1

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
 #pragma omp parallel default(none) shared(pVector,Rows)
 {
  struct drand48_data RandData; 				//For each thread
  long Random;							//For each thread
  srand48_r(omp_get_thread_num(), &RandData);	//For each thread
 
  #pragma omp for 
  for (int i=0;i<Rows;i++)
  	  {
  	   lrand48_r(&RandData,&Random);
       pVector[i]=Random%10;
      }
 }
}

/*---------------------------------------------------------------------------*/
void MinVectorInt(int *pVector, const int Rows, int *pMin)
{
 #pragma omp parallel for default(none) shared(pVector,Rows,pMin)
 for (int i=0;i<Rows;i++)
      	{
 		if(pVector[i] < *pMin)
 		{
 		 	*pMin = pVector[i];
 		}
 	}
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector;
 int Min = 10;
 
 struct timeval tv1,tv2,tv3,tv4; 
 struct timezone tz;

 //IMPORTANT: This program only works for OPENMP.
 #ifndef _OPENMP
 printf("Compilado sin -fopenmp.\n");
 exit(1);
 #endif

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

 #if (PRINT==1)
 printf("SumVectorInt: Rows=%d.\n", Rows);
 #endif
 
 #pragma omp parallel default(none) shared(Rows)
     { 
 	  #pragma omp single 
      printf("SumVectorInt OMP with %d threads, Rows=%d\n", 
      		 omp_get_num_threads(),Rows);
     } 

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 #if (DEBUG==1)
 gettimeofday(&tv1, &tz);
 #endif
 
 InitVectorInt(pVector,Rows);
 
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif
 
 #if (PRINT==1)
 PrintVInt(stdout,pVector,Rows);
 printf("\n");
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 MinVectorInt(pVector,Rows, &Min);
 
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar el vector         = %12.4g sec\n", TInitV);
 double TMinV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar el vector               = %12.4g sec\n", TMinV);
 printf("Tiempo de inicializar y sumar el vector = %12.4g sec\n", 
 TInitV+TMinV);
 #endif
 
 printf("MinVectorInt=%d.\n",Min);

 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
