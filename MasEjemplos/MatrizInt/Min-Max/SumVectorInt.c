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
 puts("\t  -c <n columns>		");
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
void MinMatrizInt(int **pMatriz, const int Rows, const int Cols, int *pMin)
{
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		if(pMatriz[i][j] < *pMin)
 		{
 		 	*pMin = pMatriz[i][j];
 		}
 	}
 }
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 int ** pMatriz;
 int Minimo = 10;
 
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

 #if (DEBUG==1)
 printf("MinMatrizInt: Rows=%d Cols=%d.\n\n", Rows, Cols);
 #endif

 //Init seed of randum number generator
 srand48(0);

 //Calloc de Getmen put data to zeroes
 pMatriz  = (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Matriz");
 
  #if (DEBUG==1)
 gettimeofday(&tv1, &tz);
 #endif
 
 //Se llaman a las funciones en el orden de inicializar, imprimir, luego busca el mínimo y por ultimo lo muestra
 InitMatrizInt (pMatriz,Rows,Cols);
 
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif
 
 #if (PRINT==1)
 PrintMInt(stdout,pMatriz,Rows,Cols);
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
   
 MinMatrizInt (pMatriz,Rows,Cols,&Minimo);
 
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar la matriz         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar la matriz               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y sumar la matriz = %12.4g sec\n", 
 TInitV+TSumV);
 #endif
 
 printf("\nEl valor minimo es: %d.\n", Minimo);

 //Y se libera la memoria utilizada
 
 free((void **)pMatriz);

 return 0;
}

