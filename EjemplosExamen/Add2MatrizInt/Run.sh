#!/bin/bash

make

echo "-----------------------------------------"
echo "Add2MatrizInt serie"
echo "-----------------------------------------"
time ./Add2MatrizInt -r 15000 -c 15000

make all

echo "-----------------------------------------"
echo "Add2MatrizInt-PTh 1 T"
echo "-----------------------------------------"
time ./Add2MatrizInt-PTh -r 15000 -c 15000 -nt 1

echo "-----------------------------------------"
echo "AddMatrizInt-PTh 2 T"
echo "-----------------------------------------"
time ./Add2MatrizInt-PTh -r 15000 -c 15000 -nt 2

echo "-----------------------------------------"
echo "Add2MatrizInt-PTh 2 T"
echo "-----------------------------------------"
time ./Add2MatrizInt-PTh -r 15000 -c 15000 -nt 2



echo "-----------------------------------------"
echo "Add2MatrizInt-MPI 1 T"
echo "-----------------------------------------"
time mpirun -np 1 ./Add2MatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "Add2MatrizInt-MPI 2 T"
echo "-----------------------------------------"
time mpirun -np 2 ./Add2MatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "Add2MatrizInt-MPI 4 T"
echo "-----------------------------------------"
time mpirun -np 4 ./Add2MatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "Add2MatrizInt-MPI XTERM"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Add2MatrizInt-MPI -r 15000 -c 15000

