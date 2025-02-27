#!/bin/bash

make SumVectorInt

echo "-----------------------------------------"
echo "SumVectorInt serie"
echo "-----------------------------------------"
time ./SumVectorInt -r 4

make SumVectorInt-OMP

export OMP_NUM_THREADS=4 

echo "-----------------------------------------"
echo "SumVectorInt-OMP"
echo "-----------------------------------------"
time ./SumVectorInt-OMP -r 4

make SumVectorInt-PTh

echo "-----------------------------------------"
echo "SumVectorInt-PTh"
echo "-----------------------------------------"
time ./SumVectorInt-PTh -r 4 -nt 4
