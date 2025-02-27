#!/bin/bash

make

echo "-----------------------------------------"
echo "Add2VectorInt serie"
echo "-----------------------------------------"
time ./Add2VectorInt -r 10

make all

echo "-----------------------------------------"
echo "Add2VectorInt-PTh 1 Threads"
echo "-----------------------------------------"
time ./Add2VectorInt-PTh -r 10 -nt 1

echo "-----------------------------------------"
echo "Add2VectorInt-PTh 2 Threads"
echo "-----------------------------------------"
time ./Add2VectorInt-PTh -r 10 -nt 2

echo "-----------------------------------------"
echo "Add2VectorInt-PTh 4 Threads"
echo "-----------------------------------------"
time ./Add2VectorInt-PTh -r 10 -nt 4


echo "-----------------------------------------"
echo "Add2VectorInt-MPI 1 Tasks"
echo "-----------------------------------------"
time mpirun -np 1 ./Add2VectorInt-MPI -r 10

echo "-----------------------------------------"
echo "Add2VectorInt-MPI 2 Tasks"
echo "-----------------------------------------"
time mpirun -np 2 ./Add2VectorInt-MPI -r 10

echo "-----------------------------------------"
echo "Add2VectorInt-MPI 4 Tasks"
echo "-----------------------------------------"
time mpirun -np 4 ./Add2VectorInt-MPI -r 10

echo "-----------------------------------------"
echo "Add2VectorInt-MPI XTERM 4 Tasks"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Add2VectorInt-MPI -r 10

make clean

