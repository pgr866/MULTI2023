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
 int * pSubVector1=NULL, * pSubVector2=NULL, * pSubVector3=NULL;
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
 
 pSubVector1  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector1");
 pSubVector2  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector2");
 pSubVector3  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector3");

 //All do de Scatterv
 MPI_Scatterv(pVector1,   pSizeSubV, pPosSubV, MPI_INT, 
              pSubVector1,pSizeSubV[MyRank],MPI_INT, 
              0, MPI_COMM_WORLD);
 MPI_Scatterv(pVector2,    pSizeSubV, pPosSubV, MPI_INT, 
              pSubVector2, pSizeSubV[MyRank],MPI_INT, 
              0, MPI_COMM_WORLD);
 
 #if (PRINT==1)
 printf("Task %d scater vectors:\n", MyRank);
 PrintVectorInt(pSubVector1,pSizeSubV[MyRank],"SubVector1:");
 PrintVectorInt(pSubVector2,pSizeSubV[MyRank],"SubVector2:");
 #endif

 #if (DEBUG==1)
 gettimeofday(&tv3, &tz);
 #endif
 
 //Perform the partial component wise sum
 for (int i=0;i<pSizeSubV[MyRank];i++)
     pSubVector3[i]=pSubVector1[i]-pSubVector2[i];
     
 #if (DEBUG==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar los vectores         = %12.4g sec\n", TInitV);
 double TSumV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de restar los vectores               = %12.4g sec\n", TSumV);
 printf("Tiempo de inicializar y restar los vectores = %12.4g sec\n", 
         TInitV+TSumV);
 #endif

 MPI_Gatherv(pSubVector3, pSizeSubV[MyRank], MPI_INT,
             pVector3,    pSizeSubV, pPosSubV, MPI_INT,
             0, MPI_COMM_WORLD);
 
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubVector3);
 free((void *)pSubVector2);
 free((void *)pSubVector1);

 

 if (MyRank==0)
    {
     #if (PRINT==1)
     PrintVInt(stdout,pVector3,Rows);
     #endif
     free((void *)pVector3);
     free((void *)pVector2);
     free((void *)pVector1);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

