#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/*---------------------------------------------------------------------------*/
FILE * OpenFile (char *name, char const *mode)
{
 FILE *f;

 f=fopen(name,mode);
 if (f==NULL)
    {
     fprintf(stderr,"Can not open the file %s\n",name);
     exit(1);
    }
 return f;
}

/*---------------------------------------------------------------------------*/
void CloseFile (FILE *f)
{
  if (fclose(f))
      fprintf(stderr,"Unable to close file.\n");
}

/*---------------------------------------------------------------------------*/
void PrintVChar (FILE * FOut, char * pVI, int Cols)
{
 int i;

 for (i=0;i<Cols;i++)
     fprintf(FOut,"%d, ",(int)pVI[i]);
 fprintf(FOut,"\n");    
}

/*---------------------------------------------------------------------------*/
void PrintMChar (FILE * FOut, char ** pMI, int Rows, int Cols)
{
 int i;

 for (i=0;i<Rows;i++)
     PrintVChar (FOut,pMI[i],Cols);
 fprintf(FOut,"\n");
}


/*---------------------------------------------------------------------------*/
void PrintVInt (FILE * FOut, int * pVI, int Cols)
{
 int i;

 for (i=0;i<Cols;i++)
     fprintf(FOut,"%d, ",(int)pVI[i]);
 fprintf(FOut,"\n");    
}

/*---------------------------------------------------------------------------*/
void PrintMInt (FILE * FOut, int ** pMI, int Rows, int Cols)
{
 int i;

 for (i=0;i<Rows;i++)
     PrintVInt (FOut,pMI[i],Cols);
 fprintf(FOut,"\n");
}

/*---------------------------------------------------------------------------*/
void PrintVDouble (FILE * FOut, double * pVD, int Cols)
{
 int i;

 for (i=0;i<Cols;i++)
     fprintf(FOut,"%g, ",pVD[i]);
 fprintf(FOut,"\n");    
}

/*---------------------------------------------------------------------------*/
void PrintMDpuble (FILE * FOut, double ** pMD, int Rows, int Cols)
{
 int i;

 for (i=0;i<Rows;i++)
     PrintVDouble (FOut,pMD[i],Cols);
 fprintf(FOut,"\n");
}
/*---------------------------------------------------------------------------*/
void SwapInt (int * a, int * b)
{
 int tmp;

 tmp=*a;
 *a=*b;
 *b=tmp;
} 
