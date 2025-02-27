export OMP_STACKSIZE=12000000
export OMP_PROC_BIND=true
export OMP_NUM_THREADS=4

#To compile a program.c  $ make program
#For example:
make 01-omp_hello

#Run the program. 
./01-omp_hello
