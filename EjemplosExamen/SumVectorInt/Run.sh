#!/bin/bash

make

echo "-----------------------------------------"
echo "SumVectorInt serie -r 300000000"
echo "-----------------------------------------"
time ./SumVectorInt -r 300000000

make all

echo "-----------------------------------------"
echo "SumVectorInt-PTh 1 Thread"
echo "-----------------------------------------"
time ./SumVectorInt-PTh -r 300000000 -nt 1

echo "-----------------------------------------"
echo "SumVectorInt-PTh 2 Thread"
echo "-----------------------------------------"
time ./SumVectorInt-PTh -r 300000000 -nt 2

echo "-----------------------------------------"
echo "SumVectorInt-PTh 4 Thread"
echo "-----------------------------------------"
time ./SumVectorInt-PTh -r 300000000 -nt 4

echo "-----------------------------------------"
echo "SumVectorInt-MPI -r 300000000 Pararell 1"
echo "-----------------------------------------"
time mpirun -np 1 ./Add2VectorInt-MPI -r 300000000

echo "-----------------------------------------"
echo "SumVectorInt-MPI -r 300000000 Pararell 2"
echo "-----------------------------------------"
time mpirun -np 2 ./Add2VectorInt-MPI -r 300000000

echo "-----------------------------------------"
echo "SumVectorInt-MPI -r 300000000 Pararell 4"
echo "-----------------------------------------"
time mpirun -np 4 ./Add2VectorInt-MPI -r 300000000

echo "-----------------------------------------"
echo "SumVectorInt-MPI -r 300000000 XTERM 4"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Add2VectorInt-MPI -r 300000000

make clean
