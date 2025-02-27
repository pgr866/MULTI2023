
Ejemplo de ejecución en una consola por tarea para depuración:
```console
$ mpirun -np 4 --oversubscribe xterm -fa 'Monospace' -fs 14 -hold -e Add2VectorInt-MPI -r 10
```

## Programas: 
1. Add2VectorInt.c: programa secuencial que suma dos vectores de enteros aleatorios de igual tamaño en otro vector de enteros.
2. Add2VectorInt-MPI.c: Los vectores los inicializa la tarea 0 y la suma se hace en paralelo.
3. Add2VectorInt-MPI-InPlace.c: Igual que Add2VectorInt-MPI, pero la hebra 0 hace las operaciones de suma IN_PLACE, por lo que no necesita coger memoria para los subvectores.
4. Add2VectorInt-MPI-ParInit.c: Los vectores los inicializan cada tarea y la suma se hace en paralelo.
5. Add2VectorInt-MPI-ParInit-InPlace.c: Los vectores los inicializan cada tarea y la suma se hace en paralelo, pero la tarea 0 hace la suma en pVector3 IN_PLACE.

## Ejecuciones:

```console
$ time Add2VectorInt -r 500000000 
Rows=500000000.
Tiempo de inicializar los vectores         =         6.31 sec
Tiempo de sumar los vectores               =       0.8198 sec
Tiempo de inicializar y sumar los vectores =         7.13 sec

real	0m7,352s
user	0m5,857s
sys	0m1,485s
```

```console
$ time mpirun -np 4 Add2VectorInt-MPI -r 500000000 
Rows=500000000.

real	0m9,314s
user	0m32,614s
sys	0m4,521s
```
**Tarda más en enviar los vectores y recibirlos que en procesarlos en secuencial.**

```console
$ time mpirun -np 4 Add2VectorInt-MPI-InPlace -r 500000000 
Rows=500000000.

real	0m8,633s
user	0m30,438s
sys	0m3,993s
```

**Se ahorra un envío, pero sigue siendo más lento que el secuencial.**

```console
$ time mpirun -np 4 Add2VectorInt-MPI-ParInit -r 500000000 
Rows=500000000.

real	0m2,945s
user	0m9,432s
sys	0m2,274s
```
**Paralelizar el inicio de los vectores si mejora los tiempos.**

```console
$ time mpirun -np 4 Add2VectorInt-MPI-ParInit-InPlace -r 500000000 
Rows=500000000.

real	0m2,648s
user	0m8,359s
sys	0m2,167s
```
**Además, al hacerlo IN_PLACE nos ahorramos un envío.**
