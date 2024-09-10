#!/bin/bash


#Set rows to sequential lasts at least some seconds.
export Rows=500000000

make SumVectorInt

echo "-----------------------------------------"
echo "SumVectorInt serie -r 4"
echo "-----------------------------------------"
time ./SumVectorInt -r 4

make SumVectorInt-OMP

export OMP_NUM_THREADS=4 

echo "-----------------------------------------"
echo "SumVectorInt-OMP -r 4"
echo "-----------------------------------------"
time ./SumVectorInt-OMP -r 4

make SumVectorInt-PTh

echo "-----------------------------------------"
echo "SumVectorInt-PTh -r 4"
echo "-----------------------------------------"
time ./SumVectorInt-PTh -r 4 -nt 4

#======================================================

echo "-----------------------------------------"
echo "SumVectorInt serie -r $Rows"
echo "-----------------------------------------"
time ./SumVectorInt -r $Rows

echo "-----------------------------------------"
echo "SumVectorInt-OMP -r $Rows"
echo "-----------------------------------------"
export OMP_NUM_THREADS=4 
time ./SumVectorInt-OMP -r $Rows

echo "-----------------------------------------"
echo "SumVectorInt-PTh -r $Rows"
echo "-----------------------------------------"
time ./SumVectorInt-PTh -r $Rows -nt 4
