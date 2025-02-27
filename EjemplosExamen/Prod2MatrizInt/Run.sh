#!/bin/bash

make

echo "-----------------------------------------"
echo "Prod2MatrizInt serie"
echo "-----------------------------------------"
time ./Prod2MatrizInt -r 15000 -c 15000

make all

echo "-----------------------------------------"
echo "Prod2MatrizInt-PTh 1 T"
echo "-----------------------------------------"
time ./Prod2MatrizInt-PTh -r 15000 -c 15000 -nt 1

echo "-----------------------------------------"
echo "Prod2MatrizInt-PTh 2 T"
echo "-----------------------------------------"
time ./Prod2MatrizInt-PTh -r 15000 -c 15000 -nt 2

echo "-----------------------------------------"
echo "Prod2MatrizInt-PTh 4 T"
echo "-----------------------------------------"
time ./Prod2MatrizInt-PTh -r 15000 -c 15000 -nt 4


echo "-----------------------------------------"
echo "Prod2MatrizInt-MPI 1 T"
echo "-----------------------------------------"
time mpirun -np 1 ./Prod2MatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "Prod2MatrizInt-MPI 2 T"
echo "-----------------------------------------"
time mpirun -np 2 ./Prod2MatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "Prod2MatrizInt-MPI 4 T"
echo "-----------------------------------------"
time mpirun -np 4 ./Prod2MatrizInt-MPI -r 15000 -c 15000

echo "-----------------------------------------"
echo "Prod2MatrizInt-MPI XTERM"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./Prod2MatrizInt-MPI -r 15000 -c 15000

make clean

