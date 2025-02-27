#!/bin/bash

make all

echo "-----------------------------------------"
echo "ProdVectorInt serie"
echo "-----------------------------------------"
time ./ProdVectorInt -r 10

echo "-----------------------------------------"
echo "ProdVectorInt MPI"
echo "-----------------------------------------"
time mpirun -np 4 ./ProdVectorInt-MPI -r 10


echo "-----------------------------------------"
echo "ProdVectorInt MPI en consolas"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./ProdVectorInt-MPI -r 10
