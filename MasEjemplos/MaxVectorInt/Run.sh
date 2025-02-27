#!/bin/bash

make all

echo "-----------------------------------------"
echo "MaxVectorInt serie"
echo "-----------------------------------------"
time ./MaxVectorInt -r 10

echo "-----------------------------------------"
echo "MaxVectorInt MPI"
echo "-----------------------------------------"
time mpirun -np 4 ./MaxVectorInt-MPI -r 10


echo "-----------------------------------------"
echo "MaxVectorInt MPI en consolas"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./MaxVectorInt-MPI -r 10
