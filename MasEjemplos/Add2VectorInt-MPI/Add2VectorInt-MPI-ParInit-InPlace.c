/*
Progam that generate two vectors (1 and 2) of random ints in [0,9] and
performs Vector3[i]=Vector1[i]+Vector2[i]
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

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
     pVector[i]=lrand48()%10;
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
//Adds Vector1 and Vector2 in Vector3
void Add2VectorInt(int *pVector1, int *pVector2, int * pVector3,
                   const int Rows)
{
 for (int i=0;i<Rows;i++)
     pVector3[i]=pVector1[i]+pVector2[i];
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector3   =NULL;
 int * pSubVector1=NULL, * pSubVector2=NULL, * pSubVector3=NULL;
 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int *pPosSubV;			//Vector with position each task starts in Vectors.
 int *pSizeSubV;			//Vector with size of subvector.
 int Rem; 				//Remainder
 int AddPos;			//Adding positions.
 int Root=0;			//Root process.

 //MPI Initialization
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
 
 if (MyRank==Root)	//Task Root--
    {
     #if (PRINT==1)
     printf("Task %d is the Root.\n",MyRank);
     #endif

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

	 //Calloc de Getmen put data to zeroes
	 //pVector1 and 2 are not needed in task0
	 //They are generated in pSubvector1 and pSubvector 2 by all tasks
	 pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector3");
	 
	}
	
 //All tasks 
 MPI_Bcast(&Rows,1,MPI_INT,Root,MPI_COMM_WORLD);
 //Get men and initialize DisplacV and pSizeSubV
 pPosSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:DisplacV");
 pSizeSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:pSizeSubV");
 
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
 
 //Init seed of random number generator to the task rank
 //each sequence is local to each task becasue tasks do not share variables.
 srand48(MyRank);
 
 //All tasks including root init subvectors
 pSubVector1  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector1");
 pSubVector2  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector2");
 
  //All tasks init values of SubVector1 and 2
 InitVectorInt (pSubVector1,pSizeSubV[MyRank]);
 InitVectorInt (pSubVector2,pSizeSubV[MyRank]);
 
 #if (PRINT==1)
 printf("Task %d scater vectors:\n", MyRank);
 PrintVectorInt(pSubVector1,pSizeSubV[MyRank],"SubVector1:");
 PrintVectorInt(pSubVector2,pSizeSubV[MyRank],"SubVector2:");
 #endif
 
 
 //Only workers needs psubvector3
 if (MyRank!=Root)
    {
     pSubVector3  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubVector3");
     for (int i=0;i<pSizeSubV[MyRank];i++)
         pSubVector3[i]=pSubVector1[i]+pSubVector2[i];
         
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pSubVector3,pSizeSubV[MyRank],"SubVector3:");
     #endif
     
     MPI_Gatherv(pSubVector3, pSizeSubV[MyRank], MPI_INT,
                 pVector3, pSizeSubV, pPosSubV, 
                 MPI_INT,Root, MPI_COMM_WORLD);
    }

 //Take care: pSubVector1 and 2 starts in 0.
 //Root task do not need pSubvector3, it use pVector3 IN_PLACE 
 if (MyRank==Root)
    {
     for (int i=pPosSubV[Root];i<pPosSubV[Root]+pSizeSubV[Root];i++)
         pVector3[i]=pSubVector1[i-pPosSubV[Root]]+pSubVector2[i-pPosSubV[Root]];
         
     MPI_Gatherv(MPI_IN_PLACE,0, MPI_INT,
                 pVector3, pSizeSubV, pPosSubV, 
                 MPI_INT,Root, MPI_COMM_WORLD);
    }


 //All deallocate subvectors
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubVector3);//For task 0 is NULL
 free((void *)pSubVector2);
 free((void *)pSubVector1);

 if (MyRank==Root)
    {
     #if (PRINT==1)
     PrintVectorInt(pVector3,Rows,"Vector3");
     #endif
     free((void *)pVector3);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
