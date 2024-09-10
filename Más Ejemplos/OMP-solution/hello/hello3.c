#include <stdio.h>
#include <unistd.h>

# ifdef _OPENMP
#  include <omp.h>
# endif

int main(int argc, char** argv)
{
  int i;

    i = -1;

#pragma omp parallel
  {
# ifdef _OPENMP
    i = omp_get_thread_num();
# endif

    printf( "hello world %d\n", i );
  }

  return 0;
}
