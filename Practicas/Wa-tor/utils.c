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
void PrintVC (FILE * FOut, char * pVC, int NDim)
{
 int i;

 for (i=0;i<NDim;i++)
     fprintf(FOut,"%d, ",(int)pVC[i]);
 fprintf(FOut,"\n");    
}

/*---------------------------------------------------------------------------*/
void PrintMC (FILE * FOut, char ** pMC, int Fil, int Col)
{
 int i;

 for (i=0;i<Fil;i++)
     PrintVC (FOut,pMC[i],Fil);
 fprintf(FOut,"\n");    
}

/*---------------------------------------------------------------------------*/
void PrintVI (FILE * FOut, int * pVI, int NDim)
{
 int i;

 for (i=0;i<NDim;i++)
     fprintf(FOut,"%d, ",pVI[i]);
 fprintf(FOut,"\n");    
}

/*---------------------------------------------------------------------------*/
void PrintMI (FILE * FOut, int ** pMI, int Fil, int Col)
{
 int i;

 for (i=0;i<Fil;i++)
     PrintVI (FOut,pMI[i],Fil);
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

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
