/******************************************************************************
* FILE: omp_bug4.c
* DESCRIPTION:
*   This very simple program causes a segmentation fault.
* AUTHOR: Blaise Barney  01/09/04
* LAST REVISED: 04/06/05
* Fix by LGCasado 04/10/23
* Do $ ulimit -s 200000
*   The updating of a shared matrix a were not done inside a omp for.
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1048

int main (int argc, char *argv[])
{
 double a[N][N];
 
 // Fork a team of threads with explicit variable scoping 
 #pragma omp parallel default(none) shared(a)
  {
  // Obtain/print thread info 
  int tid = omp_get_thread_num();
  if (tid == 0) 
    {
    int nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d starting...\n", tid);
  #pragma omp barrier

  /* False: Each thread works on its own private copy of the array */
  /* True: Each thread works is a different i */
  #pragma omp for
  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++)
      a[i][j] = tid + i + j;

  /* For confirmation */
  printf("Thread %d done. Last element= %f\n",tid,a[N-1][N-1]);

  }  /* All threads join master thread and disband */

 return 0;
}

