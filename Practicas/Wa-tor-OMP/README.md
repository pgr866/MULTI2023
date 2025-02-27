# Ejercicio: Wa-tor OMP.

# A rellenar por el alumno/grupo
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
  - Número de cores: 4
  - RAM: 8 GB
  - Capacidad HDD: 50 GB

# Instrucciones:
 * Buscar "TODO" en los ficheros:
```console 
$ grep -n TODO *
```
  * En el Run.sh se tienen los comandos a ejecutar para 
     + -r 102 -c 102 y -ni 10000 con distinto número de hebras y 
     + -r 201 -c 201 -ni 10000. 
  * Hay que asegurarse que el número de iteraciones final realizadas por el programa sean **10000**, es decir deben quedar peces y tiburones después de la ejecución para obtener tiempos.
  * Cuando se miden tiempos, no se usaran las salidas gráficas o de datos de gnuplot. Solo se usarán para chequear que el programa paralelo funciona como el secuencial. Hay que quitar el PRINT=1 y poner PRINT=0, ya que la salida por pantalla consume mucho tiempo. Hay que compilar sin el -g ni el -pg ya que no se está depurando ni haciendo un profiling.

  * Debido a la complejidad de evitar el acceso de varias hebras a una celda, la inicialización del océano con peces y tiburones se hará por una sola hebra.
  * Cada hebra usará su secuencia de números aleatorios con ldrand48_r() para el movimiento de los peces y tiburones.
  * Los números aleatorios se inicializaran con srand48(0).
  * Hay que tener cuidado con los contadores del numero de peces y de tiburones en la rutina IterateOcean.
    + Se pueden hacer reducciones. Usad variables locales que se inicializan a *pNFishes y *pNSharks y luego después de los reduction ya actualizadas se vuelven a asignar a *pNFishes y *pNSharks. Si se hace con #pragma omp critical se obtiene menos nota.
  * Hay que tener en cuenta que para evitar secciones críticas, cuando una celda se visita por una hebra, las que dependen de ella, (las del alrededor) no pueden ser visitadas por otra hebra.
  * Una solución que evita secciones críticas es desenrollar los bucles para evitar condiciones de carrera. Si se fuerza que el tamaño del océano sea múltiplo de 3 en filas y columnas, se podría tener, por ejemplo:
    + Tres bucles dobles con i=0,1,2; i < rows; i=i+3 cada uno  y su respectivo bucle j para todas las columnas, donde cada hebra haría una fila completa, que estaría separada de otra fila en otra hebra por al menos tres filas, evitando conflictos.
    + Nueve bucles, donde se realiza lo mismo que antes pero también para las columnas.



Ejemplo de tres bucles dobles para **rows=cols=9**. Cada hebra se hace cargo de una fila, cuyas celdas están marcadas con x. Las acciones en una celda solo afectan a las de alrededor, que no pueden visitarse por otra hebra dentro del doble bucle. Se habrán visitado todas las celdas al finalizar los tres dobles bucles. 

 - - -
  * **i=0**; i < rows; **i=i+3**
  * > j=0; j < cols ; j++ 

| + | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| - | - | - | - | - | - | - | - | - | - |
| 0 | x | x | x | x | x | x | x | x | x |
| 1 |   |   |   |   |   |   |   |   |   |
| 2 |   |   |   |   |   |   |   |   |   |
| 3 | x | x | x | x | x | x | x | x | x |
| 4 |   |   |   |   |   |   |   |   |   |
| 5 |   |   |   |   |   |   |   |   |   |
| 6 | x | x | x | x | x | x | x | x | x |
| 7 |   |   |   |   |   |   |   |   |   |
| 8 |   |   |   |   |   |   |   |   |   |

 - - -
  * **i=1**; i < rows; **i=i+3**
  * > j=0; j < cols ; j++ 

| + | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| - | - | - | - | - | - | - | - | - | - |
| 0 |   |   |   |   |   |   |   |   |   |
| 1 | x | x | x | x | x | x | x | x | x |
| 2 |   |   |   |   |   |   |   |   |   |
| 3 |   |   |   |   |   |   |   |   |   |
| 4 | x | x | x | x | x | x | x | x | x |
| 5 |   |   |   |   |   |   |   |   |   |
| 6 |   |   |   |   |   |   |   |   |   |
| 7 | x | x | x | x | x | x | x | x | x |
| 8 |   |   |   |   |   |   |   |   |   |

 - - -
  * **i=2**; i < rows; **i=i+3**
  *  > j=0; j < cols ; j++ 

| + | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| - | - | - | - | - | - | - | - | - | - |
| 0 |   |   |   |   |   |   |   |   |   |
| 1 |   |   |   |   |   |   |   |   |   |
| 2 | x | x | x | x | x | x | x | x | x |
| 3 |   |   |   |   |   |   |   |   |   |
| 4 |   |   |   |   |   |   |   |   |   |
| 5 | x | x | x | x | x | x | x | x | x |
| 6 |   |   |   |   |   |   |   |   |   |
| 7 |   |   |   |   |   |   |   |   |   |
| 8 | x | x | x | x | x | x | x | x | x |


## Librerias
   * El paquete **netpbm-progs** (o **netpbm**) para el comando del sistema rawtoppm.
   * El paquete **eog** para visualizar los ficheros .ppm.
   * El paquete **gnuplot** para generar las gráficas de evolución de peces y tiburones.
   * El paquete **valgrind** para chequear posibles memoria asignada y no liberada.
   * El paquete **ffmpeg** para generar video raw que se visualiza con ffplay


## Objetivos
 * Familiarizar al alumno con 
	- El uso de OpenMP.
	- Trabajar con problemas dinámicos donde hay dependencia de datos.
	- Evitar secciones críticas con el desenrollado de bucles.
	- Uso de secuencias de números aleatorios independientes por hebra.

## Compilación

```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Wa-tor -h
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

* Hay que realizar la versión paralela con **tres dobles bucles** en IterateOcean(). 

1. **¿Tiene el algoritmo paralelo perdidas de memoria (memory leaks)?**
 * Usa ```$ valgrind -s ./Wa-tor -ni xx```,  con xx no muy grande, ya que tarda al coger y soltar muchas veces memoria. Si tienes perdida de memoria es que dos animales se han movido a la misma celda y no está bien el código. Muestra aquí el ERROR SUMMARY.

==41013== Memcheck, a memory error detector

==41013== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.

==41013== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info

==41013== Command: ./Wa-tor-OMP -ni 50

==41013== 

------------

OpenMP-parallel with 4 threads

Wa-tor ends. Niter=50, NFishes= 4480, NSharks=1091.

==41013== 

==41013== HEAP SUMMARY:

==41013==     in use at exit: 3,408 bytes in 8 blocks

==41013==   total heap usage: 33,162 allocs, 33,154 frees, 673,176 bytes allocated

==41013== 

==41013== LEAK SUMMARY:

==41013==    definitely lost: 0 bytes in 0 blocks

==41013==    indirectly lost: 0 bytes in 0 blocks

==41013==      possibly lost: 864 bytes in 3 blocks

==41013==    still reachable: 2,544 bytes in 5 blocks

==41013==         suppressed: 0 bytes in 0 blocks

==41013== Rerun with --leak-check=full to see details of leaked memory

==41013== 

==41013== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

2. **¿Se puede hablar de speed-up teórico, según la ley de Amdahl, si en cada ejecución la semilla de la secuencia pseudo-aleatoria se inicia a un número diferente? ¿Porqué?**

* Nota: Termina el algoritmo con el mismo número de peces y tiburones para secuencial, 1 ,2 y 4 hebras? Es decir, ¿se hace el mismo trabajo en las distintas versiones de los algoritmos?

Si en cada ejecución de un programa paralelo la semilla de la secuencia pseudo-aleatoria se inicia con un número diferente, esto podría afectar la medida del speed-up teórico según la Ley de Amdahl. La razón es que las ejecuciones con diferentes semillas pueden dar lugar a diferentes patrones de comportamiento en términos de asignación de recursos y carga de trabajo en los procesadores. Esto puede hacer que la parte paralela del programa (la que se puede ejecutar en paralelo) sea más o menos eficiente en diferentes ejecuciones.

Dado que las ejecuciones con diferentes semillas pueden generar resultados ligeramente diferentes en términos de asignación de recursos y comportamiento de los hilos, el speed-up teórico calculado en una ejecución específica podría no ser representativo de otras ejecuciones con diferentes semillas. Esto podría hacer que la ley de Amdahl no se aplique de manera precisa en todas las ejecuciones.

Para medir el speed-up teórico de manera precisa según la Ley de Amdahl, generalmente se asume que la semilla de la secuencia pseudo-aleatoria y otras condiciones iniciales son consistentes en todas las ejecuciones. Esto permite realizar comparaciones más justas y significativas entre las versiones secuencial y paralela del programa.

Para que el speed-up sea válido y se pueda hablar de una mejora real en el rendimiento, es importante que todas las versiones del algoritmo (secuencial, 1 hebra, 2 hebras y 4 hebras) realicen el mismo trabajo. Esto significa que deben mantener el mismo número de peces y tiburones al final de la simulación. Si las versiones paralelas no mantienen el mismo número de peces y tiburones que la versión secuencial, entonces el speed-up no sería significativo, ya que estaría ocurriendo un comportamiento diferente en las distintas versiones del algoritmo.

3. **Y si la semilla del lrand48() se inicializa siempre con srand48(0) y las semillas de lrand48_r() se inicializan siempre a la su posición *i* en el vector pRandData con srand48\_r(i,&pRandData[i]) ¿Se puede hablar de speed-up? ¿Porqué?**

* Nota: la misma que en el punto 2.

```console
// TODO: init seed for each  drand48_data using sdrand48_r(i,...), i=0..nthreads-1. See man pages.
   for (int i = 0; i < nthreads; i++)
      srand48_r(i, &pRandData[i]);
