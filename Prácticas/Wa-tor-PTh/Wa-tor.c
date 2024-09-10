#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include "animals.h"
#include "ocean.h"

#define PRINT 0



/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]				");
 puts("\t  -r   <n rows image>			[102]	");
 puts("\t  -c   <n columns image>		[102]	");
 puts("\t  -nf  <n fishes>			[2500]	");
 puts("\t  -ns  <n sharks>			[500]	");
 puts("\t  -ni  <max n iter.>			[1000]	");
 puts("\t  -fb  <n iter.fish breed >		 [5]	");
 puts("\t  -sb  <n iter.shark breed>		[40]	");
 puts("\t  -sie <shak init.energy>		[10]	");
 puts("\t  -sef <shark eat fish energy>		[3]	");
 puts("\t  -fg <graphical file> 			[Image]	");
 puts("\t  -ffmpeg : to visualise video creating a pipe to:"); 
 puts("\t  -o  (graphical output with eog [Image].");
 puts("\t  -fd <data file> 			[data.txt]	");
 puts("\t  -d  (data output is generated.");
 exit(0);
}


/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int     MaxNIter;			//Max number of Iterations.
 int 	 SimIter; 			//Iteration of the simulation.
 int	 NInitFishes;			//Number of fishes
 int 	 NiFBreed;			//Number of iterations a Fish breed.
 int 	 NInitSharks; 			//Number of Sharks
 int 	 NiSBreed;			//Number of iterations a Shark breed.
 int 	 SiEnergy;			//Shark initial energy.
 int 	 SeFEnergy;			//Shark eat fish energy.
 int     Rows, Cols;		//Rows and Cols of the image.
 char *  FileNameGh;		//Output FileName of the image grah.
 bool 	 Draw;				//If graphical output with eog Image
 bool 	 FFMPEG;			//If output to ffmpeg
 char *  FileNameDt;		//Output FileName of the Data.
 FILE *  FOutDt=NULL;		//File handler of FileNameDt
 unsigned char GenData;		//If iterations data is generated on FileNameDt
 char    CommandRawToPpm[256];//String of the command rawtoppm
 char    CommandEog     [256];//String of the command eog
 char    CommandGnuPlot [256];//String of the command gnuplot
 char    PipeFFPlay  	[256];//String of the pipe to FFPlay
 FILE *  FPipeFFPlay=NULL;		//File handler of pipe for FFPlay
 int NFishes;				 //Current number of fishes
 int NSharks;				 //Current number of sharks
 DataAnimal *** Ocean;		//Each pixel is NULL or a pointer to an animal. 
 
 srand48(0); //Initiate the seed of random numbers. Fix a number for repetition
 
 //Obtener los parámetros de la línea de comandos.
 if (ExistArg("-h",argc,argv))
    ParametersError();  

 if (!ExistArg("-r",argc,argv))
    Rows=102;
 else
    {
     Rows = atoi(GetArg("-r",argc,argv));
     if (Rows % 3!=0 )
        {
         puts("Rows must be multiple of 3");
         exit(1);
        }
    } 

 if (!ExistArg("-c",argc,argv))
    Cols=102;
 else
    {
     Cols = atoi(GetArg("-c",argc,argv)); 
     if (Cols % 3 !=0 )
        {
         puts("Col must be multiple of 3");
         exit(1);
        }
    }  

 if (!ExistArg("-nf",argc,argv))
    NInitFishes=2500;
 else
    {
     NInitFishes = atoi(GetArg("-nf",argc,argv));
     if (NInitFishes <=0)
        {
         puts("NInitFishes<=0");
         exit(1);
        }
    } 

 if (!ExistArg("-ns",argc,argv))
    NInitSharks=500;
 else
    {
     NInitSharks = atoi(GetArg("-ns",argc,argv));
     if (NInitSharks <=0)
        {
         puts("NInitSharks<=0");
         exit(1);
        }
    } 

 if (NInitFishes+NInitSharks > Rows*Cols)
    {
     puts("The number of animals > numbre of pixels");
     exit(1);
    }


 if (!ExistArg("-ni",argc,argv))
    MaxNIter = 1000;
 else
    {
     MaxNIter = atoi(GetArg("-ni",argc,argv)); 
     if (MaxNIter <=0)
        {
         puts("MaxNIter<=0");
         exit(1);
        } 
    }

 if (!ExistArg("-fb",argc,argv))
    NiFBreed = 5;
 else
    {    
     NiFBreed = atoi(GetArg("-fb",argc,argv)); 
     if (NiFBreed <=0)
        {
         puts("N. iter. Fish to Breed<=0");
         exit(1);
        } 
    }

 if (!ExistArg("-sb",argc,argv))
    NiSBreed = 40;
 else
    {    
     NiSBreed = atoi(GetArg("-sb",argc,argv)); 
     if (NiSBreed <=0)
        {
         puts("N. iter Shark to Breed<=0");
         exit(1);
        } 
    }

 if (!ExistArg("-sie",argc,argv))
    SiEnergy = 10;
 else
    {    
     SiEnergy = atoi(GetArg("-sie",argc,argv)); 
     if (SiEnergy <=0 || SiEnergy > NiSBreed) //A shark has to eat for breed.
        {
         puts("Shark initial energy <= 0 or");
         puts("Shark initial energy > N. iter Shark to breed");
         exit(1);
        } 
    }

 if (!ExistArg("-sef",argc,argv))
    SeFEnergy = 3;
 else
    {    
     SeFEnergy = atoi(GetArg("-sef",argc,argv)); 
     if (SeFEnergy <=0)
        {
         puts("Shark eaf fish energy <= 0");
         exit(1);
        } 
    }

 if (!ExistArg("-fg",argc,argv))
     FileNameGh="Image";
 else
     FileNameGh = GetArg("-fg",argc,argv); 

 if (!ExistArg("-o",argc,argv))
     Draw = False;
 else
     Draw = True; 

 if (!ExistArg("-ffmpeg",argc,argv))
     FFMPEG = False;
 else
     FFMPEG = True; 

 if (!ExistArg("-fd",argc,argv))
     FileNameDt="data.txt";
 else
     FileNameDt = GetArg("--fd",argc,argv); 

 if (!ExistArg("-d",argc,argv))
     GenData = False;
 else
     GenData = True; 

 #if (PRINT==1)
 printf("Img.Rows=%d, Img.Cols=%d,\n", Rows, Cols);
 printf("Fish number=%d,  N. iter. fish breed=%d.\n",NInitFishes, NiFBreed);
 printf("Shark number=%d, N. iter.shark breed=%d.\n",NInitSharks, NiSBreed);
 printf("Shark init. energy=%d, Shark eat fish energy=%d.\n",SiEnergy, SeFEnergy);
 printf("Max. number of iterations=%d.\n",MaxNIter);
 printf("Draw=%d\n",Draw);
 if (Draw)
    printf("Draw file=%s.\n", FileNameGh);
 printf("GenData=%d\n",GenData);
 if (GenData)
    printf("Output data file=%s.\n", FileNameDt);
 printf("FFMPEG=%d\n",FFMPEG);
 #endif

 //GetMem for the Ocean
 //Ocean is a pointer to a vector of pointers of sixe Rows (one *)
 //Ocean[i] is a pointer to a vector of pointers of size Cols (another *)
 //Ocean[i][j] is a pointer to DataAnimal (another *)  
 Ocean = (DataAnimal ***) GetMem2D (Rows,Cols,sizeof(DataAnimal **),"Main: ocean"); 

 #if (PRINT==1)
 if (Ocean[0][0]==NULL)
    printf("GetMem2D initiates pointers of Ocean to NULL.\n");
 #endif

 //Initialize variables
 SimIter = 0;  //First iteration of the simulation.
 NFishes = NInitFishes;
 NSharks = NInitSharks;
 InitOcean(Ocean,Rows,Cols,NInitFishes,NInitSharks,NiFBreed,NiSBreed,SiEnergy);
 if (Draw)
    {
     OceanToRGBFile (Ocean,FileNameGh,Rows,Cols);
     sprintf(CommandRawToPpm,"rawtoppm %d %d %s > %s.ppm", 
                          Cols,Rows, FileNameGh, FileNameGh); 
     system(CommandRawToPpm);
 
     /* eog -f from gnome visualize the image when it change*/
     sprintf(CommandEog,"eog -f -g %s.ppm &\n", FileNameGh); 
     //sprintf(CommandEog,"gwenview -f %s.ppm &\n", FileNameGh); 
     system(CommandEog);
     sleep(1);
    }

 if (FFMPEG)
    {
     sprintf(PipeFFPlay,"ffplay -f rawvideo -pixel_format rgb24"
     					" -vf scale=4*iw:4*ih:flags=neighbor "
     					" -video_size %dx%d -",Rows,Cols);
     FPipeFFPlay = popen(PipeFFPlay, "w");
    }

 if (GenData==True)
    {
     FOutDt=OpenFile(FileNameDt,"w");
     fprintf(FOutDt,"%d\t%d\t%d\n", SimIter,NFishes,NSharks);
    }

 while (SimIter < MaxNIter && NFishes >0 && NSharks > 0)
       {
        IterateOcean(Ocean,Rows,Cols,SimIter,&NFishes,&NSharks,
                     NiFBreed,NiSBreed,SiEnergy,SeFEnergy);
        SimIter++;
        if (Draw)
           {
            OceanToRGBFile (Ocean,FileNameGh,Rows,Cols);
            system(CommandRawToPpm);
            sleep(1);
           }
           
        if (GenData==True)
           fprintf(FOutDt,"%d\t%d\t%d\n", SimIter,NFishes,NSharks);

        if (FFMPEG)
           {
			unsigned char White[]={255, 255, 255};
			unsigned char Red[]  ={255,   0,   0};
			unsigned char Blue[] ={  0,   0, 255};
		 	for (int i=0; i<Rows; i++)
				for (int j=0; j<Cols; j++)
				 	{
				 	 if (Ocean[i][j]==NULL)
				 		 {
				 	  	  fwrite(White, sizeof(char),3,FPipeFFPlay);
				 	  	  continue;
				 	  	 }
				 	  if (Ocean[i][j]->Animal==FISH)
				 		 {
				 	  	  fwrite(Blue, sizeof(char),3,FPipeFFPlay);
				 	  	  continue;
				 	  	 }
				 	  if (Ocean[i][j]->Animal==SHARK)
				 		 {
				 	  	  fwrite(Red, sizeof(char),3,FPipeFFPlay);
				 	  	  continue;
				 	  	 }
				 	  fflush(FPipeFFPlay);
				 	 }
			//usleep(200000);
           }
       }

 //Free ocean
 FreeOcean (Ocean, Rows, Cols);

 if (GenData==True)
    {
     CloseFile(FOutDt);
     sprintf(CommandGnuPlot,"echo 'plot \"%s\" using 1:2 with lines title \"fishes\","
                   "\"%s\" using 1:3 with lines title \"sharks\"' | gnuplot -persist", 
                   FileNameDt, FileNameDt);
     #if (PRINT==1)
     printf("%s\n", CommandGnuPlot); 
     #endif
     system(CommandGnuPlot);
    }

 if (FFMPEG)
 	pclose(FPipeFFPlay);

 printf("Wa-tor ends. Niter=%d, NFishes= %d, NSharks=%d.\n", SimIter,NFishes,NSharks);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
