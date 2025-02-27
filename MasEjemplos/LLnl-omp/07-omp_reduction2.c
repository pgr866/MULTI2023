/******************************************************************************
* FILE: omp_reduction.c
* DESCRIPTION:
*   OpenMP Example - Combined Parallel Loop Reduction - C/C++ Version
*   This example demonstrates a sum reduction within a combined parallel loop
*   construct.  Notice that default data element scoping is assumed - there
*   are no clauses specifying shared or private variables.  OpenMP will 
*   automatically make loop index variables private within team threads, and
*   global variables shared.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main ()  {

int   i, n, chunk;
float a[100], b[100], result;

/* Some initializations */
n = 100;
chunk = 10;
result = 0.0;
for (i=0; i < n; i++)
  a[i] = b[i] = i * 1.0;

#pragma omp parallel for \
  default(shared) private(i) \
  schedule(static,chunk) \
  reduction(+:result)

  for (i=0; i < n; i++)
    result = result + (a[i] * b[i]);

printf("Final result= %f\n",result);

return 0;
}
