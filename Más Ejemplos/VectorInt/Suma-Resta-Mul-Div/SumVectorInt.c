#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

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
 for (int i=0;i<Rows;i++)
     pVector[i]=drand48()*10;
}

/*---------------------------------------------------------------------------*/
void SumVectorInt(int *pVector1, int *pVector2, int *pVector3, const int Rows)
{
 for (int i=0;i<Rows;i++)
 {
 	pVector3[i]=pVector1[i] + pVector2[i];
 }
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector1;
 int * pVector2;
 int * pVector3;
 
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

 #if (PRINT==1)
 printf("MinVectorInt: Rows=%d.\n", Rows);
 #endif

 //Init seed of randum number generator
 srand48(0);

 //Calloc de Getmen put data to zeroes
  pVector1  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
  pVector2  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
  pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 #if (DEBUG==1)
 gettimeofday(&tv1, &tz);
 #endif
 
 InitVectorInt(pVector1,Rows);
 InitVectorInt(pVector2,Rows);
 
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif
 
 
 #if (PRINT==1)
 PrintVInt(stdout,pVector1,Rows);
 PrintVInt(stdout,pVector2,Rows);
 printf("\n");
 #endif
 
  #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 SumVectorInt(pVector1,pVector2,pVector3,Rows);
 
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar el vector         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar el vector               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y sumar el vector = %12.4g sec\n", 
         TInitV+TSumV);
 #endif
 
  #if (PRINT==1)
 PrintVInt(stdout,pVector3,Rows);
 #endif
 
 free((void *)pVector1);
 free((void *)pVector2);
 free((void *)pVector3);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
