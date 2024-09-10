#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getmem.h"
#include "utils.h"
#include "animals.h"
#include "ocean.h"
#ifdef _OPENMP
#  include <omp.h>
#endif

#define PRINT 1

/*---------------------------------------------------------------------------*/
void InitOcean(DataAnimal *** Ocean,
               const int Rows,    const int Cols, 
               const int NInitFishes, const int NInitSharks,
               const int NiFBreed, const int NiSBreed,
               const int SiEnergy)
{
 int Row,Col;

 for (int i=0;i<NInitFishes;i++)
     {
      do
       {
        Row=lrand48() % Rows;
        Col=lrand48() % Cols;
       }
      while (Ocean[Row][Col]!=NULL);

      //Create a Fish
      Ocean[Row][Col]=NewAnimal(FISH,0,SiEnergy);
     //Init NIterLive ramdomly.
      Ocean[Row][Col]->NIterLive=lrand48() % NiFBreed;
     }

 for (int i=0;i<NInitSharks;i++)
     {
      do
       {
        Row=lrand48() % Rows;
        Col=lrand48() % Cols;
       }
      while (Ocean[Row][Col]!=NULL);
      
      //Create a Shark
      Ocean[Row][Col]=NewAnimal(SHARK,0,SiEnergy);
      //Init NIterLive ramdomly.
      Ocean[Row][Col]->NIterLive=lrand48() % NiSBreed;
     } 
}

/*---------------------------------------------------------------------------*/
void OceanToRGBFile(DataAnimal *** Ocean, char * FileName,
                    const int Rows, const int Cols)
{
 FILE *  FOut;
 unsigned char White[]={255, 255, 255};
 unsigned char Red[]  ={255,   0,   0};
 unsigned char Blue[] ={  0,   0, 255};

 FOut=OpenFile(FileName,"wb");
 for (int i=0; i<Rows; i++)
     for (int j=0; j<Cols; j++)
         {
          if (Ocean[i][j]==NULL)
              fwrite(White,sizeof(char),3,FOut);
          else if (Ocean[i][j]->Animal==FISH)
                   fwrite(Blue, sizeof(char),3,FOut);
               else if (Ocean[i][j]->Animal==SHARK)
                        fwrite(Red,  sizeof(char),3,FOut);
                    else
                       {
                        printf("OceanToRGBFile: Unknown ocean[%d][%d] state.\n",
                                i,j);
                        exit(1);
                       }
         }
 fflush(FOut);
 CloseFile(FOut);
}


/*---------------------------------------------------------------------------*/
void FreeNeighbours(DataAnimal ***Ocean, 
                   const int Rows, const int Cols,
                   const int i, const int j,
                   int * FreeX, int * FreeY, int * NFree)
{
 int Newi, Newj;

 (*NFree)=0;

 //North
 if (i==0)
     Newi=Rows-1;
 else
     Newi=i-1;

 if (Ocean[Newi][j]==NULL)
    {
     FreeX[*NFree]=Newi;
     FreeY[*NFree]=j;
     (*NFree)++;
    }

//South
 if (i==Rows-1)
     Newi=0;
 else
     Newi=i+1;

 if (Ocean[Newi][j]==NULL)
    {
     FreeX[*NFree]=Newi;
     FreeY[*NFree]=j;
     (*NFree)++;
    }

 //East
 if (j==Cols-1)
     Newj=0;
 else
     Newj=j+1;

 if (Ocean[i][Newj]==NULL)
    {
     FreeX[*NFree]=i;
     FreeY[*NFree]=Newj;
     (*NFree)++;
    }

 //West
 if (j==0)
     Newj=Cols-1;
 else
     Newj=j-1;

 if (Ocean[i][Newj]==NULL)
    {
     FreeX[*NFree]=i;
     FreeY[*NFree]=Newj;
     (*NFree)++;
    }
}


