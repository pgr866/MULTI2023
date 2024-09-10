/******************************************************************************
			getmem.h  - description
			----------------------
	begin		: Jan 2002
	copywirght	: (C) 2002 by L.G.C.
	
     ********************************************************************	
     Purpose: Rutines to get and free memory.
     
     Routines:
     	GetMem():   Get memory for a one dimensional vector. "Where" is a string
		which usualy contains the caller routine name to know where was
		the error.
			Use free((void *) p) for p=(type) GetMem (....)

		GetMem2D(): Get memory for a 2D array using GetMem.
					Allocated memory is not continous.

		Free2D(): Free memory allocated with GetMem2D.

		To Get 2D continous allocation we can use GetMem with N*M elements: 
			for (i=0; i < N; i++) {
  				for (j=0; j < M; j++) {
	   				2d_array[i*M +j]=...

		GetMem3D(): continous memory allocation of a 3D array.
					Allocated memory is not continous.
		Free3D(): Free memory allocated with GetMem3D.


*******************************************************************************/
#ifndef __GETMEM__
#define __GETMEM__


/*----------------------------------------------------------------------------*/
void *   GetMem       (size_t Num, size_t Tam, char const * Where);
void **  GetMem2D     (int rows, int columns, int sizeofTipo, 
                       char const * Where);
void     Free2D       (void **  h, int rows);
void *** GetMem3D     (int nelem, int rows, int columns, int sizeofTipo, 
                       char const * Where);
void     Free3D       (void ***  h, int nelem, int rows);


#endif /*__GETMEM__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
