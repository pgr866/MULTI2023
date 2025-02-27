#!/bin/bash

#Librerias necesarias
#sudo apt -y install netpbm
#sudo apt -y install eog
#sudo apt -y install valgrind
#sudo apt -y install linux-tools-common
#perf
#sudo apt -y install linux-tools-6.2.0-36-generic
#echo "kernel.perf_event_paranoid = -1" | sudo tee -a /etc/sysctl.conf
#echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid
#perf

#sudo apt -y install gnuplot
#sudo apt -y install gdb
#sudo apt -y install binutils
#sudo apt -y install ffmpeg

# Verificar software
make
./Wa-tor -ni 100

echo Primero debemos activar el flag de profiling en el archivo makefile: PROF=-pg
read -p "Presione Enter para continuar..."
make clean && make
# Ejecutar para obtener gmon.out
./Wa-tor -ni 10000
# Generar archivo de profiling
gprof ./Wa-tor gmon.out > Profiling.txt

echo De igual manera que hemos activado el flag -pg en el makefile, ahora debemos desactivarlo: \#PROF=-pg
read -p "Presione Enter para continuar..."
make clean && make
# generar el archivo
valgrind --log-file="MemProf.txt" ./Wa-tor -ni 10000

make clean
