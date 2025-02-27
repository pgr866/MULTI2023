#!/bin/bash

make Mandel

echo "-----------------------------------------"
echo "Mandel -mi 1e4 1 serie"
echo "-----------------------------------------"
time ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000

echo "-----------------------------------------"
echo "Mandel -mi 1e5 1 serie"
echo "-----------------------------------------"
time ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000

make Mandel-MPI

echo "-----------------------------------------"
echo "-mi 1e4 1 Task"
echo "-----------------------------------------"
time mpirun -np 1 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000

echo "-----------------------------------------"
echo "-mi 1e4 2 Task"
echo "-----------------------------------------"
time mpirun -np 2 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000

echo "-----------------------------------------"
echo "-mi 1e4 4 Task"
echo "-----------------------------------------"
time mpirun -np 4 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000


echo "-----------------------------------------"
echo "-mi 1e5 1 Task"
echo "-----------------------------------------"
time mpirun -np 1 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000

echo "-----------------------------------------"
echo "-mi 1e5 2 Task"
echo "-----------------------------------------"
time mpirun -np 2 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000

echo "-----------------------------------------"
echo "-mi 1e5 4 Task"
echo "-----------------------------------------"
time mpirun -np 4 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000

make clean

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#Ejemplo para ejecutar cada tarea en un terminal distinto:
#> mpirun -np 4 xterm -fa 'Monospace' -fs 10 -hold -e ./Mandel-MPI -r 10 -c 10 -mx -2 -my -2 -sx 4 -sy 4 -mi 100 -o 

#Ejemplo para depurar la ejecución de cada tarea con gdb en un terminal distinto. hay que compilar con -g
# #> mpirun -np 4 xterm -fa 'Monospace' -fs 10 -hold -e gdb ./Mandel-MPI
# En cada terminal ejecutar 
#(gdb) run  -r 10 -c 10 -mx -2 -my -2 -sx 4 -sy 4 -mi 100 -o
#(gdb) quit



