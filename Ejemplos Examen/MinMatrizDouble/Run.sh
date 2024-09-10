#!/bin/bash

make

echo "-----------------------------------------"
echo "MinMatrizDouble serie"
echo "-----------------------------------------"
time ./MinMatrizDouble -r 10000 -c 10000

make all

echo "-----------------------------------------"
echo "MinMatrizDouble-PTh 1 Thread"
echo "-----------------------------------------"
time ./MinMatrizDouble-PTh -r 10000 -c 10000 -nt 1

echo "-----------------------------------------"
echo "MinMatrizDouble-PTh 2 Thread"
echo "-----------------------------------------"
time ./MinMatrizDouble-PTh -r 10000 -c 10000 -nt 2

echo "-----------------------------------------"
echo "MinMatrizDouble-PTh 4 Thread"
echo "-----------------------------------------"
time ./MinMatrizDouble-PTh -r 10000 -c 10000 -nt 4


echo "-----------------------------------------"
echo "MinMatrizDouble-PTh 1 Thread"
echo "-----------------------------------------"
time mpirun -np 1 ./MinMatrizDouble-MPI -r 10000 -c 10000

echo "-----------------------------------------"
echo "MinMatrizDouble-PTh 2 Thread"
echo "-----------------------------------------"
time mpirun -np 2 ./MinMatrizDouble-MPI -r 10000 -c 10000

echo "-----------------------------------------"
echo "MinMatrizDouble-PTh 4 Thread"
echo "-----------------------------------------"
time mpirun -np 4 ./MinMatrizDouble-MPI -r 10000 -c 10000

echo "-----------------------------------------"
echo "MinMatrizDouble-PTh 4 XTERM"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./MinMatrizDouble-MPI -r 10000 -c 10000

make clean

