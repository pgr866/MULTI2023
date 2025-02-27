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
void InitVectorDouble(double *pVector, const int Rows)
{
 for (int i=0;i<Rows;i++)
     pVector[i]=drand48()*10;
}

/*---------------------------------------------------------------------------*/
void MinVectorDouble(double *pVector, const int Rows, double *pMin)
{
 double Min = pVector[0];
 
 for (int i=0;i<Rows;i++)
      	{
 		if(pVector[i] < Min)
 		{
 		 	Min = pVector[i];
 		}
 	}
*pMin=Min;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 double * pVector;
 double Min = 10;

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
  pVector  = (double *) GetMem(Rows,sizeof(double),"Main:Vector");
 
 #if (DEBUG==1)
 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
 gettimeofday(&tv1, &tz);
 #endif
 
 InitVectorDouble(pVector,Rows);
 
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif
 
 
 #if (PRINT==1)
 PrintVDouble(stdout,pVector,Rows);
 printf("\n");
 #endif
 
  #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 MinVectorDouble(pVector,Rows, &Min);
 
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar el vector         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar el vector               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y sumar el vector = %12.4g sec\n", 
         TInitV+TSumV);
 #endif
 
 printf("MinVectorDouble=%f.\n",Min);
 
 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