```

Si se inicializan las semillas de manera diferente para diferentes hilos, entonces se generarán secuencias de números pseudoaleatorios distintas entre los hilos, lo que puede llevar a resultados diferentes en cada hilo. En este caso, no se podría hablar de un speed-up válido y consistente, ya que las diferencias en los números pseudoaleatorios pueden afectar el comportamiento del programa y llevar a resultados no comparables.

Sin embargo, por otro lado estamos poniendo al mismo nivel todas las ejecuciones al revisar que realicen el mismo número de iteraciones.

4. **Si has contestado que si se puede hablar de speed-up, rellena la siguiente tabla sin salidas gráficas ni de datos y usando siempre las mismas semillas para los números aleatorios**:

| Ejecución   |  102x102       | 201x201         |
| ----------- | -------------- | --------------- |
|T.Sec        |                |                 |
|T.CsPar      |                |                 |
|SpA(2)       |                |                 |
|SpA(4)       |                |                 |

4. **Si has contestado que si se puede hablar de speed-up, ¿Donde has medido el tiempo del código secuencial que se va a paralizar (T.CsPar) para calcular SpA(p)?**


5. **A partir de la experiencia con el Mandelbrot, ¿qué scheduler sin especificar el chunk podría ser el mejor para Wa-tor? ¿Porqué?**

El mejor scheduler sería dynamic, ya que es una buena elección para Wa-tor-OMP porque se adapta a la variabilidad de la carga de trabajo, equilibra la carga entre los hilos y permite una granularidad variable sin un excesivo overhead en la asignación de trabajo.

6. Rellena la siguiente tabla usando el scheduler elegido:

| Ejecución   |  102x102       | 201x201         |
| ----------- | -------------- | --------------- |
|T.Sec        |     2.496s     |     11.089s     |
|T(2)         |     1.733s     |     6.485s     |
|T(4)         |     1.213s     |     3.938s      |


8. **¿Se podrían comparar tiempos con distinto número de hebras?** 

No se podrían comparar directamente los tiempos con distinto número de hebras en este caso, ya que hay que tener en cuenta la aleatoriedad de algunos datos. Sin embargo, por otro lado estamos poniendo al mismo nivel todas las ejecuciones al revisar que realicen el mismo número de iteraciones.

9. **¿Son razonables los tiempos alcanzados? ¿Porqué?**

Los tiempos alcanzados son razonables. El tiempo de ejecución disminuye al aumentar el número de hilos, lo cual es un resultado esperado al paralelizar una aplicación en un sistema multi-hilo.

10. **Si se hubieran hecho 9 dobles bucles donde además de las i, las j también estarían separadas por 3 celdas dentro del mismo doble bucle i, teoricamente: ¿Habría más o menos fallos de cache? ¿Sería más eficiente usar collapse(2)? Razona las respuestas.** 

Si se tuvieran 9 bucles anidados, cada uno iterando sobre una celda separada por 3 posiciones dentro del mismo bucle i, teóricamente habría más fallos de caché. Esto se debe a que las celdas con diferencias de 3 posiciones probablemente no estarían almacenadas en la misma línea de caché, lo que podría resultar en más lecturas de memoria principal y, por lo tanto, en un mayor número de fallos de caché.

Usar collapse(2) sería más eficiente en este caso, ya que combinaría los bucles i y j en un solo bucle, permitiendo una mayor eficiencia en términos de fallos de caché y reduciendo la sobrecarga de bucles anidados. Esto sería beneficioso siempre que los bucles i y j fueran independientes y no dependieran el uno del otro.

11. **¿Se te ocurre un método más eficiente de paralelizar Wa-tor?** 

Un método más eficiente para paralelizar Wa-tor podría ser la exploración de técnicas de procesamiento en lotes o vectorización. Utilizar instrucciones SIMD (Single Instruction, Multiple Data) o aceleración de hardware (por ejemplo, GPU) podría proporcionar mejoras significativas en el rendimiento. Además, la implementación actual podría optimizarse para minimizar los accesos a la memoria principal y aprovechar al máximo la jerarquía de la caché para reducir los fallos de caché.

12. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

Sí.

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
