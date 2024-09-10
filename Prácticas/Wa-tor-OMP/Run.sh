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

#sudo apt -y install gnuplot
#sudo apt -y install gdb
#sudo apt -y install binutils
#sudo apt -y install ffmpeg

#prevent threads migrating between cores
export OMP_PROC_BIND=true

echo "Antes de compilar:"
echo "Secuencial: makefile sin -fopenmp"
echo "Paraleo:    makefile con -fopenmp"

#######
# Verificar software
make
#./Wa-tor-OMP -ni 1000 -ffmpeg

# Para ver las posibles perdidas de memoria:
valgrind -s ./Wa-tor-OMP -ni 50

echo "-----------------------------------------"
echo "-r 100 -c 100 -ni 10000 2 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=2
time ./Wa-tor-OMP -r 102 -c 102 -ni 10000

echo "-----------------------------------------"
echo "-r 100 -c 100 -ni 10000 4 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=4
time ./Wa-tor-OMP -r 102 -c 102 -ni 10000

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 2 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=2
time ./Wa-tor-OMP -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 4 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=4
time ./Wa-tor-OMP -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000

make clean
