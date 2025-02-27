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

#######
# Verificar software
make clean && make all
#./Wa-tor -ni 1000 --ffmpeg
#./Wa-tor -ni 1000 -d

# Para ver las posibles perdidas de memoria:s
valgrind -s ./Wa-tor-PTh -ni 102 -nt 4

# Sequential
echo "-----------------------------------------"
echo "Wa-tor -r 201 -c 201 -ni 10000 serie"
echo "-----------------------------------------"
time ./Wa-tor -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000

# Parallel
echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 1 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000 -nt 1

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 2 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000 -nt 2

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 4 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000 -nt 4

# Sequential
echo "-----------------------------------------"
echo "Wa-tor -r 300 -c 300 -ni 10000 serie"
echo "-----------------------------------------"
time ./Wa-tor -r 300 -c 300 -nf 20000 -ns 7000 -ni 10000

# Parallel
echo "-----------------------------------------"
echo "-r 300 -c 300 -ni 10000 1 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 300 -c 300 -nf 20000 -ns 7000 -ni 10000 -nt 1

echo "-----------------------------------------"
echo "-r 300 -c 300 -ni 10000 2 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 300 -c 300 -nf 20000 -ns 7000 -ni 10000 -nt 2

echo "-----------------------------------------"
echo "-r 300 -c 300 -ni 10000 4 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 300 -c 300 -nf 20000 -ns 7000 -ni 10000 -nt 4
