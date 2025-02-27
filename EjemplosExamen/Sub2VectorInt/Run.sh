#!/bin/bash

make

echo "-----------------------------------------"
echo "Sub2VectorInt serie"
echo "-----------------------------------------"
time ./Sub2VectorInt -r 100000000

make all

echo "-----------------------------------------"
echo "Sub2VectorInt-PTh pararell 1 Threads"
echo "-----------------------------------------"
time ./Sub2VectorInt-PTh -r 100000000 -nt 1

echo "-----------------------------------------"
echo "Sub2VectorInt-PTh pararell 2 Threads"
echo "-----------------------------------------"
time ./Sub2VectorInt-PTh -r 100000000 -nt 2

echo "-----------------------------------------"
echo "Sub2VectorInt-PTh pararell 4 Threads"
echo "-----------------------------------------"
time ./Sub2VectorInt-PTh -r 100000000 -nt 4


echo "-----------------------------------------"
echo "Sub2VectorInt-MPI pararell 1 Tasks"
echo "-----------------------------------------"
time mpirun -np 1 ./Sub2VectorInt-MPI -r 100000000

echo "-----------------------------------------"
echo "Sub2VectorInt-MPI pararell 2 Tasks"
echo "-----------------------------------------"
time mpirun -np 2 ./Sub2VectorInt-MPI -r 100000000

echo "-----------------------------------------"
echo "Sub2VectorInt-MPI pararell 4 Tasks"
echo "-----------------------------------------"
time mpirun -np 4 ./Sub2VectorInt-MPI -r 100000000

echo "-----------------------------------------"
echo "Sub2VectorInt-MPI pararell XTERM 4 Tasks"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Sub2VectorInt-MPI -r 100000000

make clean

