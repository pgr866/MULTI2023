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

# Compilar Bandera y Bandera-PTh
make all

#Serie=========================================

echo "-----------------------------------------"
echo "Serie: -r 1024 -c 1024 "
echo "-----------------------------------------"
time ./Bandera -r 1024 -c 1024
#eog España.png &

echo "-----------------------------------------"
echo "Serie: -r 20480 -c 20480"
echo "-----------------------------------------"
time ./Bandera -r 20480 -c 20480


#PTh============================================

echo "-----------------------------------------"
echo "PTh: -r 1024 -c 1024 "
echo "-----------------------------------------"
time ./Bandera-PTh -r 1024 -c 1024 -nt 1
time ./Bandera-PTh -r 1024 -c 1024 -nt 2
time ./Bandera-PTh -r 1024 -c 1024 -nt 4

echo "-----------------------------------------"
echo "PTh: -r 20480 -c 20480"
echo "-----------------------------------------"
time ./Bandera-PTh -r 20480 -c 20480 -nt 1 
time ./Bandera-PTh -r 20480 -c 20480 -nt 2
time ./Bandera-PTh -r 20480 -c 20480 -nt 4

make clean
