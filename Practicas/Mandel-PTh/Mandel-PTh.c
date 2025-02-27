#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include "stdbool.h"


#define PRINT 0
#define DEGUG 0

/*---------------------------------------------------------------------------*/
typedef struct th_mandel_args
{
 char ** 	ppRed;
 char ** 	ppGreen;
 char ** 	ppBlue;
 double 	MinX;
 double		MinY;
 double 	IncX;
 double		IncY;
 int		MaxNIter;
 int *		pRowToProcess;	//Shared variable to know which row process next.
 int *		pChunckSize;	//How many to process.
 int		Rows;
 int		Cols;
 pthread_mutex_t * pth_RowToProcess_mutex;
 int 		ThreadId;
 #if (DEGUG==1)
 int 		NRowsDone; 		//How many rows were done by this thread.
 #endif
} Type_Th_Mandel_Args;

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]		");
 puts("\t  -r  <n rows image>			");
 puts("\t  -c  <n columns image>		");
 puts("\t  -mx <min  x Mandel's window>	");
 puts("\t  -my <min  y Mandel's window>	");
 puts("\t  -sx <size x Mandel's window>	");
 puts("\t  -sy <size y Mandel's window>	");
 puts("\t  -mi <max n of iterations in pixel>   ");
 puts("\t  -nt <Number of Threads>");
 puts("\t  -cs <Chunk size> [1]"); 
 puts("\t  -of <file> [Image]>	");
 puts("\t  -o graphical output	");

 exit(0);
}

/*---------------------------------------------------------------------------*/
void NIterToRGB(int NIter, char * pRed, char *pGreen, char *pBlue)
//Move the number of iterations to RGB using a pallete.
{
 const char PalleteRed  [16] =
            {66,25, 9, 4,  0, 12, 24, 57,134,211,241,248,255,204,153,106};
 const char PalleteGreen[16] =
            {30, 7, 1, 4,  7, 44, 82,125,181,236,233,201,170,128, 87, 52};
 const char PalleteBlue [16] =
            {15,26,47,73,100,138,177,209,229,248,191,95,   0,  0,  0,  3};

 NIter=NIter%16;
 *pRed   = PalleteRed  [NIter];
 *pGreen = PalleteGreen[NIter];
 *pBlue  = PalleteBlue [NIter];
}

/*---------------------------------------------------------------------------*/
 void PixelMandel(char **ppRed, char ** ppGreen, char ** ppBlue,
                  const double MinX, const double MinY, 
                  const double IncX, const double IncY,
                  const int MaxNIter, const int i, const int j)
{
 int 	 NIter; 			//Number of iterations.
 double zr,zi,cr,ci;
 double zrs,zis;

 zi=ci=MinY+i*IncY;
 zr=cr=MinX+j*IncX;
 zrs=zis=0.0;
 NIter=0;
 while (zrs+zis<=4.0 && NIter<=MaxNIter)
       {
        zrs=zr*zr;
        zis=zi*zi;
        zi=2.0*zr*zi+ci;
        zr=zrs-zis+cr;
        NIter++;
       }
       
 NIterToRGB(NIter,&(ppRed[i][j]),&(ppGreen[i][j]),&(ppBlue[i][j]));
 
}

/*---------------------------------------------------------------------------*/
void * Th_Mandel (void * pTh_Args)
{
 Type_Th_Mandel_Args * pMyData;
 pMyData=(Type_Th_Mandel_Args *) pTh_Args;
 int Rows = pMyData->Rows;
 int Cols = pMyData->Cols;
 int *pRowToProcess=pMyData->pRowToProcess;
 //Following is updated in mutual exclusion.
 int MyRow=0; 
 int MyChunckSize=1; //Rows may be not multiple of NThreads.
 
 while (*(pMyData->pRowToProcess)< Rows)
       {
        //TODO: lock mutex to get MyRow -> Row to process
        pthread_mutex_lock(pMyData->pth_RowToProcess_mutex);

        if (*(pMyData->pRowToProcess) < pMyData->Rows)
           {
            //TODO: Set MyRow
            MyRow = *pRowToProcess;
            //TODO: Set MyChunkSize without reach Rows 
            MyChunckSize = *pMyData->pChunckSize;
            if (*pRowToProcess + MyChunckSize > Rows)
            {
               MyChunckSize = Rows - *pRowToProcess;
            }
            //TODO: Increase (*pRowToProcess)
            *pRowToProcess += MyChunckSize;
           }
        //TODO: unlock  mutex
        pthread_mutex_unlock(pMyData->pth_RowToProcess_mutex);

        #if (PRINT==1)  
        printf("Thread %d, doing rows %d-%d.\n", 
                pMyData->ThreadId,MyRow, MyRow+MyChunckSize-1);
        fflush (stdout);
        #endif
        for (int i=MyRow;i<MyRow+MyChunckSize;i++)
            for (int j=0; j < Cols; j++)
                {
                 PixelMandel(pMyData->ppRed, 
                             pMyData->ppGreen, 
                             pMyData->ppBlue,
                             pMyData->MinX, pMyData->MinY, 
                             pMyData->IncX, pMyData->IncY,
                             pMyData->MaxNIter, i, j);
            }
        #if (DEGUG==1)
        pMyData->NRowsDone+=MyChunckSize;
        #endif
       }
 if (pMyData->ThreadId!=0) // I am not the Main Thread. 
    pthread_exit((void *)&(pMyData->ThreadId));

 return (NULL);
}


