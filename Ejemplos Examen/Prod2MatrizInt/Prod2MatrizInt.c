/*
Progam that generate two Matrizs (1 and 2) of random ints in [0,9] and
performs Matriz3[i]=Matriz1[i]+Matriz2[i]
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
 puts("\t  -c <n cols>			");
 exit(0);
}

/*---------------------------------------------------------------------------*/
//Inicializar matriz con parámetros aleatorios
void InitMatrizInt(int **pMatriz, const int Rows, const int Cols)
{
 for (int i=0;i<Rows;i++)
 	for (int j=0;j<Cols;j++)
 	{
 	pMatriz[i][j]=lrand48()%10;
 	}
}

/*---------------------------------------------------------------------------*/
//Busca el mínimo entre los parámetros de la matriz
void Prod2MatrizInt(int ** pMatriz1, int ** pMatriz2, int ** pMatriz3, const int Rows, const int Cols)
{
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		pMatriz3[i][j]=pMatriz1[i][j]*pMatriz2[i][j];
 	}
 }
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 int ** pMatriz1, ** pMatriz2, ** pMatriz3;


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
     if (Cols <=3)
        {
         puts("Col<=3");
         exit(1);
        }
    }
 #if (PRINT==1)
 printf("Rows=%d.\n", Rows);
 printf("Cols=%d.\n", Cols);
 #endif
 //Init seed of random number generator
 srand48(0);

 //Calloc de Getmen put data to zeroes

 pMatriz1  =  (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Matriz1");
 pMatriz2  =  (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Matriz2");
 pMatriz3  =  (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Matriz3");
 
 //Init values of Matriz1 and 2
 #if (DEBUG==1)
 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
 gettimeofday(&tv1, &tz);
 #endif
 InitMatrizInt (pMatriz1,Rows,Cols);
 InitMatrizInt (pMatriz2,Rows,Cols);
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif

 #if (PRINT==1)
 PrintMInt(stdout,pMatriz1,Rows,Cols);
 PrintMInt(stdout,pMatriz2,Rows,Cols);
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 Prod2MatrizInt(pMatriz1,pMatriz2,pMatriz3,Rows,Cols);
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Matrizs initialization time            = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Time to Prod the Matrizs                = %12.4g sec\n", TSumV);
 printf("Time to initialize and Prod the Matrizs = %12.4g sec\n", 
         TInitV+TSumV);
 #endif
 
 #if (PRINT==1)
 PrintMInt(pMatriz3,Rows,Cols);
 #endif
 
 Free2D((void **)pMatriz1, Rows);
 Free2D((void **)pMatriz2, Rows);
 Free2D((void **)pMatriz3, Rows);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
