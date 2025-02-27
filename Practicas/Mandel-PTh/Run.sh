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

# Compila Mandel
make Mandel

echo "-----------------------------------------"
echo "Mandel -mi 10000 1 serie"
echo "-----------------------------------------"
time ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -o 

echo "-----------------------------------------"
echo "Mandel -mi 100000 1 serie"
echo "-----------------------------------------"
time ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -o 

# Compila Mandel-PTh
make Mandel-PTh

echo "-----------------------------------------"
echo "-mi 10000 1 Thread, -cs 1"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -cs 1 -nt 1 -o

echo "-----------------------------------------"
echo "-mi 10000 2 Thread, -cs 1"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -cs 1 -nt 2 -o

echo "-----------------------------------------"
echo "-mi 10000 4 Thread, -cs 1"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -cs 1 -nt 4 -o


echo "-----------------------------------------"
echo "-mi 100000 1 Thread, -cs 1"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -cs 1 -nt 1 -o

echo "-----------------------------------------"
echo "-mi 100000 2 Thread, -cs 1"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -c 1 -nt 2 -o

echo "-----------------------------------------"
echo "-mi 100000 4 Thread, -cs 1"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -cs 1 -nt 4 -o


echo "-----------------------------------------"
echo "-mi 10000 1 Thread, chunk = 1024/1 = 1024"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -cs 1024 -nt 1 -o

echo "-----------------------------------------"
echo "-mi 10000 2 Thread, chunk = 1024/2 = 512"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -c 512 -nt 2 -o

echo "-----------------------------------------"
echo "-mi 10000 4 Thread, chunk = 1024/4 = 256"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -cs 256 -nt 4 -o


echo "-----------------------------------------"
echo "-mi 100000 1 Thread, chunk = 1024/1 = 1024"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -cs 1024 -nt 1 -o

echo "-----------------------------------------"
echo "-mi 100000 2 Thread, chunk = 1024/2 = 512"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -c 512 -nt 2 -o

echo "-----------------------------------------"
echo "-mi 100000 4 Thread, chunk = 1024/4 = 256"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -cs 256 -nt 4 -o


echo "-----------------------------------------"
echo "-mi 100000 2 Thread, mejor chunk = 32"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -c 32 -nt 2 -o

echo "-----------------------------------------"
echo "-mi 100000 4 Thread, mejor chunk = 6"
echo "-----------------------------------------"
time ./Mandel-PTh -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -cs 6 -nt 4 -o

make clean
