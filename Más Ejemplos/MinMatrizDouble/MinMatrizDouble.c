#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

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
void InitMatrizDouble(double **pMatriz, const double Rows, const double Cols)
{
 for (int i=0;i<Rows;i++)
 	for (int j=0;j<Cols;j++)
 	{
 	pMatriz[i][j]=drand48()*10;
 	}
}

/*---------------------------------------------------------------------------*/
//Muestra por pantalla
void PrintMatrizDouble(double **pMatriz, const double Rows, const double Cols)
{
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 	printf("[%f]", pMatriz[i][j]);
 	}
  printf("\n");
 }
}

/*---------------------------------------------------------------------------*/
//Busca el mínimo entre los parámetros de la matriz
void MinMatrizDouble(double **pMatriz, const double Rows, const double Cols, double *pMin)
{
 double Min = 10;

 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		if(pMatriz[i][j] < Min)
 		{
 		 	Min = pMatriz[i][j];
 		}
 	}
 }
 *pMin=Min;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 double Rows, Cols;
 double ** pMatriz;
 double Minimo = 0;

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
 printf("MinMatrizDouble: Rows=%f Cols=%f.\n\n", Rows, Cols);
 #endif

 //Init seed of randum number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pMatriz  = (double **) GetMem2D(Rows,Cols,sizeof(double),"Main:Matriz");
 
 //Se llaman a las funciones en el orden de inicializar, imprimir, luego busca el mínimo y por ultimo lo muestra
 InitMatrizDouble (pMatriz,Rows,Cols);
 PrintMatrizDouble (pMatriz,Rows,Cols);
 MinMatrizDouble (pMatriz,Rows,Cols,&Minimo);
 
 printf("\nEl valor minimo es: %f.\n", Minimo);


 //Y se libera la memoria utilizada
 free((void **)pMatriz);

 return 0;
}

