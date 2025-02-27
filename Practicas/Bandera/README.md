# Actividad: Bandera serie.

# A rellenar por el alumno/grupo
 * Nombre y apellidos alumno 1   : Pablo Gómez Rivas
 * Nombre y apellidos alumno 2   : Jesús Fornieles Muñoz
 * Mayoría en GTA1, GTA2 o GTA3  : GTA1
 * Nombre de grupo de actividades: Grupo Actividades 8


# Descripción de la arquitectura utilizada:
Ver la información del micro en Internet. Otra información se puede ver con los comandos:
  * $ free -h}
  * $ lscpu  (versión reducida de $less /proc/cpuinfo)
  * $ lstopo

A la BIOS se accede justo después de arrancar pulsando ESC, F1 o F2.

## Arquitectura: 
  * Microprocesador: AMD Ryzen 7 3750H with Radeon Vega Mobile Gfx
  * Número de núcleos: 4
  * Cantidad de subprocesos por nucleo: 2
  * Tiene hyperthreading (SMT) activado en BIOS: Sí
  * HDD/SDD: 476,92 GB SSD
  * RAM: 16 GB
  * Se usa máquina virtual: Sí
    - Número de cores: 6
    - RAM: 8 GB
    - Capacidad HDD: 50 GB

## Instrucciones:
 * El ejemplo muestra como generar una imagen RGB de la bandera de España.
 * Los parámetros de anchura y altura se introducen por la línea de comandos.
 * La memoria se asigna de forma dinámica. Aunque se podría escribir directamente en el fichero de salida, se usan tres matrices (R, G y B) para generar los colores RGB de cada pixel.

## Librerias
Se necesita tener instalados los siguientes paquetes:
 * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
 * eog para visualizar la imagen.
 * valgrind para ver las posibles perdidas de memoria.
 * perf para medir el rendimiento del programa.

## Objetivos
 * Practicar los conocimientos de C.
 * Familiarizar al alumno con 
    - El uso de varios archivos fuente. 
    - El uso de makefile y el comando make.
  * Uso de memoria dinámica. Ver getmem.h y getmem.c
  * Gestión de parámetros de entrada. Ver argshand.h y argshand.c
  * Uso de script de ejemplo para compilación y ejecución. Ver Run.sh
  * Ejemplo secuencial donde el trabajo computacional se sabe de antemano.
  * Observar que el visor de imágenes eog actualiza la salida si se cambia la imagen.
  * Chequear memory leaks.
  * Mejorar el rendimiento del programa

## Compilación

```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Bandera -h
```

## Ejemplo de compilacion y ejecución
 * En el script Run.sh

 * El script tiene dos ejecuciones. Para ejecutar solo una, hay que copiarla en el línea de comandos del shell. Normalmente se usa la línea de comandos, no el Run.sh. El Run.sh es para recordar los comandos a ejecutar y sus parámetros.

- - - 

# Entrega: 

## Parte 1: Verificar el Software.

Ejecutar en la consola:
```console 
$ make 
$ ./Bandera -r 300 -c 400 -o España
$ ./eog España.png
```
* Se debería ver la bandera de España.

Hay que evaluar si hay alguna perdida de memoria. 
* Ejecutar en la consola:
```console 
$ valgrind ./Bandera -r 300 -c 400
```
1. **Muestra aquí la salida de pantalla:**

```console
 user@Ubuntu:~$ sudo su
[sudo] password for user: 
root@Ubuntu:/home/user# cd /home/user/Desktop/shared_folder/Bandera
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# make
gcc  -Wall -c -o argshand.o argshand.c
gcc  -Wall -c -o getmem.o getmem.c
gcc  -Wall -c -o utils.o utils.c
gcc  -Wall -I.  argshand.o getmem.o utils.o\
 -L/usr/lib64 -lm -o Bandera Bandera.c
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# ./Bandera -r 300 -c 400 -o España
pnmtopng: 2 colors found
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# eog España.png
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# valgrind ./Bandera -r 300 -c 400
==41173== Memcheck, a memory error detector
==41173== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==41173== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==41173== Command: ./Bandera -r 300 -c 400
==41173== 
==41173== 
==41173== HEAP SUMMARY:
==41173==     in use at exit: 122,400 bytes in 301 blocks
==41173==   total heap usage: 903 allocs, 602 frees, 367,200 bytes allocated
==41173== 
==41173== LEAK SUMMARY:
==41173==    definitely lost: 2,400 bytes in 1 blocks
==41173==    indirectly lost: 120,000 bytes in 300 blocks
==41173==      possibly lost: 0 bytes in 0 blocks
==41173==    still reachable: 0 bytes in 0 blocks
==41173==         suppressed: 0 bytes in 0 blocks
==41173== Rerun with --leak-check=full to see details of leaked memory
==41173== 
==41173== For lists of detected and suppressed errors, rerun with: -s
==41173== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# 
```

