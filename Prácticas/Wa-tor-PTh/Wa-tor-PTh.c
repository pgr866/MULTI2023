#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include "animals.h"
#include "ocean.h"

#define PRINT 0

/*---------------------------------------------------------------------------*/
typedef struct th_Wator_args
{
 DataAnimal *** Ocean;
 int		Rows;
 int		Cols;
 int *		pSimIter;
 int *		pNFishes;
 int *		pNSharks;
 int 		NiFBreed;
 int 		NiSBreed;
 int 		SiEnergy;
 int 		SeFEnergy;
 STDrand48Data * pRandData;	//For lrand48_r().
 int *		pRowToProcess; //Shared variable to know which row process next.
 pthread_mutex_t *   pth_RowToProcess_mutex; 	//To update RowToProcess
 pthread_mutex_t *   pth_Counters_mutex;		//To update pNFishes and pNSharks
 pthread_barrier_t * pth_NextIter_Starts_barrier;//To synchronice iterations
 pthread_barrier_t * pth_NextIter_Ends_barrier;	//To synchronice iterations
 pthread_barrier_t * pth_Loop_Starts_barrier;	//To synchronice between loops
 pthread_barrier_t * pth_Loop_Ends_barrier;		//To synchronice between loops
 bool  * pFinish;		//To finish threads.
 int 		ThreadId;
} Type_Th_Wator_Args;

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
 puts("\t  -nt <Numbre of Threads>");
 exit(0);
}

/*---------------------------------------------------------------------------*/
void Th_IterateRows(void * pTh_Args, int *pLocalNFishes, int *pLocalNSharks)
{
 Type_Th_Wator_Args * pMyData;
 pMyData=(Type_Th_Wator_Args *) pTh_Args;
 int MyRow=pMyData->Rows; //Set to something impossible

 while (*(pMyData->pRowToProcess)< pMyData->Rows)
	   {
        MyRow=pMyData->Rows; //Set to something impossible

		//TODO Lock Mutex to update pRowToProcess
      pthread_mutex_lock(pMyData->pth_RowToProcess_mutex);

        if (*(pMyData->pRowToProcess) < pMyData->Rows)
           {
            MyRow  =  *(pMyData->pRowToProcess); //Get the Row.
            #if (PRINT==1)
			 printf("Th %d SimIter=%d, RowToProcess=%d\n", 
			 pMyData->ThreadId,*(pMyData->pSimIter), *(pMyData->pRowToProcess));
			 fflush(stdout);
		    #endif
           *(pMyData->pRowToProcess)+=3; //To avoid race conditions
           }
		//TODO Unlock mutex after udating pRowToProcess
      pthread_mutex_unlock(pMyData->pth_RowToProcess_mutex);

		if (MyRow < pMyData->Rows)
		   {
		    for (int j=0; j<pMyData->Cols; j++)
			    {
		         if (pMyData->Ocean[MyRow][j]!=NULL && 
		             pMyData->Ocean[MyRow][j]->Animal==FISH)
			         IterateFish(pMyData->Ocean,pMyData->Rows,pMyData->Cols,
			                     MyRow,j,*(pMyData->pSimIter),
			                     pLocalNFishes,pMyData->NiFBreed,
			                     pMyData->pRandData);

		         if (pMyData->Ocean[MyRow][j]!=NULL && 
		             pMyData->Ocean[MyRow][j]->Animal==SHARK)
			        IterateShark(pMyData->Ocean,pMyData->Rows,pMyData->Cols,
			                     MyRow,j,*(pMyData->pSimIter),
				                 pLocalNFishes, pLocalNSharks, pMyData->NiSBreed,
				                pMyData->SiEnergy, pMyData->SeFEnergy,
				                pMyData->pRandData);
			    }
		   }
	   }
}

