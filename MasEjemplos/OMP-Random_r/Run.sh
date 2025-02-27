#!/bin/bash
make 
export OMP_NUM_THREADS=4 
time ./SumVectorInt-OMP_R -r 500000000
time ./SumVectorInt-OMP_R_Vector -r 500000000

