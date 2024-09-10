#!/bin/bash

echo "Cambiar 2048 por valor para que el secuencial dure varios segundos"  

make Bandera

echo ""
echo "-----------------------------------------"
echo "Secuencial"
echo "-----------------------------------------"
time ./Bandera -r 1024  -c 1024
time ./Bandera -r 768  -c 1024
time ./Bandera -r 20480 -c 20480


make Bandera-MPI

echo ""
echo "-----------------------------------------"
echo "Bandera-MPI -r 768 -c 1024 -o España"
echo "-----------------------------------------"
mpirun -np 1 ./Bandera-MPI -r 768 -c 1024 -o España

echo ""
echo "-----------------------------------------"
echo "Bandera-MPI -r 768 -c 1024 "
echo "-----------------------------------------"
time mpirun -np 1 ./Bandera-MPI -r 768 -c 1024
time mpirun -np 2 ./Bandera-MPI -r 768 -c 1024
time mpirun -np 4 ./Bandera-MPI -r 768 -c 1024

echo ""
echo "-----------------------------------------"
echo "Bandera-MPI -r 20480 -c 20480"
echo "-----------------------------------------"
time mpirun -np 1 ./Bandera-MPI -r 20480 -c 20480
time mpirun -np 2 ./Bandera-MPI -r 20480 -c 20480
time mpirun -np 4 ./Bandera-MPI -r 20480 -c 20480

make clean
