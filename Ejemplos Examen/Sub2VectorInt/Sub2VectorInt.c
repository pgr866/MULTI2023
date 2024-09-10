/*
Progam that generate two vectors (1 and 2)  of random ints in [0,9] and performs vector3[i]=vector1[i]-vector2[i].
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#define PRINT 0
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
//Init values of two vectors of size Rows.
void Init2VectorInt(int *pVector1, int *pVector2, const int Rows)
{ 
 for (int i=0;i<Rows;i++)
     {
      pVector1[i]=lrand48()%10;
      pVector2[i]=lrand48()%10;
     }
}

/*---------------------------------------------------------------------------*/
//Vector1-Vector2->Vector3
void Sub2VectorInt(int *pVector1, int *pVector2, int * pVector3,
                   const int Rows)
{
 for (int i=0;i<Rows;i++)
     pVector3[i]=pVector1[i]-pVector2[i];
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector1, * pVector2, * pVector3;


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
 printf("Rows=%d.\n", Rows);
 #endif

 //Init seed of random number generator
 srand48(0);

 //Calloc de Getmen put data to zeroes
 pVector1  = (int *) GetMem(Rows,sizeof(int),"Main:Vector1");
 pVector2  = (int *) GetMem(Rows,sizeof(int),"Main:Vector2");
 pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector3");
 
 //Init values of Vector1 and 2
#if (DEBUG==1)
 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
 gettimeofday(&tv1, &tz);
 #endif

 Init2VectorInt (pVector1,pVector2,Rows);

 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif

 #if (PRINT==1)
 PrintVInt(stdout,pVector1,Rows);
 PrintVInt(stdout,pVector2,Rows);
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 Sub2VectorInt(pVector1,pVector2,pVector3,Rows);
  #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar los vectores         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de restar los vectores               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y restar los vectores = %12.4g sec\n", 
         TInitV+TSumV);
 #endif
 
 #if (PRINT==1)
 PrintVInt(stdout,pVector3,Rows);
 #endif
 
 free((void *)pVector3);
 free((void *)pVector2);
 free((void *)pVector1);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
