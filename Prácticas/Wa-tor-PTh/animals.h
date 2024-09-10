/******************************************************************************
			animals.h  - description
			----------------------
	begin		: Sep 2021
	copywirght	: (C) 2021 by L.G.Casado.

********************************************************************	
Purpose: implement fish and shark ruotines 
	
	NewAnimal: get memory for a new animal (fish or Shark)
	PrintAnimal: print animal data.

*******************************************************************************/
#ifndef __ANIMALS__
#define __ANIMALS__

#define FISH  0
#define SHARK 1


typedef struct StAnimal
{
 unsigned char Animal; 	//Fish or Shark
 int SimIter;			//Current simulation iteration.
 int NIterLive; 		//Number of iterations an animal is living
 int Energy;			//Current energy of a Shark
} DataAnimal;


/*---------------------------------------------------------------------------*/
DataAnimal * NewAnimal(unsigned char typeAnimal, const int SimIter,
                       const int SiEnergy);
void         PrintAnimal (DataAnimal * pDataAnimal);

#endif /*__ANIMALS__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
