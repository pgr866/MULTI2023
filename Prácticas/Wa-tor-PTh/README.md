# Ejercicio: Wa-tor PTh.

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
    - Número de cores: 6 (lógicos)
    - RAM: 8 GB
    - Capacidad HDD: 50 GB


# Instrucciones:
 * Se pide realizar los **//TODO** del código. 
```console 
$ grep -n TODO *
```
  * En el Run.sh se tienen los comandos a ejecutar, (-r 201 -c 201)  y (-r 300 -c 300) con -ni 10000 y distinto número de hebras. 
**El número de iteraciones al final de la ejecución debe ser 10000**, es decir, deben quedar peces y tiburones después de la ejecución para obtener tiempos.
* No se usaran las salidas gráficas o de datos de gnuplot para medir los tiempos. Solo se usarán para chequear que el programa paralelo funciona como el secuencial.
* Para evitar algunas secciones criticas, se ha forzado que el tamaño del océano sea múltiplo de 3 en filas y columnas y se ha optado por la opción de tener tres bucles con i=0,1,2; i < rows; i=i+3 (y su respectivo bucle j para las columnas completo). **Cada hebra realiza una fila a la vez**.
* Las hebras tienen un barrier antes de empezar cada doble bucle (i,j) para que antes se ponga la *RowToProcess* a 0,1,o 2. Para 0 lo hace el main, y para 1 y 2 lo hace la hebra 0.
* Se ejecutan el main y -nt hebras.
* Nótese que las variables locales de la función de cada hebra son independientes en cada hebra.
* Se usa srand(0) y lrand48() para inicializar el océano por una sola hebra. 
* Se usa srand48\_r(i,&pRandData[i]) y lrand48_r(&pRandData[i],&Res) para el movimiento de peces y tiburones, así como la comida de tiburones. 
* Se entrega la versión secuencial (Wa-tor.c) y la paralela a modificar (Wa-tor-PTh.c).

## Librerias
   * El paquete **netpbm-progs** (o **netpbm**) para el comando del sistema rawtoppm.
   * El paquete **eog** para visualizar los ficheros .ppm.
   * El paquete **gnuplot** para generar las gráficas de evolución de peces y tiburones.
   * El paquete **valgrind** para chequear posibles memoria asignada y no liberada.
   * El paquete **ffmpeg** para generar video raw que se visualiza con ffplay


## Objetivos
 * Familiarizar al alumno con 
	- El uso de PThreads.
	- Trabajar con problemas dinámicos donde hay dependencia de datos.
	- Evitar secciones críticas con el desenrollado de bucles.
	- Usar mutexes y barriers.
	- Uso de secuencias de números aleatorios independientes por hebra.

## Compilación

```console 
$ make -j
$ make -j all
$ make -j Wa-tor
$ make -j wa-tor-PTh
```

## Ayuda parámetros 
```console
$ ./Wa-tor -h
$ ./Wa-tor-PTh -h
```
## Ejemplo de compilacion y ejecución
	En el script Run.sh

Wa-tor realiza una llamada a system para ejecutar **eog** (image viewer) si se le indica salida gráfica **-o** de los movimientos de los animales en el océano.

Wa-tor tambien usa el paquete **gnuplot** que hay que instalar si se le indica **-d** para general un fichero con número de peces y tiburones por iteración y mostrarlos como líneas en gratifico.

Wa-tor tambien usa el paquete **ffmpeg** que hay que instalar si se le indica el parámetro -ffmpeg para que la salida por el pipe generado la coja el comando ffmplay, generando un vídeo sin interrupciones del movimiento en el océano.

Antes de ejecutar Wa-tor hay que ver que no hay otra instancia corriendo:
```console
$ps aux  | grep Wa-tor
$ps aux  | grep eog
$ps aux  | grep gnouplot
$ps aux  | grep ffmplay

```
de **Wa-tor** o en su caso **eog**, **gnuplot**, y/o **ffmplay**. En tal caso matad el proceso necesario con:
```command 
$ kill -9 <pid>
```

- - -

# Entrega :

1. **¿Tiene el algoritmo paralelo finalizado perdidas de memoria (memory leaks)?**
 * Mostrar la salida de 
```console
$ valgrind -s ./Wa-tor-PTh -ni 102 -nt 4 
```
Si hay perdida de memoria es que dos animales se han movido a la misma celda y no está bien el código. Muestra aquí el ERROR SUMMARY.

