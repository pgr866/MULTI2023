echo |cpp -fopenmp -dM |grep -i open
gcc -fopenmp helloworld.c -o helloworld
./helloworld
env OMP_NUM_THREADS=4 ./helloworld
# OPENMP CUANDO TENGAMOS MEMORIA COMPARTIDA
# hilo 0 es el principal, siempre esta
# Schedule static: reparte al principio por tamaño entre las cajas
# Schedule dynamic: reparte dinámicamente, cuando uno acaba se le asigna la siguiente tarea.
# dentro de secciones paralelizadas:
#pragma omp master antes de bloque es como single, pero que lo haga el hilo principal.
#pragma omp critical para que solo se ejecute por un hilo a la vez. atomic como critical
#pragma omp barrier punto de sincronización o espera

#pragma omp parallel private(nthreads)