2. **¿Hay perdida de memoria?**

Sí.

3. **¿Qué has tenido que corregir en Bandera.c para que no haya perdida de memoria**. 

He tenido que descomentar la línea que liberaba el puntero a la matriz con el color azul.

* Hay que entregar el Bandera.c corregido. 

4. **Muestra aquí la salida de pantalla del comando valgrind sobre Bandera sin perdida de memoria.**

```console
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# make
gcc  -Wall -I.  argshand.o getmem.o utils.o\
 -L/usr/lib64 -lm -o Bandera Bandera.c
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# ./Bandera -r 300 -c 400 -o España
pnmtopng: 2 colors found
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# eog España.png
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# valgrind ./Bandera -r 300 -c 400
==41250== Memcheck, a memory error detector
==41250== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==41250== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==41250== Command: ./Bandera -r 300 -c 400
==41250== 
==41250== 
==41250== HEAP SUMMARY:
==41250==     in use at exit: 0 bytes in 0 blocks
==41250==   total heap usage: 903 allocs, 903 frees, 367,200 bytes allocated
==41250== 
==41250== All heap blocks were freed -- no leaks are possible
==41250== 
==41250== For lists of detected and suppressed errors, rerun with: -s
==41250== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# 
```

- - - 
## Parte 2: Mejorar el rendimiento.

**Copia Bandera.c en Bandera2.c y en vez de usar tres bucles dobles: dos para el rojo y uno para el amarillo; hay que usar un solo bucle doble para establecer el color de cada pixel.** 

**Nota**: El makefile está ya preparado para poder compilar  Bandera2 con `$ make Bandera2`. Hay que verificar que Bandera2 funciona correctamente viendo si genera bien la imagen. 

**Nota**: El amarillo se compone de rojo y verde. Todos los pixeles tienen rojo. Solo los de la banda central tienen verde.

### Comparar el rendimiento de Bandera y de Bandera2:

**Nota**: Hay que elegir un número de filas y columnas (Rows=Cols) múltiplo de 1024 que haga que el programa tarde varios segundos y que no consuma toda la RAM.

**Nota**: cada pixel necesita tres bytes, uno para cada color.

**Nota**: 1 kB =1024 bytes.

6. **¿Que valor de Rows=Cols has elegido? ¿Cuanta memoria coge el programa con GetMem para imagen?** 

Para Rows=Cols=30720:

Tamaño de la imagen en memoria = Rows x Cols x 3 bytes = 30720 x 30720 x 3 bytes = 2831155200 bytes = **2764800 KB**

7. **Mostrar el menor tiempo real de varias ejecuciones de la salida de:**
```console 
& time ./Bandera -r Rows -c Cols
```

real	0m8,009s

8. **¿Que algoritmo tiene menor tiempo de ejecución, Bandera o Bandera2?**

El menor tiempo real de varias ejecuciones de la salida del comando

```console
time ./Bandera2 -r 30720 -c 30720
```

es  real	0m8,310s , por lo que es más tiempo que el de Bandera.

**Ejecutar el comando:**
```console 
& perf stat -r 3 -d ./Bandera2 -r Rows -c Cols
```

```console
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# perf stat -r 3 -d ./Bandera2 -r 30720 -c 30720

 Performance counter stats for './Bandera2 -r 30720 -c 30720' (3 runs):

         50 920,49 msec task-clock                       #    2,998 CPUs utilized               ( +-  8,54% )
               255      context-switches                 #    5,882 /sec                        ( +-  8,38% )
                 4      cpu-migrations                   #    0,092 /sec                      
         1 978 192      page-faults                      #   45,630 K/sec                       ( +- 19,24% )
   <not supported>      cycles                                                                
   <not supported>      instructions                                                          
   <not supported>      branches                                                              
   <not supported>      branch-misses                                                         
   <not supported>      L1-dcache-loads                                                       
   <not supported>      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

             16,99 +- 9,45 seconds time elapsed  ( +- 55,61% )

root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# valgrind --tool=cachegrind ./Bandera2 -r 30720 -c 30720
==4564== Cachegrind, a cache and branch-prediction profiler
==4564== Copyright (C) 2002-2017, and GNU GPL'd, by Nicholas Nethercote et al.
==4564== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==4564== Command: ./Bandera2 -r 30720 -c 30720
==4564== 
--4564-- warning: L3 cache found, using its data for the LL simulation.
==4564== brk segment overflow in thread #1: can't grow to 0x4847000
==4564== (see section Limitations in user manual)
==4564== NOTE: further instances of this message will not be shown
==4564== 
==4564== I   refs:      31,975,739,949
==4564== I1  misses:             1,290
==4564== LLi misses:             1,287
==4564== I1  miss rate:           0.00%
==4564== LLi miss rate:           0.00%
==4564== 
==4564== D   refs:      13,408,427,179  (11,808,757,886 rd   + 1,599,669,293 wr)
==4564== D1  misses:        68,217,409  (     1,351,496 rd   +    66,865,913 wr)
==4564== LLd misses:        66,571,591  (       147,626 rd   +    66,423,965 wr)
==4564== D1  miss rate:            0.5% (           0.0%     +           4.2%  )
==4564== LLd miss rate:            0.5% (           0.0%     +           4.2%  )
==4564== 
==4564== LL refs:           68,218,699  (     1,352,786 rd   +    66,865,913 wr)
==4564== LL misses:         66,572,878  (       148,913 rd   +    66,423,965 wr)
==4564== LL miss rate:             0.1% (           0.0%     +           4.2%  )
```