No tiene pérdidas de memoria:

```console
==14449== Memcheck, a memory error detector
==14449== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==14449== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==14449== Command: ./Wa-tor-PTh -ni 102 -nt 4
==14449==
Wa-tor ends. Niter=102, NFishes= 482, NSharks=1055.
==14449==
==14449== HEAP SUMMARY:
==14449==     in use at exit: 0 bytes in 0 blocks
==14449==   total heap usage: 40,992 allocs, 40,992 frees, 744,918 bytes allocated
==14449==
==14449== All heap blocks were freed -- no leaks are possible
==14449==
==14449== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```


2. **Rellena la siguiente tabla:**
 * T.Sec es el tiempo de ejecución del algoritmo secencial.
 * T(p) es el tiempo de ejecución del algoritmo paralelo con -nt p hebras.

| Ejecución   | 201x201         | 300x300         |
| ----------- | --------------- | --------------- |
|T.Sec        |     8.967s      |     22.151s     |
|T(1)         |     9.889s      |     27.146s     |
|T(2)         |     7.191s      |     16.524s     |
|T(4)         |     6.811s      |     13.481s     |

2. **¿Tiene ventajas su paralelización?** 

Sí, la paralelización tiene ventajas. A medida que aumenta el número de hilos (hebras), el tiempo de ejecución del algoritmo paralelo disminuye en comparación con la ejecución secuencial. Esto indica una mejora en el rendimiento a medida que se aprovechan los recursos de la CPU de manera más eficiente.

3. **¿Se podrían usar menos número de mutexes o barriers? ¿Porqué?**

Sí, es posible reducir el número de mutexes y barreras. En el código, se utiliza una barrera para sincronizar el comienzo y el final de cada iteración en todos los hilos. Sin embargo, podrían utilizarse menos barreras, ya que solo es necesario sincronizar al principio y al final de la simulación completa, en lugar de después de cada iteración individual. Del mismo modo, es posible utilizar menos mutexes si se ajusta la lógica para minimizar las secciones críticas que requieren exclusión mutua. La reducción de mutexes y barreras puede mejorar el rendimiento al reducir la sobrecarga de sincronización.

4. **¿Se te ocurre un método más eficiente de paralelizar Wa-tor?** 

Una posible mejora para la paralelización de Wa-tor podría ser dividir el trabajo en función de las filas en lugar de asignar un conjunto fijo de filas a cada hilo. Esto permitiría una distribución más equitativa de la carga de trabajo entre los hilos, ya que algunas filas pueden requerir más tiempo de procesamiento que otras en función de la simulación. También se podría explorar la paralelización a nivel de celda en lugar de a nivel de fila, lo que permitiría un mayor grado de paralelización en el proceso de actualización de las celdas. Además, el uso de estructuras de datos más eficientes y técnicas de programación paralela avanzadas, como la paralelización basada en tareas, podría mejorar aún más el rendimiento.

5. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

Sí

- - -

# Actividad Wa-tor: reglas.

Tomadas de [wiki](https://en.wikipedia.org/wiki/Wa-Tor)

## Movimento

* En cada iteración un pez o tiburón puede moverse norte sur este u oeste a un pixel adyacente, si no está ocupado por un miembro de su misma especie.

### Peces

* Si los 4 pixeles adyacentes están ocupados (por peces o tiburones), no se mueve.
* Se selecciona el pixel a moverse aleatoriamente entre los libres.

### Tiburón

* Se elige entre los adyacentes ocupados por peces, uno aleatoriamente y se come el pez.
* Si no hay peces adyacentes, se mueve como el pez, evitando otros tiburones.

## Vida y reproducción:

### Pez

Si el pez ha sobrevivido un número de iteraciones se reproduce. Esto ocurre cuando además hay un pixel adyacente libre en el cual se crea un nuevo pez. Ambos, padre e hijo ponen sus contadores de iteraciones a 0.

### Tiburón

* En cada iteración se le decrementa en 1 la energía.
* Al alcanzar la energía 0 el tiburón muere.
* Al comerse un pescado, el tiburón gana una cantidad de energía.
* Si el tiburón alcanza un número de iteraciones, se reproduce como el pez. El hijo coge la energía de creación de tiburón. El padre mantiene la suya.


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

[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)


- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
