#!/bin/bash

make SumVectorInt

echo "-----------------------------------------"
echo "SumVectorInt"
echo "-----------------------------------------"
time ./SumVectorInt -r 5 -c 5


make SumVectorInt-OMP

export OMP_NUM_THREADS=4 

echo "-----------------------------------------"
echo "SumVectorInt-OMP"
echo "-----------------------------------------"
time ./SumVectorInt-OMP -r 5 -c 5

make SumVectorInt-PTh

echo "-----------------------------------------"
echo "SumVectorInt-PTh -r 4"
echo "-----------------------------------------"
time ./SumVectorInt-PTh -r 5 -c 5 -nt 4
