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
 puts("\t  -c <n columns>		");
 exit(0);
}

/*---------------------------------------------------------------------------*/
//Inicializar matriz con parámetros aleatorios
void InitMatrizDouble(double **pMatriz, const double Rows, const double Cols)
{
 #ifdef _OPENMP
 #pragma omp parallel default(none) shared(pMatriz, Rows, Cols)
  {
  struct drand48_data RandData;
  double Random;					
  srand48_r(omp_get_thread_num(), &RandData);
  
  #pragma omp master
    {
   for (int i=0;i<Rows;i++)
      for (int j=0;j<Cols;j++)
 	{
 	drand48_r(&RandData,&Random);
        pMatriz[i][j]=Random*10;
 	}
    }
  }
}
#endif

/*---------------------------------------------------------------------------*/
void SumMatrizDouble(double **pMatriz1, double **pMatriz2, double **pMatriz3, const double Rows, const double Cols)
{
 #pragma omp parallel default(none) shared(pMatriz1,pMatriz2,pMatriz3,Rows,Cols)
 for (int i=0;i<Rows;i++)
 {
 	for (int j=0;j<Cols;j++)
 	{
 		pMatriz3[i][j]=pMatriz1[i][j] + pMatriz2[i][j];
 	}
 }
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 double Rows, Cols;
 double ** pMatriz1;
 double ** pMatriz2;
 double ** pMatriz3;

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

 #if (PRINT==1)
 printf("MinMatrizDouble: Rows=%f Cols=%f.\n\n", Rows, Cols);
 #endif
 
 # ifdef _OPENMP
 #pragma omp parallel default(none)
     { 
 	  #pragma omp single 
      printf("MinMatrizDouble OMP with %1d threads\n", 
      		 omp_get_num_threads());
     } 
  # pragma omp barrier    
 # endif     
   //Init seed of randum number generator
   //srand48(time(NULL));

   //Calloc de Getmen put data to zeroes
 pMatriz1  = (double **) GetMem2D(Rows,Cols,sizeof(double),"Main:Matriz");
 pMatriz2  = (double **) GetMem2D(Rows,Cols,sizeof(double),"Main:Matriz");
 pMatriz3  = (double **) GetMem2D(Rows,Cols,sizeof(double),"Main:Matriz");
   //Se llaman a las funciones en el orden de inicializar, imprimir, luego busca el mínimo y por ultimo lo muestra
   
 #if (DEBUG==1)
 gettimeofday(&tv1, &tz);
 #endif
 InitMatrizDouble (pMatriz1,Rows,Cols);
 InitMatrizDouble (pMatriz2,Rows,Cols);
   
 #if (DEBUG==1)
 gettimeofday(&tv2, &tz);
 #endif
   
 #if (PRINT==1)
 PrintMDpuble(stdout,pMatriz1,Rows,Cols);
 PrintMDpuble(stdout,pMatriz2,Rows,Cols);
 #endif
 
 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif

 SumMatrizDouble (pMatriz1,pMatriz2,pMatriz3,Rows,Cols);
  
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar la matriz         = %12.4g sec\n", TInitV);
 double TMinV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar la matriz                = %12.4g sec\n", TMinV);
 printf("Tiempo de inicializar y sumar la matriz  = %12.4g sec\n", 
 TInitV+TMinV);
 #endif
 
  #if (PRINT==1)
 PrintMDpuble(stdout,pMatriz3,Rows,Cols);
 #endif

 //Y se libera la memoria utilizada 
 free((void **)pMatriz1);
 free((void **)pMatriz2);
 free((void **)pMatriz3);
 
   return 0;
}


