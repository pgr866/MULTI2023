#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"

#ifdef _OPENMP
#  include <omp.h>
#endif

#define PRINT 0
#define DEBUG 1
#define OMP_R 0 // 0: lrand() 1: lrand_r()

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
  //-----------------------------------
 #ifndef _OPENMP //Sequential
 for (int i=0;i<Rows;i++)
      pVector[i]=lrand48()%100;
 #endif // Sequential

 //-----------------------------------
 #ifdef _OPENMP //Paralell
 
  //------------
  #if (OMP_R==0) //Paralell with lrand48
  #pragma omp parallel for default(none) shared(pVector,Rows)
   for (int i=0;i<Rows;i++)
      pVector[i]=lrand48()%10;
  #endif //OMP_R=0 

  //------------
  #if (OMP_R==1) //Paralell con lrand48_r
  #pragma omp parallel default(none) shared(pVector,Rows)
  {
   struct drand48_data r_state; //Local for each thread
   long r_result;				//Local for each thread
   srand48_r(time(NULL)+omp_get_thread_num(), &r_state);//Local for each thread
 
   #pragma omp for 
   for (int i=0;i<Rows;i++)
  	   {
  	    lrand48_r(&r_state,&r_result);
        pVector[i]=r_result%100;
       }
  }
  #endif //OMP_R=1
 #endif //Parallel
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

 #if (PRINT==1)
 printf("SumVectorInt: Rows=%d.\n", Rows);
 #endif
 
 #ifndef _OPENMP
 printf("SumVectorInt sequential: rows=%d \n", Rows);
 #endif
 
 
 #ifdef _OPENMP
 #pragma omp parallel default(none) shared(Rows)
     { 
 	  #pragma omp single
 	  #if (OMP_R==0) 
      printf("SumVectorInt OMP with %d threads, Rows=%d, using lrand48() \n", 
      		 omp_get_num_threads(),Rows);
      #endif //OMP_R
      #if (OMP_R==1) 
      printf("SumVectorInt OMP with %d threads, Rows=%d, using lrand48_r() \n", 
      		 omp_get_num_threads(),Rows);
      #endif //OMP_R
     } /* end omp parallel */
 #endif

 //Init seed of randum number generator
 
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
  #if (DEBUG==1)
  struct timeval tv1,tv2; struct timezone tz;
  gettimeofday(&tv1, &tz);
  #endif //DEBUG
 
 InitVectorInt (pVector,Rows);
 
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 printf("T. Init Vector = %g sec.\n",
       (tv2.tv_sec-tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6);
 #endif
 
 #if (PRINT==1)
 PrintVectorInt(pVector,Rows);
 #endif //PRINT
 
  #if (DEBUG==1)
  struct timeval tv3,tv4;
  gettimeofday(&tv3, &tz);
  #endif //DEBUG
 
 SumVectorInt(pVector,Rows, &Sum);
 
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 printf("T. Add Vector  = %g sec.\n",
       (tv4.tv_sec-tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6);
 #endif
 
 printf("SumVectorInt   = %lld.\n",Sum);

 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
