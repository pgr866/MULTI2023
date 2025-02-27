#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <mpi.h>

#define PRINT 0
#define DEBUG 0

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
     pVector[i]=lrand48()%10;
}

/*---------------------------------------------------------------------------*/
void SumVectorInt(int *pVector, const int Rows, int *pSum)
{
 int Sum=0;
 
 for (int i=0;i<Rows;i++)
     Sum+=pVector[i];

 *pSum=Sum;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector    =NULL;
 int * pSubVector =NULL;
 int Sum=0;
 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int * pPosSubV;			//Vector with position each task starts in Vectors.
 int * pSizeSubV;			//Vector with size of subvector.
 int Rem; 				//Remainder
 int AddPos;			//Adding positions.
 
 
 //MPI Initialization
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
 
 if (MyRank==0)	//Task 0--
    {
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
	
	 //Init seed of randum number generator
	 srand48(MyRank);
	
	 //Calloc de Getmen put data to zeroes
	 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
	 
	 #if (DEBUG==1)
	 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
	 gettimeofday(&tv1, &tz);
	 #endif
	 
	 InitVectorInt (pVector,Rows);
	 
	 #if (DEBUG==1)
	 gettimeofday(&tv2, &tz);
	 #endif
	 
	 #if (PRINT==1)
	 PrintVInt(stdout,pVector,Rows);
	 #endif
 }
 
 //All tasks 
 MPI_Bcast(&Rows,1,MPI_INT,0,MPI_COMM_WORLD);
 //Get men and initialize DisplacV and SizeSubV
 pPosSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:DisplacV");
 pSizeSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:SizeSubV");
 
 Rem=Rows%NTasks; 
 AddPos=0;
 for (int i=0; i<NTasks; i++) 
     {
      pSizeSubV[i] = Rows/NTasks;
      if (Rem > 0) 
         {
          pSizeSubV[i]++;
          Rem--;
         }

     pPosSubV[i] = AddPos;
     AddPos += pSizeSubV[i];
    }

 #if (PRINT==1)
 printf("Task %d out of %d, responsible elements: [%d,%d].\n",
      MyRank,NTasks,pPosSubV[MyRank],pPosSubV[MyRank]+pSizeSubV[MyRank]-1);
 #endif
 
 pSubVector  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector");

 //All do de Scatterv
 MPI_Scatterv(pVector,   pSizeSubV, pPosSubV, MPI_INT, 
              pSubVector,pSizeSubV[MyRank],MPI_INT, 
              0, MPI_COMM_WORLD);
 
 #if (PRINT==1)
 printf("Task %d scater vectors:\n", MyRank);
 PrintVectorInt(pSubVector,pSizeSubV[MyRank],"SubVector:");
 #endif

  #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif

 int LocalSum=0;
 //Perform the partial component wise sum
 for (int i=0;i<pSizeSubV[MyRank];i++)
     LocalSum+=pSubVector[i];
     
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar el vector         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de sumar el vector               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y sumar el vector = %12.4g sec\n", 
         TInitV+TSumV);
 #endif

 MPI_Reduce(&LocalSum, &Sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
 
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubVector);

 if (MyRank==0)
    {
     #if (PRINT==1)
     printf("SumVectorInt=%d.\n",Sum);
     #endif
     free((void *)pVector);
    }

 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