/*---------------------------------------------------------------------------*/
//TODO Add pRandData parameters at the end
void IterateFish(DataAnimal ***Ocean, 
                 const int Rows, const int Cols,
                 const int i, const int j,
                 const int SimIter, int * pNFishes, const int NiFBreed, struct drand48_data *pRandData)
{
 int Newi, Newj; 
 int FreeX[4];	//x values of free neighbourg cells.
 int FreeY[4];	//y values of free neighbourg cells.
 int NFree=0;
 int FreeXYIndex;

 if (Ocean[i][j]->SimIter > SimIter) //Already visited
    return;

 //Fish is visited now and lives one more iteration
 Ocean[i][j]->SimIter++;
 Ocean[i][j]->NIterLive++;

 FreeNeighbours(Ocean,Rows,Cols,i,j,FreeX,FreeY,&NFree);
 
 if (NFree==0)
    return;
 
 if (NFree>1)
    {
	 //NFree=1 -> I know FreeXYIndex==0
	 
	 //TODO #ifdef _OPENMP 
     #ifdef _OPENMP
	 //TODO see your thread mum and use the correspondig pRandData
	 //to get a random number with lrand48_r() instead of lrand48()
     int IDthread = omp_get_thread_num();
     long rand = 0;
     lrand48_r(&pRandData[IDthread], &rand);
     FreeXYIndex = rand % NFree;
     #endif
	 //#ifndef _OPENMP 
	 
	 //TODO #ifndef _OPENMP use lrand48()
     #ifndef _OPENMP
	 FreeXYIndex=lrand48() % NFree;
     #endif
	}
 else
    FreeXYIndex=0;
    
 Newi=FreeX[FreeXYIndex];
 Newj=FreeY[FreeXYIndex];

 if (Ocean[i][j]->NIterLive >= NiFBreed) //free near cell and can breed
    {
     //New fish is created
     Ocean[Newi][Newj] = NewAnimal(FISH, SimIter+1,0);
     (*pNFishes)++;

     //Update parent fish to also born.
     Ocean[i][j]->NIterLive=0;
     return;
    }

 //Move fish to a free cell
 Ocean[Newi][Newj] = Ocean[i][j];
 Ocean[i][j]=NULL;
}


/*---------------------------------------------------------------------------*/
void FishNeighbours(DataAnimal ***Ocean, 
                    const int Rows, const int Cols,
                    const int i, const int j,
                    int * FishX, int * FishY, int * NInitFishes)
{
 int Newi, Newj;

 (*NInitFishes)=0;

 //North
 if (i==0)
     Newi=Rows-1;
 else
     Newi=i-1;

 if (Ocean[Newi][j]!=NULL && Ocean[Newi][j]->Animal==FISH)
    {
     FishX[*NInitFishes]=Newi;
     FishY[*NInitFishes]=j;
     (*NInitFishes)++;
    }

//South
 if (i==Rows-1)
     Newi=0;
 else
     Newi=i+1;

 if (Ocean[Newi][j]!=NULL && Ocean[Newi][j]->Animal==FISH)
    {
     FishX[*NInitFishes]=Newi;
     FishY[*NInitFishes]=j;
     (*NInitFishes)++;
    }

 //East
 if (j==Cols-1)
     Newj=0;
 else
     Newj=j+1;

 if (Ocean[i][Newj]!=NULL && Ocean[i][Newj]->Animal==FISH)
    {
     FishX[*NInitFishes]=i;
     FishY[*NInitFishes]=Newj;
     (*NInitFishes)++;
    }

 //West
 if (j==0)
     Newj=Cols-1;
 else
     Newj=j-1;

 if (Ocean[i][Newj]!=NULL && Ocean[i][Newj]->Animal==FISH)
    {
     FishX[*NInitFishes]=i;
     FishY[*NInitFishes]=Newj;
     (*NInitFishes)++;
    }
}

