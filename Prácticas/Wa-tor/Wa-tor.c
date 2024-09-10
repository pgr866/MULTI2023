#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
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
 puts("\t  -r   <n rows image>			[100]	");
 puts("\t  -c   <n columns image>		[100]	");
 puts("\t  -nf  <n fishes>			[2500]	");
 puts("\t  -ns  <n sharks>			[500]	");
 puts("\t  -ni  <max n iter.>			[1000]	");
 puts("\t  -fb  <n iter.fish breed >		 [5]	");
 puts("\t  -sb  <n iter.shark breed>		[40]	");
 puts("\t  -sie <shak init.energy>		[10]	");
 puts("\t  -sef <shark eat fish energy>		[3]	");
 puts("\t  -ffmpeg : to visualise creating a pipe to:"); 
 puts("\t\t | ffplay -f rawvideo -pixel_format rgb24");
 puts("\t\t -video_size <rows>x<cols> -"); 
 puts("\t  -o  (graphical output with eog [Image].");
 puts("\t  -fg <graphical file> 			[Image]	");
 puts("\t  -fd <data file> [data.txt]	");
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
 unsigned char Draw;		//If graphical output with eog Image
 unsigned char FFMPEG;		//If output to ffmpeg
 char *  FileNameDt;		//Output FileName of the Data.
 FILE *  FOutDt;			//File handler of FileNameDt
 unsigned char GenData;		//If iterations data is generated on FileNameDt
 char    CommandRawToPpm[256];//String of the command rawtoppm
 char    CommandGenerateMov[256];
 char    CommandDeletePgm[256];
 int NFishes;				 //Current number of fishes
 int NSharks;				 //Current number of sharks
 DataAnimal *** Ocean;		//Each pixel is NULL or a pointer to an animal. 

 //Inicio de la semilla números aleatórios. $ man 3 srand48 $ man 2 time
 srand48(time(NULL));
 
 //Obtener los parámetros de la línea de comandos.
 if (ExistArg("-h",argc,argv))
    ParametersError();  

 if (!ExistArg("-r",argc,argv))
    Rows=100;
 else
    {
     Rows = atoi(GetArg("-r",argc,argv));
     if (Rows <3)
        {
         puts("Rows<3");
         exit(1);
        }
    } 

 if (!ExistArg("-c",argc,argv))
    Cols=100;
 else
    {
     Cols = atoi(GetArg("-c",argc,argv)); 
     if (Cols <3)
        {
         puts("Col<3");
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
     sprintf(CommandRawToPpm,"rawtoppm %d %d %s | pnmtopng > %s_%04d.png", 
                          Cols,Rows,FileNameGh,FileNameGh,0); 
     system(CommandRawToPpm);
    }

 if (FFMPEG)
    {
     //hpcjmart sprintf(PipeFFPlay,"ffplay -f rawvideo -pixel_format rgb24 "
     //hpcjmart 					" -vf scale=4*iw:4*ih:flags=neighbor "
     //hpcjmart 					" -video_size %dx%d -",Rows,Cols);
     //hpcjmart FPipeFFPlay = popen(PipeFFPlay, "w");
    }
    
 if (GenData==True)
    {
     FOutDt=OpenFile(FileNameDt,"wb");
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
            sprintf(CommandRawToPpm,"rawtoppm %d %d %s | pnmtopng > %s_%04d.png", 
                          Cols,Rows,FileNameGh,FileNameGh,SimIter); 
            system(CommandRawToPpm);
            usleep(200000);
           }
        if (GenData==True)
           fprintf(FOutDt,"%d\t%d\t%d\n", SimIter,NFishes,NSharks);

       }

 //Free ocean
 FreeOcean (Ocean, Rows, Cols);

 
 if (Draw)
    {
     sprintf(CommandGenerateMov,"ffmpeg -framerate 30 -pattern_type glob -i '*.png' -c:v libx264 -pix_fmt yuv420p out.mp4 -y");
     system(CommandGenerateMov);
     sprintf(CommandDeletePgm,"rm -rf *.png");
     system(CommandDeletePgm);   
    }
    
 if (GenData==True)
    {
     CloseFile(FOutDt);
    }


 printf("Wa-tor ends. Niter=%d, NFishes= %d, NSharks=%d.\n", SimIter,NFishes,NSharks);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
