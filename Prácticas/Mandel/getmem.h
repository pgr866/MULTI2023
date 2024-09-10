/******************************************************************************
			getmem.h  - description
			----------------------
	begin		: Jan 2002
	copywirght	: (C) 2002 by L.G.C.
	
     ********************************************************************	
     Purpose: Rutines to get and free memory.
     
     Routines:
     	GetMem():   Get memory for a one dimensional vector. Where is a string
		which usualy contains the caller routine name to know where was
		the error.

	GetMem2D(): Get memory for a two dimensional vector using GetMem.
	Free2D(): Free memory allocated with GetMem2D.
	free((void *) p) for p=(type) GetMem (....)

*******************************************************************************/
#ifndef __GETMEM__
#define __GETMEM__


/*----------------------------------------------------------------------------*/
void *   GetMem   (size_t Num, size_t Tam, char const * Where);
void **  GetMem2D (int rows, int columns, int sizeofTipo, char const * Where);
void *** GetMem3D (int nelem, int rows, int columns, int sizeofTipo, 
                   char const * Where);
void    Free3D    (void ***  h, int nelem, int rows);
void    Free2D    (void **  h, int rows);

#endif /*__GETMEM__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
