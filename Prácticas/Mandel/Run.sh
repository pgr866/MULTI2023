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

# Verificar software
make
./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 1000 -g
eog Image.png &
# Para ver las posibles perdidas de memoria:
valgrind ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 1000 -g

# Ejecuciones de prueba
time ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 1000
sleep 2
time ./Mandel -r 1024 -c 1024 -mx -0.586684 -my -0.6612 -sx 0.0488 -sy 0.0366 -mi 1000 -g
sleep 2
time ./Mandel -r 1024 -c 1024 -mx -0.586684 -my -0.6612 -sx 0.0488 -sy 0.0366 -mi 1000
sleep 2
time ./Mandel -r 1024 -c 1024 -mx 0.27085 -my 0.004640 -sx 0.00015 -sy 0.00017 -mi 1000 -g
sleep 2
time ./Mandel -r 1024 -c 1024 -mx 0.27085 -my 0.004640 -sx 0.00015 -sy 0.00017 -mi 1000
sleep 2
time ./Mandel -r 1024 -c 768 -mx 0.278587 -my -0.012560 -sx 0.006826 -sy 0.00512 -mi 1000 -g
sleep 2
time ./Mandel -r 1024 -c 768 -mx 0.278587 -my -0.012560 -sx 0.006826 -sy 0.00512 -mi 1000 

make clean
