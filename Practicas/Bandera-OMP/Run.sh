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
#set the number of threads
export OMP_NUM_THREADS=4

echo "Antes de compilar:"
echo "Secuencial: makefile sin -fopenmp"
echo "Paraleo:    makefile con -fopenmp"

#######
# Verificar software
make
#./Bandera-OMP -r 1024 -c 1024 -o España
#eog España.png &
#./Bandera-OMP -r 20480 -c 20480 -o España

time ./Bandera-OMP -r 1024 -c 1024
time ./Bandera-OMP -r 20480 -c 20480

#perf stat -r 3 -ddd ./Bandera-OMP -r 1024 -c 1024
#perf stat -r 3 -ddd ./Bandera-OMP -r 20480 -c 20480

make clean
