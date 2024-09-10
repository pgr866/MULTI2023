# Ejercicio: Mandelbrot MPI.

## A rellenar por el alumno/grupo
 * Nombre y apellidos alumno 1   : Pablo Gómez Rivas
 * Nombre y apellidos alumno 2   : Jesús Fornieles Muñoz
 * Mayoría en GTA1, GTA2 o GTA3  : GTA1
 * Nombre de grupo de actividades: Grupo Actividades 8

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

* En el Run.sh se tienen los comandos a ejecutar para -mi 1e4 y -mi 1e5 para el algoritmo secuencial (Mandel) y el paralelo (Mandel-MPI) con distinto número de tareas.
* La opción **-o** genera la imagen. La imagen se puede usar para verificar que el código es correcto, al visualizarse con el comando *eog*.

## Librerías
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.

## Objetivos
 * Familiarizar al alumno con 
	- El uso de la librería MPI.
 	- Uso de una estrategia maestro-trabajador donde la tarea maestra (0) reparte trabajo entre las tareas trabajadoras (>0). Este esquema se usa para realizar balanceo dinámico de la carga, cuando la carga computacional por unidad a calcular no es homogénea. Usar Scatter y Gather no sería lo más eficiente para este tipo de cómputo.
 	- En la Opción 2: cómo chequear sin bloqueo si hay o no mensajes para la tarea maestra, de forma que si no los hay la tarea maestra también haga trabajo. 

## Compilación

```console 
$ make -j
$ make -j Mandel
$ make -j Mandel-MPI
$ make -j all
```

## Ayuda parámetros 
```console
$ ./Mandel -h
$ ./Mandel-MPI -h
```

## Ejemplo de compilacion y ejecución
	En el script Run.sh

- - -

# Entrega:


## Speed-up teórico

1. **Rellena la siguiente tabla para la versión secuencial, Mandel.c.**


| Ejecución   | -mi 1e4         | -mi 1e5           |
| ----------- | --------------- | ----------------- |
|T.Sec        |     9.881s      |      95.102s      |
|T.CsPar      |    9.86532s     |      95.0845s     |
|SpA(2)       |   1.996831261   |    1,999632042    |
|SpA(4)       |   3.981047619   |    3,997793063    |

donde
 * T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del $time Mandel ... 
 * T.CsPar: El tiempo de la parte del código secuencial que será paralelizado: doble bucle.
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.



**Se tiene que entregar una de las siguientes opciones**:

## Opción 1: La tarea maestra (0) no realiza cómputo.
 * Es la opción que para la que está preparado el código. Es la más fácil, pero no se obtiene la máxima nota.
 * Se requiere completar el Mandel-MPI. Ver **//TODO** en el código. Solo se requieren modificaciones en las comunicaciones de las tareas trabajadoras (>0) que deben casar con las que realiza la tarea maestra (0).
 ```console
 $ grep -n TODO Mandel-MPI.c
 ```
* Las tareas trabajadoras en Mandel-MPI trabajan por filas completas y en una fila cada vez. Cada fila tiene tres vectores de `char` para RGB.
* Las tareas trabajadoras terminan cuando reciben un número de fila a procesar igual al número de filas. Las filas  van de 0 a Rows-1.


2. **Rellene la siguiente tabla para Mandel-MPI.c:**

T(1) y Sp(1) no tienen sentido. Dan error porque la tarea maestra no realiza cómputo (p debe ser >=2).

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T.Sec        |     9.881s      |     95.102s     |
|T(1)         |     Error       |     Error       | 
|T(2)         |     11.498s     |     102.327s    | 
|T(4)         |     4.341s      |     32.922s     | 
|Sp(1)        |     Error       |     Error       | 
|Sp(2)        |   0,859366846   |   0,929393024   | 
|Sp(4)        |   2,27620364    |   2,88870664    |

3. **Indica al número de filas que realiza cada hebra para una ejecución con p=4 de la tabla anterior.**

