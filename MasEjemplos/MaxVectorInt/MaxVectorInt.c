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

#define DEBUG 1
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
//Init values of a vector of size Rows.
void InitVectorInt(int *pVector, const int Rows)
{ 
 for (int i=0;i<Rows;i++)
     pVector[i]=lrand48()%100;
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
void MaxVectorInt(int *pVector, const int Rows, int * Max)
{
 int LocalMax=pVector[0];
 for (int i=1;i<Rows;i++)
      LocalMax = LocalMax > pVector[i] ? LocalMax : pVector[i];
 *Max=LocalMax;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector, Max;


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

 printf("Rows=%d.\n", Rows);

 //Init seed of random number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 #if (DEBUG==1)
 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
 gettimeofday(&tv1, &tz);
 #endif
 
 //Init values of Vector
 InitVectorInt (pVector,Rows);
 
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif

 #if (PRINT==1)
 PrintVectorInt(pVector,Rows,"Vevtor1");
 #endif
 
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 MaxVectorInt(pVector,Rows,&Max);
 
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar el vector        = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de max del vector               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y max del vector = %12.4g sec\n", 
         TInitV+TSumV);
 #endif
 
 #if (PRINT==1)
 printf("Max=%d.\n",Max);
 #endif

 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