/*---------------------------------------------------------------------------*/
//TODO Add pRandData  parameter at the end
void IterateShark(DataAnimal ***Ocean, 
                 const int Rows, const int Cols,
                 int i, int j,
                 const int SimIter, int * pNFishes, int * pNSharks, 
                 const int NiSBreed,
                 const int SiEnergy, const int SeFEnergy, struct drand48_data *pRandData)
{
 int Newi, Newj;//New cell and temp to swap
 unsigned char Eat   = False;// a shark ate.
 int FishX[4];	//x values of fish in neighbourg cells.
 int FishY[4];	//y values of fish in neighbourg cells.
 int NFoundFishes=0; //Number of neighbour fishes.
 int FishXYIndex;//Index of the fish to move and eat, in [FishX][FishY]
 int FreeX[4];	//x values of free neighbourg cells.
 int FreeY[4];	//y values of free neighbourg cells.
 int NFree=0;	//Number of neighboug free cells.
 int FreeXYIndex;//Index of the cell to move, in [FishX][FishY]

 #if (PRINT==2)
 printf("Shark %d, %d before moving. NiSBreed=%d -----------\n", i,j,NiSBreed);
 PrintAnimal(Ocean[i][j]);
 PrintOcean(Ocean,Rows,Cols);
 #endif

 if (Ocean[i][j]->SimIter > SimIter) //Already visited
    return;

 //Shark is visited now and reduces in one the Energy
  Ocean[i][j]->SimIter++;
  Ocean[i][j]->NIterLive++;
  Ocean[i][j]->Energy--;

 //Shark could die
 if (Ocean[i][j]->Energy==0)
    {
     //puts("shark dies");
     free((void *)Ocean[i][j]);
     Ocean[i][j]=NULL;
     (*pNSharks)--;
     return;
    }

 FishNeighbours(Ocean,Rows,Cols,i,j,FishX,FishY,&NFoundFishes);
 #if (PRINT==2)
 puts("FishesXY:");
 PrintVI(stdout,FishX,NFoundFishes);
 PrintVI(stdout,FishY,NFoundFishes);
 #endif

 //Shark can eat and breed at the same time
 //It could eat a fish
 if (NFoundFishes>0)
    {
     if (NFoundFishes>1)
        {
		 //NFoundFishes=1 -> I know FishXYIndex==0
		
		 //TODO #ifdef _OPENMP 
         #ifdef _OPENMP
		 //TODO see your thread mum and use the correspondig pRandData 	
		 // to get a random number with lrand48_r() instead of lrand48()
         int IDthread = omp_get_thread_num();
		 long rand = 0;
         lrand48_r(&pRandData[IDthread], &rand);
         FishXYIndex = rand % NFoundFishes;
         #endif
		 //TODO next is done only if #ifndef _OPENMP 
         #ifndef _OPENMP
		 FishXYIndex=lrand48() % NFoundFishes;
         #endif
		}
	 else
	    FishXYIndex=0;
     
     Newi=FishX[FishXYIndex];
     Newj=FishY[FishXYIndex];
     #if (PRINT==2)
     printf("Eat fish %d %d.\n", Newi, Newj);
     #endif

     //Fish died
     free((void *)Ocean[Newi][Newj]);
     Ocean[Newi][Newj]=NULL;
     (*pNFishes)--;

     //Shark gets energy.
     (Ocean[i][j]->Energy)+=SeFEnergy;

     //Shark moves to the new cell
     Ocean[Newi][Newj]=Ocean[i][j];
     Ocean[i][j]=NULL;
     #if (PRINT==2)
     printf("Shark %d, %d after eating -----------\n", Newi,Newj);
     PrintAnimal(Ocean[Newi][Newj]);
     PrintOcean(Ocean,Rows,Cols);
     #endif
     Eat=True;
     SwapInt(&i,&Newi);
     SwapInt(&j,&Newj);
     #if (PRINT==2)
     printf("Eat=%d\n", Eat);
     #endif
    }

 if (Eat==False) //look for a free cell
    {
     //Try to move to a free cell
     #if (PRINT==2)
     printf("Eat=False\n");
     #endif
     FreeNeighbours(Ocean,Rows,Cols,i,j,FreeX,FreeY,&NFree);
 
     if (NFree==0)
        return;
        
	 if (NFree>1)
	    {
		  //NFree=1 -> I know FreeXYIndex==0
		  
		 //TODO #ifdef _OPENMP    
         #ifdef _OPENMP
		 //TODO see your thread mum and use the correspondig pRandData 
		 // to get a random number with lrand48_r() instead of lrand48()
		     int IDthread = omp_get_thread_num();
         long rand = 0;
         lrand48_r(&pRandData[IDthread], &rand);
         FreeXYIndex = rand % NFree;
         #endif
		 //TODO next is done only if #ifndef _OPENMP 
         #ifndef _OPENMP
		    FreeXYIndex=lrand48() % NFree;
         #endif
		}
	 else
	    FreeXYIndex=0;
     
     Newi=FreeX[FreeXYIndex];
     Newj=FreeY[FreeXYIndex];
    }

 //Shark can breed. If eat, the new shark will be in old fish cell 
 //Newi and Newj are for a new cell, after eat a fish or looking for.
 if (Ocean[i][j]->NIterLive >= NiSBreed) 
    {
     //New Shark is created
     Ocean[Newi][Newj] = NewAnimal(SHARK,SimIter+1,SiEnergy);
     (*pNSharks)++;

     //Update parent Shark to also born, but maintain Energy.
     Ocean[i][j]->NIterLive=0;
     #if (PRINT==2)
     printf("Shark bred\n");
     PrintOcean(Ocean,Rows,Cols);
     #endif
     return;
    }

 //Shark did not eat niehter breed. Move shark to a new free cell
 if (Eat==False) //It moves when eat.
    {
     #if (PRINT==2)
     printf("Shark moves without eat neither breed.\n");
     #endif
     Ocean[Newi][Newj] = Ocean[i][j];
     Ocean[i][j]=NULL;
    }
}

