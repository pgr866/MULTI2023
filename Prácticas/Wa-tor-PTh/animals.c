#include <stdio.h>
#include <stdlib.h>
#include "getmem.h"
#include "animals.h"


/*---------------------------------------------------------------------------*/
DataAnimal * NewAnimal(unsigned char typeAnimal, const int SimIter,
                       const int SiEnergy)
{
 DataAnimal * pDataAnimal;
 
 pDataAnimal = (DataAnimal *) GetMem ((size_t)1, sizeof(DataAnimal),
									  "NewAnimal:Getmem");

 pDataAnimal->SimIter   = SimIter;
 pDataAnimal->NIterLive = 0;
 if (typeAnimal==FISH)
     pDataAnimal->Animal=FISH;
 else //SHARK
    {
     pDataAnimal->Animal=SHARK;
     pDataAnimal->Energy=SiEnergy;
    }

 return pDataAnimal;
}

/*---------------------------------------------------------------------------*/
void PrintAnimal(DataAnimal * pDataAnimal)
{
 if (pDataAnimal==NULL)
    {
     puts("PrintAnimal: pAnimal==NULL");
     return;
    }

 if (pDataAnimal->Animal==FISH)
    {
     printf("Fish in iter %d living %d iter.\n", 
            pDataAnimal->SimIter, pDataAnimal->NIterLive);
     return;
    }

 if (pDataAnimal->Animal==SHARK)
    {
     printf("Shark in iter %d living %d has Energy=%d.\n", 
             pDataAnimal->SimIter, pDataAnimal->NIterLive, pDataAnimal->Energy);
     return;
    }

 puts("PrintAnimal error.\n");
 exit(1);
}



