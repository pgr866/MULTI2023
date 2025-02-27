#!/bin/bash

make

echo "-----------------------------------------"
echo "AddMinMatrizDouble serie"
echo "-----------------------------------------"
time ./AddMinMatrizDouble -r 12000 -c 12000

make all

echo "-----------------------------------------"
echo "AddMinMatrizDouble-PTh 1 T"
echo "-----------------------------------------"
time ./AddMinMatrizDouble-PTh -r 12000 -c 12000 -nt 1

echo "-----------------------------------------"
echo "AddMinMatrizDouble-PTh 2 T"
echo "-----------------------------------------"
time ./AddMinMatrizDouble-PTh -r 12000 -c 12000 -nt 2

echo "-----------------------------------------"
echo "AddMinMatrizDouble-PTh 4 T"
echo "-----------------------------------------"
time ./AddMinMatrizDouble-PTh -r 12000 -c 12000 -nt 4


echo "-----------------------------------------"
echo "AddMinMatrizDouble-MPI 1 T"
echo "-----------------------------------------"
time mpirun -np 1 ./AddMinMatrizDouble-MPI -r 12000 -c 12000

echo "-----------------------------------------"
echo "AddMinMatrizDouble-MPI 2 T"
echo "-----------------------------------------"
time mpirun -np 2 ./AddMinMatrizDouble-MPI -r 12000 -c 12000

echo "-----------------------------------------"
echo "AddMinMatrizDouble-MPI 4 T"
echo "-----------------------------------------"
time mpirun -np 4 ./AddMinMatrizDouble-MPI -r 12000 -c 12000

echo "-----------------------------------------"
echo "AddMinMatrizDouble-MPI 4 T XTERM"
echo "-----------------------------------------"
#ime mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./AddMinMatrizDouble-MPI -r 12000 -c 12000

make clean

