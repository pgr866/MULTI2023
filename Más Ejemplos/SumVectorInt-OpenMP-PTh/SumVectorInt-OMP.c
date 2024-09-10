#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <omp.h>


#define PRINT 1

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
  //Inside a omp paralel: These variables are local to each thread.
  struct drand48_data RandData; 
  long Random;
  srand48_r(omp_get_thread_num(), &RandData);
 
  #pragma omp for 
  for (int i=0;i<Rows;i++)
  	  {
  	    lrand48_r(&RandData,&Random);
       pVector[i]=Random%10;
      }
 }
}

/*---------------------------------------------------------------------------*/
void SumVectorInt(int *pVector, const int Rows, int *pSum)
{
 int Sum=0;

 #pragma omp parallel for default(none) shared(pVector,Rows) reduction(+:Sum) 
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
 int Sum=0;

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
 
 InitVectorInt (pVector,Rows);
 
 #if (PRINT==1)
 PrintVInt(stdout,pVector,Rows);
 #endif
 
 SumVectorInt  (pVector,Rows, &Sum);
 
 printf("SumVectorInt=%d.\n",Sum);

 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