/*---------------------------------------------------------------------------*/
//TODO Add pRandData parameter at the end
void IterateOcean(DataAnimal ***Ocean,
                  const int Rows, const int Cols,
                  const int SimIter, int *pNFishes, int *pNSharks,
                  const int NiFBreed, const int NiSBreed,
                  const int SiEnergy, const int SeFEnergy, struct drand48_data *pRandData)
{
    int local_pNFishes = 0;
    int local_pNSharks = 0;

  // TODO: Parallelize with OpenMP using a reduction(+:...)
  #ifdef _OPENMP
    #pragma omp parallel shared(Ocean, Rows, Cols, SimIter, NiFBreed, NiSBreed, SiEnergy, SeFEnergy, pRandData) reduction(+ : local_pNFishes, local_pNSharks)
  #endif 
  {
    #ifdef _OPENMP
      #pragma omp for schedule(dynamic)
    #endif
    for (int i=0; i<Rows; i+=3) {
      for (int j=0; j<Cols; j++) {
        if (Ocean[i][j] != NULL && Ocean[i][j]->Animal == FISH)
          // TODO Add pRandData parameters at the end
          IterateFish(Ocean, Rows, Cols, i, j, SimIter,
            &local_pNFishes, NiFBreed, pRandData);
            
        if (Ocean[i][j] != NULL && Ocean[i][j]->Animal == SHARK)
          // TODO Add pRandData  parameters at the end
          IterateShark(Ocean, Rows, Cols, i, j, SimIter,
            &local_pNFishes, &local_pNSharks, NiSBreed,
            SiEnergy, SeFEnergy, pRandData);
      }
    }

    #ifdef _OPENMP
      #pragma omp for schedule(dynamic)
    #endif
    for (int i=1; i<Rows; i+=3) {
      for (int j=0; j<Cols; j++) {
        if (Ocean[i][j] != NULL && Ocean[i][j]->Animal == FISH)
          // TODO Add pRandData parameters at the end
          IterateFish(Ocean, Rows, Cols, i, j, SimIter,
            &local_pNFishes, NiFBreed, pRandData);
            
        if (Ocean[i][j] != NULL && Ocean[i][j]->Animal == SHARK)
          // TODO Add pRandData  parameters at the end
          IterateShark(Ocean, Rows, Cols, i, j, SimIter,
            &local_pNFishes, &local_pNSharks, NiSBreed,
            SiEnergy, SeFEnergy, pRandData);
      }
    }

    #ifdef _OPENMP
      #pragma omp for schedule(dynamic)
    #endif
    for (int i=2; i<Rows; i+=3) {
      for (int j=0; j<Cols; j++) {
        if (Ocean[i][j] != NULL && Ocean[i][j]->Animal == FISH)
          // TODO Add pRandData parameters at the end
          IterateFish(Ocean, Rows, Cols, i, j, SimIter,
            &local_pNFishes, NiFBreed, pRandData);
            
        if (Ocean[i][j] != NULL && Ocean[i][j]->Animal == SHARK)
          // TODO Add pRandData  parameters at the end
          IterateShark(Ocean, Rows, Cols, i, j, SimIter,
            &local_pNFishes, &local_pNSharks, NiSBreed,
            SiEnergy, SeFEnergy, pRandData);
      }
    }
  }
    
  // Asignar los valores locales actualizados a las variables
  // originales
  *pNFishes += local_pNFishes;
  *pNSharks += local_pNSharks;
}

/*---------------------------------------------------------------------------*/
void PrintOcean(DataAnimal *** Ocean, const int Rows, const int Cols)
{
 for (int i=0;i<Rows;i++)
     {
      for (int j=0;j<Cols;j++)
          if (Ocean[i][j]==NULL)
             printf("0");
          else if (Ocean[i][j]->Animal==FISH)
                  printf("F");
               else if (Ocean[i][j]->Animal==SHARK)
                        printf("S");
      printf("\n");
     }
}

/*---------------------------------------------------------------------------*/
void FreeOcean(DataAnimal *** Ocean, const int Rows, const int Cols)
{
 for (int i=0;i<Rows;i++)
     for (int j=0;j<Cols;j++)
         if (Ocean[i][j]==NULL)
             continue;
         else 
            free ((void*) Ocean[i][j]);

 Free2D((void **) Ocean, Rows);
}