/*---------------------------------------------------------------------------*/
void *Th_IterateOcean(void * pTh_Args)
{
 //Local variables are local to each thread.
 Type_Th_Wator_Args * pMyData;
 int LocalNFishes; 
 int LocalNSharks; 
 
 pMyData=(Type_Th_Wator_Args *) pTh_Args;
 
 while (true)
       {
		//------------------------------------------
		//All threads synchronise before each iteration. 
		//Main thread set before RowToProcess=0. 

		pthread_barrier_wait(pMyData->pth_NextIter_Starts_barrier);
		
		#if (PRINT==1)
		printf("Th %d after NextIter Starts barrier. "
               "SimIter=%d, RowToProcess=%d\n", 
		       pMyData->ThreadId,*(pMyData->pSimIter), 
			   *(pMyData->pRowToProcess));
		fflush(stdout);
		#endif
		
		//Main could set *pFinish=true after I check the while.
		if (*(pMyData->pFinish))
		   {
			#if (PRINT==1)
			printf("Th %d finishes.\n",pMyData->ThreadId);
			fflush(stdout);
			#endif
		    break; //Leave the while and finish.
		   }
		//Set my local counter of fishes and sharks to 0 in each iteration.
		LocalNFishes=0;
		LocalNSharks=0;
		
		//Threads iterate over rows starting in 0 and adding 3.
		Th_IterateRows(pTh_Args, &LocalNFishes, &LocalNSharks);
		
		//TODO Wait all threads ends. 
		//RowToProcess>Rows. But some threads may still works
      pthread_barrier_wait(pMyData->pth_Loop_Ends_barrier);

		
		//------------------------------------------
		//First thread set RowToProcess=1 before next loop.
		//Next Srtarts in 1 adding 3
		if (pMyData->ThreadId==0)
		   {
		    *(pMyData->pRowToProcess)=1;
		    #if (PRINT==1)
		    puts("----------------------");
		    printf("Thread 0 set RowToProcess=1\n");
		    #endif
		   }

		//TODO Synchronise in loop barrier before starts next loop
      pthread_barrier_wait(pMyData->pth_Loop_Starts_barrier);

		#if (PRINT==1)
		printf("Thread %d after 1 loop barrier. SimIter=%d, RowToProcess=%d\n", 
		       pMyData->ThreadId,*(pMyData->pSimIter), *(pMyData->pRowToProcess));
		fflush(stdout);
		#endif
		//Threads iterate over rows starting in 1 and adding 3.
		Th_IterateRows(pTh_Args, &LocalNFishes, &LocalNSharks);

		//TODO Wait all threads ends. 
		//RowToProcess>Rows. But some threads may still works
      pthread_barrier_wait(pMyData->pth_Loop_Ends_barrier);


		//------------------------------------------ 
		//Previous loop ends.
		//Next Srtarts in 2 adding 3
		if (pMyData->ThreadId==0)
		   {
		    *(pMyData->pRowToProcess)=2;
		    #if (PRINT==1)
		    puts("----------------------");
		    printf("Thread 0 set RowToProcess=2\n");
		    #endif
		   }

		//synchronise in loop barrier
		pthread_barrier_wait(pMyData->pth_Loop_Starts_barrier);
		#if (PRINT==1)
		printf("Thread %d before 2 loop barrier. SimIter=%d, RowToProcess=%d\n", 
		       pMyData->ThreadId,*(pMyData->pSimIter), *(pMyData->pRowToProcess));
		fflush(stdout);
		#endif
		
		//Threads iterate over rows starting in 2 and adding 3.
		Th_IterateRows(pTh_Args, &LocalNFishes, &LocalNSharks);

		//Current iteration finish.
		//Each thread updates shared counters of fishes and sharks
		//using mutual exclusion
		//TODO Lock to udate pNFishes and pNSharks
      pthread_mutex_lock(pMyData->pth_Counters_mutex);

		(*pMyData->pNFishes)+=LocalNFishes;
      (*pMyData->pNSharks)+=LocalNSharks;
      //TODO UnLock after updating pNFishes and pNSharks
      pthread_mutex_unlock(pMyData->pth_Counters_mutex);


		//TODO All threads and main synchronise after each iteration. 
		//Thus all shared counters were updated.
		//Thus, main thread could decide if continue to next iteration.
      pthread_barrier_wait(pMyData->pth_NextIter_Ends_barrier);

		
		#if (PRINT==1)
		printf("Thread %d after NextIter Ends barrier. SimIter=%d, RowToProcess=%d\n", 
		       pMyData->ThreadId,*(pMyData->pSimIter), *(pMyData->pRowToProcess));
		fflush(stdout);
		#endif
	   }
	   
	   
 //Each thread finishes.
 pthread_exit((void *)&(pMyData->ThreadId));
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
 bool    GenData;		//If iterations data is generated on FileNameDt
 char    CommandRawToPpm[256];//String of the command rawtoppm
 char    CommandEog     [256];//String of the command eog
 char    CommandGnuPlot [256];//String of the command gnuplot
 char    PipeFFPlay  	[256];//String of the pipe to FFPlay
 FILE *  FPipeFFPlay=NULL;		//File handler of pipe for FFPlay
 int 	 NFishes;			//Current number of fishes
 int 	 NSharks;			//Current number of sharks
 DataAnimal *** Ocean;		//Each pixel is NULL or a pointer to an animal. 
 int 	NThreads; 			//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_Wator_Args *pTh_Args=NULL;
 int 	rc;					//Return value  of pthread_create and join 
 void *	status;				//status returned in pthread_join.
 int	RowToProcess=0;		//Next row to process.
 pthread_mutex_t   pth_RowToProcess_mutex;		//To update RowToProcess
 pthread_mutex_t   pth_Counters_mutex;			//To update pNFishes and pNSharks
 pthread_barrier_t pth_NextIter_Starts_barrier;	//To synchronice iterations
 pthread_barrier_t pth_NextIter_Ends_barrier;	//To synchronice iterations
 pthread_barrier_t pth_Loop_Starts_barrier;		//To synchronice between loops
 pthread_barrier_t pth_Loop_Ends_barrier;		//To synchronice between loops
 bool Finish=false; 							//To finish threads; 
 STDrand48Data *pRandData=NULL; 				//drand48_data[t] for each thread.
 
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
     GenData = false;
 else
     GenData = true; 

 if (!ExistArg("-nt",argc,argv))
    {
    fputs("Parameter -nt is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     NThreads = atoi(GetArg("-nt",argc,argv)); 
     if (NThreads <=0 || NThreads > Rows/3)
        {
         puts("NThreads must be > 0 and <= Rows/3.");
         exit(1);
        }
    }  

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
 printf("NThreads=%d.\n", NThreads);
 #endif

/* Initialize mutex objects*/
 pthread_mutex_init(&pth_RowToProcess_mutex, NULL);
 pthread_mutex_init(&pth_Counters_mutex,     NULL); //Update NFishes and NSharks
 
 /* Initialize barrier objects*/
 pthread_barrier_init(&pth_NextIter_Starts_barrier, NULL, NThreads+1);	//Threads + main
 pthread_barrier_init(&pth_NextIter_Ends_barrier,   NULL, NThreads+1);	//Threads + main
 //TODO: initiate other barriers
 pthread_barrier_init(&pth_Loop_Starts_barrier, NULL, NThreads);	//Threads 
 pthread_barrier_init(&pth_Loop_Ends_barrier,   NULL, NThreads);	//Threads 


 //GetMem for the Ocean
 //Ocean is a pointer to a vector of pointers of sixe Rows (one *)
 //Ocean[i] is a pointer to a vector of pointers of size Cols (another *)
 //Ocean[i][j] is a pointer to DataAnimal (another *)  
 Ocean = (DataAnimal ***) GetMem2D (Rows,Cols,sizeof(DataAnimal **),"Main: ocean"); 

 #if (PRINT==1)
 if (Ocean[0][0]==NULL)
    printf("GetMem2D initiates pointers of Ocean to NULL.\n");
 #endif

 //Get mem for threads
 pThreads = (pthread_t *) GetMem(NThreads, sizeof(pthread_t), 
                                 "Main: pThreads");
 //Get mem for args to threads.
 pTh_Args = (Type_Th_Wator_Args *) GetMem(NThreads, 
                                           sizeof(Type_Th_Wator_Args),
                                           "Main: ppTh_Args");
                                           
 //Get mem for pRandData
 pRandData=(STDrand48Data *) GetMem(NThreads, sizeof(STDrand48Data),
 								     "pRandomData");
 //Init seeds
 for (int i=0;i<NThreads;i++)
 	 srand48_r(i,&pRandData[i]);

 //Initialize other variables
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

 if (GenData==true)
    {
     FOutDt=OpenFile(FileNameDt,"w");
     fprintf(FOutDt,"%d\t%d\t%d\n", SimIter,NFishes,NSharks);
    }


 //Create threads
 for (int t=0;t<NThreads; t++)
     {
      pTh_Args[t].Ocean		= Ocean;
      pTh_Args[t].Rows		= Rows;
      pTh_Args[t].Cols		= Cols;
      pTh_Args[t].pSimIter	= &SimIter;
      pTh_Args[t].pNFishes	= &NFishes;
      pTh_Args[t].pNSharks	= &NSharks;
      pTh_Args[t].NiFBreed	= NiFBreed;
      pTh_Args[t].NiSBreed	= NiSBreed;
      pTh_Args[t].SiEnergy	= SiEnergy;
      pTh_Args[t].SeFEnergy	= SeFEnergy;
      pTh_Args[t].pRandData	= &pRandData[t];
      pTh_Args[t].pRowToProcess	= &RowToProcess;
      pTh_Args[t].pth_RowToProcess_mutex		= &pth_RowToProcess_mutex;
      pTh_Args[t].pth_Counters_mutex			= &pth_Counters_mutex;
      pTh_Args[t].pth_NextIter_Starts_barrier	= &pth_NextIter_Starts_barrier;
      pTh_Args[t].pth_NextIter_Ends_barrier		= &pth_NextIter_Ends_barrier;
      pTh_Args[t].pth_Loop_Starts_barrier		= &pth_Loop_Starts_barrier;
      pTh_Args[t].pth_Loop_Ends_barrier			= &pth_Loop_Starts_barrier;
      pTh_Args[t].pFinish	= &Finish;
      pTh_Args[t].ThreadId	= t;

      rc = pthread_create(&pThreads[t], NULL, Th_IterateOcean, 
                          (void *) &pTh_Args[t]);
      if (rc) 
         {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
         }
      #if (PRINT==1)
      printf("Main: thread %d created.\n",t);
      fflush(stdout);
      #endif 
     } 

 while (SimIter < MaxNIter && NFishes >0 && NSharks > 0)
       {
        //IterateOcean(Ocean,Rows,Cols,SimIter,&NFishes,&NSharks,
        //     NiFBreed,NiSBreed,SiEnergy,SeFEnergy,pRanddata);
        //We do it here with Threads
        //Set RowToProcess to 0        
        RowToProcess=0;
        
		#if (PRINT==1)
        puts("----------------------------------------------------");
        printf("Main: before NIter barrier starts.SimIter=%d, RowToProcess=%d\n",
               SimIter, RowToProcess);
        fflush(stdout);
        #endif 

		//Wait on the NextIter barrier all threads before iteration starts
		pthread_barrier_wait(&pth_NextIter_Starts_barrier);
		//Threads read RowToProcess=0 and work on this iteration

		
		//Wait on the NextIter barrier all threads after iteration ends
		pthread_barrier_wait(&pth_NextIter_Ends_barrier);
		//Shared counters are updated: NFishes and NSharks
		#if (PRINT==1)
        puts("----------------------------------------------------");
        printf("Main: after NIter barrier ends. SimIter=%d, RowToProcess=%d.\n",
               SimIter, RowToProcess);
        fflush(stdout);
        #endif 

        SimIter++;
        if (Draw)
           {
            OceanToRGBFile (Ocean,FileNameGh,Rows,Cols);
            system(CommandRawToPpm);
            sleep(1);
           }

        if (GenData==true)
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

 //Tell all threads that they must finish
 for (int t=0;t<NThreads; t++)
     *(pTh_Args[t].pFinish)=true;

 //All threads are waiting on the NextIter barrier-> let them progress
 #if (PRINT==1)
 puts("----------------------------------------------------");
 printf("Main: Out of the loop before NIter barrier starts." 
        "SimIter=%d, RowToProcess=%d.\n",SimIter, RowToProcess);
 fflush(stdout);
 #endif
 pthread_barrier_wait(&pth_NextIter_Starts_barrier);
 
 //Do the threads join
 for (int t=0;t<NThreads; t++)
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


 //Free pRandData
 free((void *) pRandData);

 //Free Threads 
 free((void *)pTh_Args);
 //Free args to threads
 free((void *)pThreads);

 //Free mutexes
 pthread_mutex_destroy(&pth_Counters_mutex);
 pthread_mutex_destroy(&pth_RowToProcess_mutex);

 //Free barriers
 pthread_barrier_destroy(&pth_Loop_Ends_barrier);
 pthread_barrier_destroy(&pth_Loop_Starts_barrier);
 pthread_barrier_destroy(&pth_NextIter_Ends_barrier);
 pthread_barrier_destroy(&pth_NextIter_Starts_barrier);

 //Free ocean
 FreeOcean (Ocean, Rows, Cols);

 if (GenData==true)
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
