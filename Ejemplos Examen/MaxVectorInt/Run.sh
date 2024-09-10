#!/bin/bash

make

echo "-----------------------------------------"
echo "MaxVectorInt serie"
echo "-----------------------------------------"
time ./MaxVectorInt -r 500000000

make all

echo "-----------------------------------------"
echo "MaxVectorInt-PTh 1 Thread"
echo "-----------------------------------------"
time ./MaxVectorInt-PTh -r 500000000 -nt 1

echo "-----------------------------------------"
echo "MaxVectorInt-PTh 2 Thread"
echo "-----------------------------------------"
time ./MaxVectorInt-PTh -r 500000000 -nt 2

echo "-----------------------------------------"
echo "MaxVectorInt-PTh 4 Thread"
echo "-----------------------------------------"
time ./MaxVectorInt-PTh -r 500000000 -nt 4


echo "-----------------------------------------"
echo "MaxVectorInt MPI 1 Threads"
echo "-----------------------------------------"
time mpirun -np 1 ./MaxVectorInt-MPI -r 500000000

echo "-----------------------------------------"
echo "MaxVectorInt MPI 2 Threads"
echo "-----------------------------------------"
time mpirun -np 2 ./MaxVectorInt-MPI -r 500000000

echo "-----------------------------------------"
echo "MaxVectorInt MPI 4 Threads"
echo "-----------------------------------------"
time mpirun -np 4 ./MaxVectorInt-MPI -r 500000000

echo "-----------------------------------------"
echo "MaxVectorInt MPI en consolas"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./MaxVectorInt-MPI -r 50000

make clean

