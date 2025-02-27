/******************************************************************************
* FILE: omp_bug3.c
* DESCRIPTION:
*   Run time error
* AUTHOR: Blaise Barney  01/09/04
* LAST REVISED: 06/28/05
* Fixed by LGCasado 03/10/23.
*   print_results was called inside a omp section, so only one thread run it
*   calling a barrier in a section is not a good idea. 
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N     50

//------------------------------------------------------
void print_results(float array[N], int tid, int section) 
{
  int j;

  j = 1;
  /*** use critical for clean output ***/
  #pragma omp critical
  {
  printf("\nThread %d did section %d. The results are:\n", tid, section);
  for (int i=0; i<N; i++) {
    printf("%e  ",array[i]);
    j++;
    if (j == 6) {
      printf("\n");
      j = 1;
      }
    }
    printf("\n");
  } /*** end of critical ***/

  //Here I can not put a barrier because only one thread is running.
  printf("Thread %d done and synchronized.\n", tid); 
}

//------------------------------------------------------
int main (int argc, char *argv[]) 
{
 float a[N], b[N], c[N];

/* Some initializations */
for (int i=0; i<N; i++)
  a[i] = b[i] = i * 1.0;

#pragma omp parallel default(none) shared(a,b) private(c)
  {
   int tid; 
   tid = omp_get_thread_num();
   if (tid == 0){
    int nthreads;
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }

  /*** Use barriers for clean output ***/
  #pragma omp barrier
  printf("Thread %d starting...\n",tid);
  #pragma omp barrier

  #pragma omp sections nowait
    {
    #pragma omp section //1: Multiplication
      {
      for (int i=0; i<N; i++)
        c[i] = a[i] * b[i];		// c is private
      print_results(c, tid, 1); 
      }

    #pragma omp section //2: Addition
      {
      for (int i=0; i<N; i++)
        c[i] = a[i] + b[i];		//c is private
      print_results(c, tid, 2);
      }

    }  /* end of sections */

  /*** Use barriers for clean output ***/
  #pragma omp barrier
  printf("Thread %d exiting...\n",tid);

  }  /* end of parallel section */
}

