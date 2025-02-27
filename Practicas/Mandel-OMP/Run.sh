#!/bin/bash

#Librerias necesarias
#sudo apt -y install netpbm
#sudo apt -y install eog
#sudo apt -y install valgrind
#sudo apt -y install linux-tools-common
#perf
#sudo apt -y install linux-tools-6.2.0-36-generic
#echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid
#perf

#prevent threads migrating between cores
export OMP_PROC_BIND=true

echo "Antes de compilar:"
echo "Secuencial: makefile sin -fopenmp"
echo "Paraleo:    makefile con -fopenmp"

#######
# Compilar
make

echo "-----------------------------------------"
echo "-mi 1e4 2 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=2
time ./Mandel-OMP -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -o

echo "-----------------------------------------"
echo "-mi 1e4 4 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=4
time ./Mandel-OMP -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -o


echo "-----------------------------------------"
echo "-mi 1e5 2 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=2
time ./Mandel-OMP -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -o

echo "-----------------------------------------"
echo "-mi 1e5 4 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=4
time ./Mandel-OMP -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -o

make clean