9. **Mostrar los valores:** 
* page-faults:u = 1 978 192
* L1-dcache-load-misses:u = 68,217,409
* LLC-load-misses:u = 66,571,591
 

**Nota**: perf stat -r 3 -ddd permite obtener más detalles.

**Nota**: La memoria se ha cogido por filas, por lo que es más eficiente visitar las matrices por filas ya que los datos a modificar estarán más tiempo en las caches. 

10. **En Bandera2 cambia el orden del doble bucle, primero las columnas (j) y luego las filas (i). Muestra para esta versión los resultados solicitados en los puntos 7 y 9.**
 * Hay que usar Rows=Cols de 6. 

```console
time ./Bandera2 -r 30720 -c 30720
```

real	0m46,158s

```console
root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# perf stat -r 3 -d ./Bandera2 -r 30720 -c 30720

 Performance counter stats for './Bandera2 -r 30720 -c 30720' (3 runs):

        165 273,86 msec task-clock                       #    2,998 CPUs utilized               ( +- 18,98% )
               745      context-switches                 #    6,667 /sec                        ( +- 19,70% )
                18      cpu-migrations                   #    0,161 /sec                        ( +- 21,36% )
         1 978 190      page-faults                      #   17,703 K/sec                       ( +- 19,25% )
   <not supported>      cycles                                                                
   <not supported>      instructions                                                          
   <not supported>      branches                                                              
   <not supported>      branch-misses                                                         
   <not supported>      L1-dcache-loads                                                       
   <not supported>      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

             55,14 +- 1,58 seconds time elapsed  ( +-  2,87% )

root@Ubuntu:/home/user/Desktop/shared_folder/Bandera# valgrind --tool=cachegrind ./Bandera2 -r 30720 -c 30720
==4649== Cachegrind, a cache and branch-prediction profiler
==4649== Copyright (C) 2002-2017, and GNU GPL'd, by Nicholas Nethercote et al.
==4649== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==4649== Command: ./Bandera2 -r 30720 -c 30720
==4649== 
--4649-- warning: L3 cache found, using its data for the LL simulation.
==4649== brk segment overflow in thread #1: can't grow to 0x4847000
==4649== (see section Limitations in user manual)
==4649== NOTE: further instances of this message will not be shown
^C==4649== 
==4649== Process terminating with default action of signal 2 (SIGINT)
==4649==    at 0x109CDF: main (in /home/user/Desktop/shared_folder/Bandera/Bandera2)
==4649== 
==4649== I   refs:      16,446,890,185
==4649== I1  misses:             1,199
==4649== LLi misses:             1,196
==4649== I1  miss rate:           0.00%
==4649== LLi miss rate:           0.00%
==4649== 
==4649== D   refs:       6,918,489,190  (6,015,425,381 rd   + 903,063,809 wr)
==4649== D1  misses:       857,107,409  (   91,363,359 rd   + 765,744,050 wr)
==4649== LLd misses:       763,232,917  (    2,386,356 rd   + 760,846,561 wr)
==4649== D1  miss rate:           12.4% (          1.5%     +        84.8%  )
==4649== LLd miss rate:           11.0% (          0.0%     +        84.3%  )
==4649== 
==4649== LL refs:          857,108,608  (   91,364,558 rd   + 765,744,050 wr)
==4649== LL misses:        763,234,113  (    2,387,552 rd   + 760,846,561 wr)
==4649== LL miss rate:             3.3% (          0.0%     +        84.3%  )

```

* page-faults:u = 1 978 190
* L1-dcache-load-misses:u = 857,107,409
* LLC-load-misses:u = 763,232,917



11. **¿Cuando se tienen más fallos de cache, visitando la matriz primero por filas o primero por columnas? ¿Porqué?** 

Por columnas, por lo que es más eficiente visitar las matrices por filas ya que los datos a modificar estarán más tiempo en las caches.

12. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

Sí.

- - - 
### Como ver este .md en el navegador
En linux instalar grip:

```console 
$ pip install grip 
```

y ejecutar
```console
$ grip -b README.md
```

### Markdown cheat sheet

Para añadir información a este README.md:

[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
