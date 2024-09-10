#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdbool.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"


#define PRINT 		0

#define TagPixel	1
#define TagRed		2
#define TagGreen	3

/*---------------------------------------------------------------------------*/
void DivideMBetweenNForMe(const int Me, const int N, const int M, 
                          int *start, int *n)
{
 //Me in [0,N-1]
 //M elemts to share.
 int n_elem   = M/N;		//How many elements.
 int n_larger = M-N*n_elem;	//Some one have to do one more. 
 int n_start;	//In which n I start.

 #if (PRINT==1)
 printf("DivideMBetweenNForMe: Me=%d,N=%d,M=%d.\n", Me,N,M);
 #endif

 if (Me < n_larger)
    { 
     n_elem = n_elem + 1;
     n_start = Me * n_elem;
    }  
 else 
    if (n_elem > 0)
    { 
     n_start = n_larger + Me * n_elem;
    }
 else 
    { // this process has only zero elements
     n_start = -1;
     n_elem  =  0;
    }

 *n		= n_elem;
 *start	= n_start;
}

/*---------------------------------------------------------------------------*/
void Bandera_Function (char * ppRed, char * ppGreen,
                       const int StartPixel, const int EndPixel, 
                       const int Rows, const int Cols,
                       const int MyRank)
{
 int Row;

 #if (PRINT==1)
 printf("Bandera_Function: Task %d, pixels %d to %d, Rows=%d, Cols=%d.\n",
         MyRank,StartPixel, EndPixel-1, Rows, Cols);
 #endif
        
 for (int k=StartPixel; k<EndPixel; k++)
     {
      Row=k/Cols; //i is the current Row.
      //If Me!=0 I got memory for [0..EndPixel-StartPixel]
      ppRed[k-StartPixel]=(char)255;	//In tasks ppRed starts in 0.
      if (Row>Rows/4 && Row<Rows*3/4)
      ppGreen[k-StartPixel]=(char)255;	//In tasks ppGreen starts in 0.
     }

}

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n columns>		");
 puts("\t  -o <file>		 	");

 exit(0);
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int 		Rows, Cols;
 char 	* ppRed, *ppGreen, *ppBlue;  //Matrices de RGB (0..255)
 char *		FileName;
 FILE * 	FOut;
 char 		Command[256];
 char 		GenFile=False; 	//if output image is generated.
 //double t0, t1; 			// To get time inside MPI
 MPI_Status Stat;			//Status of the Receive.
 int NTasks, MyRank; 		//MPI number of task and Rank for current task.
 int StartPixel, NPixels;	//What do each Task.

 //MPI Initialization
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
 
  //Task 0------------------------
 if (MyRank==0)
    {
     //Get Time inside MPI
     //t0 = MPI_Wtime();
     
	 if (ExistArg("-h",argc,argv))
	    ParametersError();  

	 if (!ExistArg("-r",argc,argv))
	    {
	     fputs("Parameter -r is neccesary.\n",stderr);
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
	     fputs("Parameter -c is neccesary.\n",stderr);
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

 if (ExistArg("-o",argc,argv))
    {
     GenFile=true;
     FileName = GetArg("-o",argc,argv);
	 printf("Rows=%d, Cols=%d, Output=%s.\n", Rows, Cols, FileName);
    } 

     
     printf("Bandera-MPI Task %d: NTasks=%d.\n",MyRank,NTasks);
     puts("--------------------------------------------------------");
     fflush(stdout);
     

     //Calloc de Getmen put data to zeroes
     //Get mem for Red, Green and Blue arrays.
     //Task 0 will store the final image.
	 ppRed   = (char *) GetMem (Rows*Cols,
	                                     sizeof(char),"Main:ppRed");
	 ppGreen = (char *) GetMem (Rows*Cols,
	                                     sizeof(char),"Main:ppGreen");
	 ppBlue  = (char *) GetMem (Rows*Cols,
	                                     sizeof(char),"Main:ppBlue");
	}
	
 
 
 //All Tasks ------------------------
 //Broadcast Rows and Cols: Only task 0 know them.
 MPI_Bcast(&Rows,1,MPI_INT,0,MPI_COMM_WORLD);
 //TODO Broadcast Cols
 MPI_Bcast(&Cols,1,MPI_INT,0,MPI_COMM_WORLD);
 
 //Get the StartPixel and NPixels to process
 DivideMBetweenNForMe(MyRank, NTasks, Rows*Cols, &StartPixel, &NPixels);
 
 #if (PRINT==1)
 printf("Task %d out of %d, responsible of %d pixels: [%d,%d].\n",
        MyRank,NTasks,NPixels,StartPixel,StartPixel+NPixels-1);
 #endif
 
 //Tasks != 0  ----------------------
 if (MyRank!=0)
    {
     ppRed   = (char *) GetMem (NPixels,
                                         sizeof(char),"Task!=0:ppRed");
	 ppGreen = (char *) GetMem (NPixels,
	                                     sizeof(char),"Task!=0:ppGreen");
	 Bandera_Function (ppRed, ppGreen, StartPixel, StartPixel+NPixels, 
                       Rows, Cols, MyRank);
                       
     MPI_Send(&StartPixel,1,      MPI_INT, 0,TagPixel,MPI_COMM_WORLD);
     MPI_Send(ppRed,      NPixels,MPI_CHAR,0,TagRed,  MPI_COMM_WORLD);
     //TODO Send ppGreen
	 MPI_Send(ppGreen, NPixels, MPI_CHAR, 0, TagGreen, MPI_COMM_WORLD);

     free((void *)ppGreen);
     free((void *)ppRed);
    }

 //Task 0 ---------------------------
 if (MyRank==0)
    {	 
     Bandera_Function (ppRed, ppGreen, StartPixel, StartPixel+NPixels, 
                       Rows,Cols,MyRank);
    
    //Wait all tasks returning results.
    for (int t=1; t<NTasks; t++) 
        {
         //Receive From ANY in which Pixel its starts
		 MPI_Recv(&StartPixel, 1, MPI_INT, MPI_ANY_SOURCE, TagPixel, MPI_COMM_WORLD, &Stat);
		 //I do not know How many pixels I will receice, then
		 //TODO Do a MPI_Probe to get info in new Stat, 
		 //of a message with tag TagRed and source old Stat.MPI.SOURCE. 
		 MPI_Probe(Stat.MPI_SOURCE, TagRed, MPI_COMM_WORLD, &Stat);
		 //TODO Obtain the number of sent chars in NPixels
		 MPI_Get_count(&Stat, MPI_CHAR, &NPixels);
		 //TODO: Receive red colors
		 MPI_Recv(&ppRed[StartPixel], NPixels, MPI_CHAR, Stat.MPI_SOURCE, TagRed, MPI_COMM_WORLD, &Stat);
		 //TODO: Receive Green colors
		 MPI_Recv(&ppGreen[StartPixel], NPixels, MPI_CHAR, Stat.MPI_SOURCE, TagGreen, MPI_COMM_WORLD, &Stat);
         #if (PRINT==1)
         printf("Task 0: Receiving %d pivels starting in %d from %d\n", NPixels,StartPixel,Stat.MPI_SOURCE);
         #endif
        }

    if (GenFile)
       {
	    FOut=OpenFile(FileName,"wb");
	    for (int k=0;k<Rows*Cols;k++)
			{
		     fwrite(&ppRed  [k],sizeof(char),(size_t)1,FOut);
			 fwrite(&ppGreen[k],sizeof(char),(size_t)1,FOut);
			 fwrite(&ppBlue [k],sizeof(char),(size_t)1,FOut);
		    }

		CloseFile(FOut);
	 
		//rawtoppm and pnmtopng are in netpbm package
		sprintf(Command,"rawtoppm %d %d %s | pnmtopng > %s.png", 
		        Cols,Rows, FileName, FileName); 
		
		#if (PRINT==1)        
	 	printf("Command=%s\n",Command);
	 	#endif
	 	//eog from gnome visualize the image when it change
	 	system(Command);
	   }
	 free((void *)ppBlue);
     free((void *)ppGreen);
     free((void *)ppRed);
      
//     We wul use the real output of $time ./Bandera-MPI ... instead. 
//     t1 = MPI_Wtime();
//     printf( "Wall clock time WTime= %12.4g sec\n", t1-t0);
     fflush (stdout);
    }
 
 MPI_Finalize();
 
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
