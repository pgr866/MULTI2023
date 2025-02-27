/******************************************************************************
			utils.h  - description
			----------------------
	begin		: Sep 2021
	copywirght	: (C) 2021 by L.G.Casado.

********************************************************************	
Purpose: implement routines that will be used by other modules. 
	OpenFile: Abrir un fichero con chequeo de error
	CloseFile: Cerrar un fichero
	PrintVI: Imprimir un vector de enteros
	PrintMI: Imprimir una matriz de enteros.

*******************************************************************************/
#ifndef __UTILS__
#define __UTILS__

#define False 0
#define True 1

typedef struct drand48_data STDrand48Data;

/*---------------------------------------------------------------------------*/
FILE 	*OpenFile		(char *name, char const *mode);
void 	CloseFile		(FILE *f);
void 	PrintVChar 		(FILE * FOut, char * pVI, 	int Cols);
void 	PrintMChar 		(FILE * FOut, char ** pMI, 	int Rows, int Cols);
void 	PrintVInt 		(FILE * FOut, int * pVI,	int Cols);
void 	PrintMInt 		(FILE * FOut, int ** pMI,	int Rows, int Cols);
void 	PrintVDouble 	(FILE * FOut, double * pVD,	int Cols);
void 	PrintMDpuble 	(FILE * FOut, double ** pMD,int Rows, int Cols);
void 	SwapInt 		(int * a, int * b);
#endif /*__UTILS__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