```console
user@Ubuntu:~/Desktop/ubuntu_shared_folder/Multiprocesadores/Mandel-MPI$ time mpirun -np 4 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000
Task 0 did 0 rows.
Task 1 did 323 rows.
Task 2 did 356 rows.
Task 3 did 345 rows.

real	0m25.280s
user	1m16.582s
sys	    0m21.042s

user@Ubuntu:~/Desktop/ubuntu_shared_folder/Multiprocesadores/Mandel-MPI$ time mpirun -np 4 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000
Task 0 did 0 rows.
Task 1 did 348 rows.
Task 2 did 348 rows.
Task 3 did 328 rows.

real	0m25.282s
user	1m33.295s
sys	    0m6.066s

user@Ubuntu:~/Desktop/ubuntu_shared_folder/Multiprocesadores/Mandel-MPI$ time mpirun -np 4 ./Mandel-MPI -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000
Task 0 did 0 rows.
Task 1 did 320 rows.
Task 2 did 360 rows.
Task 3 did 344 rows.

real	0m24.493s
user	1m28.395s
sys	    0m7.569s
```

- **¿Difieren los números de filas realizadas por cada hebra de una ejecución a otra? ¿Porqué?**

Sí, los números de filas realizadas por cada hebra difieren entre ejecuciones. Esto se debe a la naturaleza de la distribución de tareas en MPI. La distribución exacta de las filas entre las tareas no está determinada de antemano y puede depender de varios factores, como la carga del sistema y la forma en que MPI asigna las tareas a los procesos.

- **¿Es el número de filas realizado por las hebras de una ejecución similar? ¿Porqué?**

Aunque el número de filas no es idéntico entre ejecuciones, se observa cierta similitud en los resultados. Esto se debe a que las tareas trabajadoras están procesando un conjunto fijo de filas, y la variabilidad en el número de filas entre ejecuciones puede deberse a la asignación dinámica de tareas por MPI y a la forma en que se distribuyen las cargas de trabajo. Sin embargo, la ejecución de las tareas sigue siendo coherente en términos de la distribución de las filas entre ellas.

4. **¿Porqué el tiempo paralelo con dos tareas (-np 2) es siempre peor que el secuencial?**

En este código, la tarea maestra (0) no realiza cómputo y solo coordina las tareas trabajadoras. Por lo tanto, si se utiliza solo un proceso MPI (paralelización con dos tareas), se pierde el beneficio de la paralelización, ya que no hay cómputo concurrente real.
El tiempo paralelo con dos tareas será peor que el secuencial porque, en lugar de realizar cómputo en paralelo, se está incurriendo en el costo de la comunicación MPI 

5. **¿Tiene ventajas su paralelización?**

Sí, la paralelización tiene ventajas cuando se utilizan más de dos tareas. En particular, cuando se emplea un número adecuado de tareas (por ejemplo, p=4), se observa una mejora significativa en el tiempo de ejecución. Esto se debe a que las tareas trabajadoras pueden realizar cómputo en paralelo, procesando diferentes filas de la imagen Mandelbrot simultáneamente.

6. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

Sí.

## Opción 2: La tarea maestra también realiza cómputo.
* Se necesita cambiar más el código, pero se opta a la máxima nota.
* La tarea maestra (0) debe chequear de forma no bloqueante si le ha llegado un mensaje de una tarea trabajadora. 
    + Si le ha llegado un mensaje, debe recoger los resultados y enviarle la nueva fila a procesar, tal como está actualmente el código. 
    + **Si no le ha llegado un mensaje** (MPI_IProbe) la hebra maestra realiza el cómputo de la fila actual en esta iteración del bucle.

7. **Rellene la siguiente tabla para Mandel-MPI.c:**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T.Sec        |                 |                 | 
|T(1)         |                 |                 | 
|T(2)         |                 |                 | 
|T(4)         |                 |                 | 
|Sp(1)        |                 |                 | 
|Sp(2)        |                 |                 | 
|Sp(4)        |                 |                 |

8. **Indica al número de filas que realiza cada hebra para una ejecución con p=2,4 de la tabla anterior.**
    - **¿Difieren los números de filas realizadas por cada hebra de una ejecución a otra? ¿Porqué?**
    - **¿Es el número de filas realizado por las hebras de una ejecución similar? ¿Porqué?**


9. **¿Tiene ventajas su paralelización?**

10. **¿Cuando sería mejor que la hebra maestra haga trabajo?**
    - Cuando el número de tareas trabajadoras es bajo, o
    - Cuando el número de tareas trabajadoras el alto.

11. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

- - - 

### Como ver este .md 
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
