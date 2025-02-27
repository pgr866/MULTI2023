/******************************************************************************
			ocean.h  - description
			----------------------
	begin		: Sep 2021
	copywirght	: (C) 2021 by L.G.Casado.

********************************************************************	
Purpose: implement ocean ruotines 
	
	

*******************************************************************************/
#ifndef __OCEAN__
#define __OCEAN__

/*---------------------------------------------------------------------------*/
void InitOcean      (DataAnimal *** Ocean,
                     const int Rows,    const int Cols, 
                     const int NInitSFishes, const int NInitSharks,
                     const int NiFBreed, const int NiSBreed,
                     const int Sienergy);
void OceanToRGBFile (DataAnimal *** Ocean, char * FileName,
                     const int Rows, const int Cols);

//TODO; Add pRandData parameter
void IterateOcean   (DataAnimal *** Ocean,
                     const int Rows, const int Cols,
                     const int SimIter, int * pNFishes, int * pNSharks,
                     const int NiFBreed, const int NiSBreed,
                     const int SiEnergy, const int SeFEnergy, struct drand48_data *pRandData);
void PrintOcean		(DataAnimal *** Ocean, const int Rows, const int Cols);
void FreeOcean      (DataAnimal *** Ocean, const int Rows, const int Cols);
#endif /*__OCEAN__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
