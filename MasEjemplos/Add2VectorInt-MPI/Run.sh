#!/bin/bash

make all

echo "-----------------------------------------"
echo "Add2VectorInt serie"
echo "-----------------------------------------"
time ./Add2VectorInt -r 10

echo "-----------------------------------------"
echo "Add2VectorInt MPI"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Add2VectorInt-MPI -r 10

echo "-----------------------------------------"
echo "Add2VectorInt MPI-InPlace"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Add2VectorInt-MPI-InPlace -r 10

echo "-----------------------------------------"
echo "Add2VectorInt MPI Init in parallel"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Add2VectorInt-MPI-ParInit -r 10

echo "-----------------------------------------"
echo "Add2VectorInt MPI Init in parallel an InPlace"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Add2VectorInt-MPI-ParInit-InPlace -r 10