/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int		MaxNIter;			//Max number of Iterations.
 int		Rows, Cols;			//Rows and Cols of the Image.
 double		MinX, MinY;			//Min x and y of the mandelbrot window.
 double		SizeX, SizeY; 		//Size in x and y of the Mandelbrot window.
   //RGB matrices (0..255)
 char **	ppRed=NULL, ** ppGreen=NULL, ** ppBlue=NULL;  
 char *		FileName;
 FILE *		FOut;
 char		Command[256];
 char		GenImage=False; 	//If graphical output is generated 
 double		IncX, IncY; 		//Pixes width in x and y.
 int 		NThreads; 			//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_Mandel_Args *pTh_Args=NULL;
 int 		rc;					//Return value  of pthread_join 
 void *		status;				//status returned in pthread_join.
 int 		RowToProcess=0;		//Next row to process.
 int 		ChunckSize =1;	    //How Many to process.
 pthread_mutex_t 	pth_RowToProcess_mutex;


 if (ExistArg("-h",argc,argv))
    ParametersError();  

 if (!ExistArg("-r",argc,argv))
    {
    fputs("Parameter -r is necessary.\n",stderr);
    ParametersError();
    }
 else
    {
     Rows = atoi(GetArg("-r",argc,argv));
     if (Rows <=3)
        {
         puts("Rows<=3");
         exit(1);
        }
    } 

 if (!ExistArg("-c",argc,argv))
    {
    fputs("Parameter -c is necessary.\n",stderr);
    ParametersError();
    }
 else
    {
     Cols = atoi(GetArg("-c",argc,argv)); 
     if (Cols <=3)
        {
         puts("Col<=3");
         exit(1);
        }
    }  

 if (!ExistArg("-mx",argc,argv))
    {
    fputs("Parameter -mx is necessary.\n",stderr);
    ParametersError();
    }
 else
     MinX = atof(GetArg("-mx",argc,argv)); 

 if (!ExistArg("-my",argc,argv))
    {
    fputs("Parameter -my is necessary.\n",stderr);
    ParametersError();
    }
 else
     MinY = atof(GetArg("-my",argc,argv)); 


 if (!ExistArg("-sx",argc,argv))
    {
    fputs("Parameter -sx is necessary.\n",stderr);
    ParametersError();
    }
 else
     SizeX = atof(GetArg("-sx",argc,argv)); 

 if (!ExistArg("-sy",argc,argv))
    {
    fputs("Parameter -sy is necessary.\n",stderr);
    ParametersError();
    }
 else
     SizeY = atof(GetArg("-sy",argc,argv)); 


 if (!ExistArg("-mi",argc,argv))
    {
    fputs("Parameter -mi is necessary.\n",stderr);
    ParametersError();
    }
 else
    {
     MaxNIter = atoi(GetArg("-mi",argc,argv));
     if (MaxNIter <1)
        {
         puts("Max. number of Iterations < 1");
         exit(1);
        }
    } 

 if (!ExistArg("-of",argc,argv))
     FileName="Image";
 else
     FileName = GetArg("-of",argc,argv);
     
 if (ExistArg("-o",argc,argv)) 
    GenImage=true; 
    
 if (!ExistArg("-nt",argc,argv))
    {
    fputs("Parameter -nt is necessary.\n",stderr);
    ParametersError();
    }
 else
    {
     NThreads = atoi(GetArg("-nt",argc,argv)); 
     if (NThreads <=0 || NThreads > Rows)
        {
         puts("NThreads must be > 0 and <= Rows.");
         exit(1);
        }
    }  

 if (!ExistArg("-cs",argc,argv))
    ChunckSize=1;
 else
    {
     ChunckSize = atoi(GetArg("-cs",argc,argv)); 
     if (ChunckSize <1 || ChunckSize > Rows)
        {
         puts("ChunckSize must be > 0 and <=Rows.\n");
         exit(1);
        }
    }  


 #if (PRINT==1)
 printf("Img.Rows=%d, Img.Cols=%d,\n", Rows, Cols);
 printf("Mandel.MinX=%f, Mandel.MinY=%f,\n",
         MinX, MinY);
 printf("Mandel.SizeX=%f, Mandel.SizeY=%f,\n",
         SizeX,SizeY);
 printf("Max.N.iterations=%d,  Output=%s.\n",MaxNIter, FileName);
 printf("ChunckSize=%d NThreads=%d.\n",ChunckSize, NThreads);
 #endif
 
 /* Initialize mutex object*/
 pthread_mutex_init(&pth_RowToProcess_mutex, NULL);
 
 //Get mem for threads
 pThreads = (pthread_t *) GetMem(NThreads, sizeof(pthread_t), 
                                 "Main: pThreads");
 //Main thread also computes.
 pTh_Args = (Type_Th_Mandel_Args *) GetMem(NThreads+1, 
                                            sizeof(Type_Th_Mandel_Args),
                                            "Main: ppTh_Args");

 //GetMem for ppRed, ppGreen and ppBlue. It put data to zeroes
 ppRed   = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppRed");
 ppGreen = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppGreen");
 ppBlue  = (char **) GetMem2D (Rows,Cols,sizeof(char),"Main:ppBlue");

 IncX=SizeX/(double)Cols;
 IncY=SizeY/(double)Rows;

 //Create arguments for all Threads, including Main
 for (int t=0;t<NThreads;t++) 
     {
      pTh_Args[t].ppRed		= ppRed;
      pTh_Args[t].ppGreen	= ppGreen;
      pTh_Args[t].ppBlue	= ppBlue;
      pTh_Args[t].MinX		= MinX;
      pTh_Args[t].MinY		= MinY;
      pTh_Args[t].IncX		= IncX;
      pTh_Args[t].IncY		= IncY;
      pTh_Args[t].MaxNIter	= MaxNIter;
      pTh_Args[t].pRowToProcess	= &RowToProcess;
      pTh_Args[t].pChunckSize = &ChunckSize;
      pTh_Args[t].Rows		= Rows;
      pTh_Args[t].Cols		= Cols;
      pTh_Args[t].pth_RowToProcess_mutex= &pth_RowToProcess_mutex;
      pTh_Args[t].ThreadId	= t;
      #if (DEGUG==1)
      pTh_Args[t].NRowsDone = 0;
      #endif
     }

 //Create NThreads-1 threads. Main also work using pTh_Args[0].
 for (int t=1;t<NThreads; t++)
     {
      rc = pthread_create(&pThreads[t], NULL, Th_Mandel, 
                          (void *) &pTh_Args[t]);
      if (rc) 
         {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
         }
     }

 //After creating and run NThreads-1 threads.
 //I (main) thread number 0 also run.
 //pThreads[0] is not used.
 //TODO: Run Th_Mandel() here for the main thread
 Th_Mandel((void *)&pTh_Args[0]);

 //We wait for NThreads-1 to end
 for (int t=1; t<NThreads; t++) 
     {
      rc = pthread_join(pThreads[t], (void **) &status);
      if (rc) 
         {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
          }
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",
             t,*(int *)status);
      fflush(stdout);
      #endif
     }


 if (GenImage)
    { 
	 //Write to the output file
	 FOut=OpenFile(FileName,"wb");
	 
	 for (int i=0;i<Rows;i++)
	     for (int j=0;j<Cols;j++)
	         {
			  fwrite(&ppRed  [i][j],sizeof(char),(size_t)1,FOut);
			  fwrite(&ppGreen[i][j],sizeof(char),(size_t)1,FOut);
			  fwrite(&ppBlue [i][j],sizeof(char),(size_t)1,FOut);
	         }

	 CloseFile(FOut);

	 //rawtopgm and pnmtopng are in netpbm package
	 //Generate the comand to move from raw to png
	 sprintf(Command,"rawtoppm %d %d %s | pnmtopng > %s.png", 
	                 Cols,Rows, FileName, FileName);

	 #if (PRINT==1)
	 printf("Command=%s\n",Command);
	 #endif

	 system(Command);
	 // eog from gnome visualize the image when it change
    }

 //Free memory for ppBlue, ppGreen and ppRed.
 Free2D((void **)ppBlue, Rows);
 Free2D((void **)ppGreen,Rows);
 Free2D((void **)ppRed,  Rows);
 
 pthread_mutex_destroy(&pth_RowToProcess_mutex);

 #if (DEGUG==1)
 for (int t=0;t<NThreads;t++)
     printf("Thread %d did %d rows.\n", t, pTh_Args[t].NRowsDone);
 #endif

 free((void *)pThreads);
 free((void *)pTh_Args);

 pthread_exit(NULL);
 
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
