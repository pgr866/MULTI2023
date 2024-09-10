#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"


#define	PRINT 0

// TAGs:
#define	TagRowToDo		1	//Master send the Row to do 
#define TagRowDone 		2	//Worker return the number of the done row
#define TagRowRed		3	//Worker return the data of the red done row
#define TagRowGreen		4	//Worker return the data of the green done row
#define TagRowBlue		5	//Worker return the data of the blue done row

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
 puts("\t..-mi <max n of iterations in pixel>   ");
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
 //Task != 0 only get memory for a row = 0
 NIterToRGB(NIter,&(ppRed[0][j]),&(ppGreen[0][j]),&(ppBlue[0][j]));
 
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
 double		IncX, IncY; 		//Pixels width in x and y.
 int 		RowToDo=0;			//Next row to be done by a tasks.
 int 		RowDone=0;			//Row done by a task.
 
 
 //double t0=0, t1=0; 		// To get time inside MPI
 MPI_Status Stat;			//Status of the Receive.
 int NTasks, MyRank; 		//MPI number of task and Rank for current task.
 int NTasksEnd=0;			//How many tasks ended.
 int * pNRowsDoneByTask;	//How many rows does each task. 
 
 //MPI Initialization
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
 
 if (NTasks<2)
    {
     printf("NTasks=%d < 2.\n",NTasks);
     puts("Master do not process. So, NTasks must be >=2");
     exit(1);
    }

  //Task 0------------------------
 if (MyRank==0)
    {
     //Get Time inside MPI
     //t0 = MPI_Wtime();

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
		GenImage=True; 


	 #if (PRINT==1)
	 printf("Img.Rows\t= %d.\n"    ,Rows);
	 printf("Img.Cols\t= %d.\n"    ,Cols);
	 printf("Mandel.MinX\t= %g.\n" ,MinX);
	 printf("Mandel.MinY\t= %g,\n" ,MinY);
	 printf("Mandel.SizeX\t= %g.\n",SizeX);
	 printf("Mandel.SizeY\t= %g,\n",SizeY);
	 printf("Max.N.iter.\t= %d.\n" ,MaxNIter);
	 printf("Output \t\t= %s.\n"   ,FileName);
	 #endif

	 //Get mem for NRowsDone by each talk
	 pNRowsDoneByTask = (int *) GetMem(NTasks,sizeof(int),"Main:pNRowsdone");
	}


 //All Tasks ------------------------
 //Get all common parameters. Only Task 0 know them.
 MPI_Bcast(&MinX,     1,MPI_DOUBLE,0,MPI_COMM_WORLD);
 MPI_Bcast(&MinY,     1,MPI_DOUBLE,0,MPI_COMM_WORLD);
 MPI_Bcast(&SizeX,    1,MPI_DOUBLE,0,MPI_COMM_WORLD);
 MPI_Bcast(&SizeY,    1,MPI_DOUBLE,0,MPI_COMM_WORLD);
 MPI_Bcast(&MaxNIter, 1,MPI_INT   ,0,MPI_COMM_WORLD);
 MPI_Bcast(&Rows,     1,MPI_INT   ,0,MPI_COMM_WORLD);
 MPI_Bcast(&Cols,     1,MPI_INT   ,0,MPI_COMM_WORLD);

 IncX=SizeX/(double)Cols;
 IncY=SizeY/(double)Rows;
 
 
 if (MyRank==0)	//Task 0------------------------
    { 
     //GetMem for ppRed, ppGreen and ppBlue. It put data to zeroes
     //Task 0 will store the final image.
	 ppRed   = (char **) GetMem2D (Rows,Cols,sizeof(char),"Task 0 ppRed");
	 ppGreen = (char **) GetMem2D (Rows,Cols,sizeof(char),"Task 0 ppGreen");
	 ppBlue  = (char **) GetMem2D (Rows,Cols,sizeof(char),"Task 0 ppBlue");

 	 //Feed workers
 	 for (int i=1;i<NTasks ;i++)
 		 {
 		  if (RowToDo < Rows)
 		     {
 		      #if (PRINT>=1)
 		      printf("Taks %d send. RowToDo %d to Task %d,\n",
 		             MyRank,RowToDo,i);
 		      #endif
 		      MPI_Send(&RowToDo,1,MPI_INT, i,TagRowToDo,MPI_COMM_WORLD);
 		      RowToDo++;
 		     }
 		  else //Rows done but tasks are waiting
 		     {
 		      //RowToDo=Rows -> task end.
 		      #if (PRINT>=1)
 		      printf("Taks %d send. RowToDo %d -> Finish to Task %d,\n",
 		             MyRank,RowToDo,i);
 		      #endif
 		      MPI_Send(&RowToDo,1,MPI_INT, i,TagRowToDo,MPI_COMM_WORLD);
 		      NTasksEnd++;
 		     }
 		 }
 	
 	 while (NTasksEnd < NTasks-1)
 		   {
 		    MPI_Recv(&RowDone,1,MPI_INT,
 		             MPI_ANY_SOURCE,TagRowDone,MPI_COMM_WORLD,&Stat);
 		    #if (PRINT>=1)
 		    printf("Taks %d recv. RowDone %d from Task %d,\n",
 		           MyRank,RowDone,Stat.MPI_SOURCE);
 		    #endif         
 		    MPI_Recv(&ppRed  [RowDone][0],Cols,MPI_CHAR,
 		             Stat.MPI_SOURCE,TagRowRed,  MPI_COMM_WORLD,&Stat);
 		    MPI_Recv(&ppGreen[RowDone][0],Cols,MPI_CHAR,
 		             Stat.MPI_SOURCE,TagRowGreen,MPI_COMM_WORLD,&Stat);
 		    MPI_Recv(&ppBlue [RowDone][0],Cols,MPI_CHAR,
 		             Stat.MPI_SOURCE,TagRowBlue, MPI_COMM_WORLD,&Stat);
 		    #if (PRINT>=1)
 		    printf("Taks %d recv. RGB of row %d from Task %d,\n",
 		           MyRank,RowDone,Stat.MPI_SOURCE);
 		    #endif

 		    //Increase the number of rows done by that task.
 		    pNRowsDoneByTask[Stat.MPI_SOURCE]++;

 		    if (RowToDo < Rows)
 			   {
 			    #if (PRINT>=1)
 		        printf("Taks %d send. RowToDo %d to Task %d,\n",
 		               MyRank,RowToDo,Stat.MPI_SOURCE);
 		        #endif
 		        MPI_Send(&RowToDo,1,MPI_INT, Stat.MPI_SOURCE,TagRowToDo,MPI_COMM_WORLD);
 		        RowToDo++;
 		       }
 		    else //Rows done but tasks are waiting
 		       {
 		        #if (PRINT>=1)
 		        printf("Taks %d send. RowToDo %d -> Finish to Task %d,\n",
 		             MyRank,RowToDo,Stat.MPI_SOURCE);
 		        #endif
 		        //RowToDo=Rows -> task end.
 		        MPI_Send(&RowToDo,1,MPI_INT,Stat.MPI_SOURCE,TagRowToDo,MPI_COMM_WORLD);
 		        NTasksEnd++;
 		       }
 		   }
 	}
 else			//Task != 0------------------------
	{
	 bool TaskNot0GetMem=false; //Task not 0 gets memory. 
	 //TODO Receive he RowTodo from Task 0
	 MPI_Recv(&RowToDo, 1, MPI_INT, 0, TagRowToDo, MPI_COMM_WORLD, &Stat);

	 #if (PRINT>=1)
     printf("Taks %d recv. RowToDo %d from Task %d,\n",
 		    MyRank,RowToDo,Stat.MPI_SOURCE);
 	 #endif
	 if (RowToDo<Rows && !TaskNot0GetMem)
	    {
	 	 //GetMem for ppRed, ppGreen and ppBlue. It put data to zeroes
     	 //Task!=0 will calculate just one row each time.
	 	 ppRed   = (char **) GetMem2D (1,Cols,sizeof(char),"Task 0 ppRed");
	 	 ppGreen = (char **) GetMem2D (1,Cols,sizeof(char),"Task 0 ppGreen");
	 	 ppBlue  = (char **) GetMem2D (1,Cols,sizeof(char),"Task 0 ppBlue");
	 	 TaskNot0GetMem=true; //Task not 0 gets memory.
	    }
	    
	 while (RowToDo < Rows)
	      {
	       for (int j=0;j<Cols;j++) 
	           PixelMandel(ppRed,ppGreen,ppBlue,
                           MinX, MinY, IncX, IncY,
                           MaxNIter, RowToDo, j);
           
           RowDone=RowToDo;
           //TODO: Send to taks 0 the number of the calculated row 
           MPI_Send(&RowDone, 1, MPI_INT, 0, TagRowDone, MPI_COMM_WORLD);

           #if (PRINT>=1)
 		   printf("Taks %d send. RowDone %d to Task 0,\n",
 		           MyRank,RowDone);
 		    #endif  
 		   //TODO: Send to task 0 calculated RGB 
           MPI_Send(&ppRed[0][0], Cols, MPI_CHAR, 0, TagRowRed, MPI_COMM_WORLD);
           MPI_Send(&ppGreen[0][0], Cols, MPI_CHAR, 0, TagRowGreen, MPI_COMM_WORLD);
           MPI_Send(&ppBlue[0][0], Cols, MPI_CHAR, 0, TagRowBlue, MPI_COMM_WORLD);

           #if (PRINT>=1)
 		   printf("Taks %d send. RGB of RowDone %d to Task 0,\n",
 		           MyRank,RowDone);
 		   #endif  
           
           
           //TODO: Receive from task 0 the next row to do
           MPI_Recv(&RowToDo, 1, MPI_INT, 0, TagRowToDo, MPI_COMM_WORLD, &Stat);

           #if (PRINT>=1)
           if (RowToDo==Rows)
              printf("Taks %d recv. RowToDo %d ->Finish from Task %d,\n",
 		             MyRank,RowToDo,Stat.MPI_SOURCE);
 		   else
 		      printf("Taks %d recv. RowToDo %d from Task %d,\n",
 		             MyRank,RowToDo,Stat.MPI_SOURCE);
 	       #endif
	      }
	//RowToDo=Rows -> finish
	//Free allocated memory
	if (TaskNot0GetMem)
	   {
	    Free2D((void **)ppBlue, 1);
 	    Free2D((void **)ppGreen,1);
 	    Free2D((void **)ppRed,  1);
	   }
	}

 //Work was done.
 //Task 0 ends with the optional work of writing the file
 //and get times.
 if (MyRank==0)	//Task 0------------------------
    { 
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
		 fflush(stdout);
		 #endif
		 system(Command);
		 // eog from gnome visualize the image when it change
		}
		

	 //Task 0 prints NRows done by each task
	 for (int i=0;i<NTasks;i++)
	 	 printf("Task %d did %d rows.\n",i,pNRowsDoneByTask[i]);

	 //Task!=0 already free their pointers
	 free((void *) pNRowsDoneByTask);
	 Free2D((void **)ppBlue, Rows);
 	 Free2D((void **)ppGreen,Rows);
 	 Free2D((void **)ppRed,  Rows);

// Use real part of $ time Mandel-MPI. instead
//     t1 = MPI_Wtime();
//     printf( "Wall clock time WTime= %12.4g sec\n", t1-t0);
     fflush (stdout);
	}

 MPI_Finalize();
 
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
