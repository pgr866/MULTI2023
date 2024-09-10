#include <stdio.h>
 #include <stdlib.h>
#include <omp.h>

int main()
{

int x,nthreads;
x = 0;

#pragma omp parallel master
{
 nthreads = omp_get_num_threads();
 printf("Number of threads = %d\n", nthreads);
}

#pragma omp parallel shared(x) 
  {

  #pragma omp critical 
  x = x + 1;

  }  /* end of parallel section */

 printf("x=%d.\n",x);
 return 0;
}

