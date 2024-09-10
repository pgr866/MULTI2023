#!/bin/bash

make

echo "-----------------------------------------"
echo "SumMatrizInt serie"
echo "-----------------------------------------"
time ./SumMatrizInt -r 15000 -c 15000

make all

echo "-----------------------------------------"
echo "SumMatrizInt-PTh 1 T"
echo "-----------------------------------------"
time ./SumMatrizInt-PTh -r 15000 -c 15000 -nt 1

echo "-----------------------------------------"
echo "SumMatrizInt-PTh 2 T"
echo "-----------------------------------------"
time ./SumMatrizInt-PTh -r 15000 -c 15000 -nt 2

echo "-----------------------------------------"
echo "SumMatrizInt-PTh 4 T"
echo "-----------------------------------------"
time ./SumMatrizInt-PTh -r 15000 -c 15000 -nt 4


echo "-----------------------------------------"
echo "SumMatrizInt-MPI 1 T"
echo "-----------------------------------------"
time mpirun -np 1 ./SumMatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "SumMatrizInt-MPI 2 T"
echo "-----------------------------------------"
time mpirun -np 2 ./SumMatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "SumMatrizInt-MPI 4 T"
echo "-----------------------------------------"
time mpirun -np 4 ./SumMatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "SumMatrizInt-MPI 4 T"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./SumMatrizInt-MPI -r 15000 -c 15000

make clean

