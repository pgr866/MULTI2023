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
#perf stat list

# Verificar software
make
./Bandera -r 300 -c 400 -o España
eog España.png &
# Para ver las posibles perdidas de memoria:
valgrind ./Bandera -r 300 -c 400

time ./Bandera -r 3072 -c 3072

make Bandera2

time ./Bandera2 -r 3072 -c 3072

# -ddd en lugar de -d da más detalles
perf stat -r 3 -d ./Bandera2 -r 3072 -c 3072
# En máquina virtual se completa con:
valgrind --tool=cachegrind ./Bandera2 -r 3072 -c 3072

make clean
