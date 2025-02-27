/******************************************************************************
			utils.h  - description
			----------------------
	begin		: Sep 2021
	copywirght	: (C) 2021 by L.G.Casado.

********************************************************************	
Purpose: implement routines that will be used by other modules. 
	OpenFile: Abrir un fichero con chequeo de error
	CloseFile: Cerrar un fichero
	PrintVC: Imprimir un Vector de bytes.
	PrintMC: Imprimir una matriz de bytes.
	PrintVI: Imprimir un Vector de int.
	PrintMI: Imprimir una matriz de int.

*******************************************************************************/
#ifndef __UTILS__
#define __UTILS__

#define False 0
#define True 1

/*---------------------------------------------------------------------------*/
FILE 	*OpenFile	(char *name, char const *mode);
void 	CloseFile	(FILE *f);
void 	PrintVC 	(FILE * FOut, char  * pVC, int NDim);
void 	PrintMC 	(FILE * FOut, char ** pMC, int Fil, int Col);
void 	PrintVI 	(FILE * FOut, int   * pVI, int NDim);
void 	PrintMI 	(FILE * FOut, int  ** pMI, int Fil, int Col);
void    SwapInt     (int * a, int * b);

#endif /*__UTILS__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
