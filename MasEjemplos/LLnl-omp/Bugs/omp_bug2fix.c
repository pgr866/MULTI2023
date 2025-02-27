/******************************************************************************
* FILE: omp_bug2.c
* DESCRIPTION:
*   Another OpenMP program with a bug. 
* AUTHOR: Blaise Barney 
* LAST REVISED: 04/06/05 
* Corrected by L.G.Casado 03/10/23
*   total was a shared variable and overwritten by all threads.
*   Using reduction on it solves de problem
*   See also how variables are used locally in the parallel bodies instead 
*   than on the main.
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
 double total=0.0; //float is not enough: weird results

/*** Spawn parallel region ***/
#pragma omp parallel default(none) shared(total) 
  {
   /* Obtain thread number */
   int tid = omp_get_thread_num();
   /* Only master thread does this */
   if (tid == 0) {
    int nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d is starting...\n",tid);
 

  /* do some work */
  #pragma omp for schedule(dynamic,10) reduction(+:total)
  for (int i=0; i<1000000; i++) 
     total = total + i*1.0;

  //There is an implicit barrier after a #pragma omp for
  printf ("Thread %d is done! Total= %e\n",tid,total);

  } /*** End of parallel region ***/
 return 0;
}
