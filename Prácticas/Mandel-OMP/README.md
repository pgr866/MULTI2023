# Ejercicio: Mandel OMP

# A rellenar por el alumno/grupo
## Nombre y apellidos alumno 1   : Pablo Gómez Rivas
## Nombre y apellidos alumno 2   : Jesús Fornieles Muñoz
## Mayoría en GTA1, GTA2 o GTA3  : : GTA1
## Nombre de grupo de actividades: Grupo Actividades 8


# Descripción de la arquitectura utilizada:
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

# Instrucciones

El ejemplo muestra como generar una imagen en color del fractal de Mandelbrot.

> Información del fractal de Mandelbrot en la [wiki](https://es.wikipedia.org/wiki/Conjunto_de_Mandelbrot). 
>
> Mandelbrot set [Rosetta code](https://rosettacode.org/wiki/Mandelbrot_set#C)
>
> Mandelbrot set  [Techniques for computer generated pictures](https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index.php/Mandelbrot_set) 

El fractal de Mandelbrot es en blanco y negro aunque se pueden generar tonos de gris o colores dependiendo del número de iteraciones "NIter" que se realicen sobre cada punto (pixel). El número de iteraciones en cada pixel es normalmente diferente porlo que se necesita balanceo dinámico e la carga para obtener buenos speed-ups.

Se proporciona el código que genera el fractal de Mandelbrot en color usando la rutina NIterToRGB(). 

Se pedirá al alumno que *decore* el programa con directivas OpenMP para obtener una versión paralela (gcc -fopenmp). Debe poder compilarse también en serie (gcc) sin warnnings. 

Los parámetros se introducen por la línea de comandos.

La memoria se asigna de forma dinámica. 
Aunque se podría escribir directamente en el fichero de salida, se usan:
  *  Tres matrices (R, G y B) para generar los colores RGB de cada pixel.


### A tener en cuenta:
  * En el Run.sh se encuentran las distintas ejecuciones paralelas con -mi 1e4 y -mi 1e5, y con distinto número de hebras, no la secuencial.
  * Cuando se miden tiempos hay que quitar el PRINT=1 y poner PRINT=0 ya que la salida por pantalla consume mucho tiempo.
  * Si se usa portátil, medir tiempos con el portátil enchufado a la corriente ya que si no os cores reducen su rendimiento.
  * Si se miden tiempos hay que compilar sin el -g ni el -pg.
  * No se realizan dos ficheros con el código secuencial y el paralelo. Es el mismo fichero para la versión secuencial y paralela, compilando o no con -fopenmp y haciendo uso de #ifdef _OPENMP y/o #ifndef _OPENMP.
  * Al compilar no debe haber warnings.
  * Hay que responder a las preguntas y argumentar los resultados.
  * Para el calculo de la ganancia de velocidad real (speed-up) Sp(p) se usará el tiempo real devuelto por $time Mandel-OMP .....
  * En el programa secuencial ( compilado sin -fopenmp) ya se mide el tiempo del código secualcial aparalelizar. El tiempo total del programa, serie o paralelo, se obtiene con la parte read de ```$ time .Mandel-OMP ...```. 
  * El parámetro -mi indica el número máximo de iteraciones a realizar en un pixel. Habrá pixeles en los que se llegue a ese máximo y otros en los que no. Por eso el trabajo en cada pixel no es estático, es variable. Esto requerirá de algún tipo de balanceo de la carga entre hebras. 

## Librerias
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.
  * Los comandos OpenMP no están en las man. Hay que instalarlos. Ver como en [OpenMP-man-pages](https://github.com/Shashank-Shet/OpenMP-man-pages).

## Objetivos
 * El uso de OpenMP sobre bucles for y el uso de distintos *schedule* y como establecer el *chunk*.
 * Cómo medir el tiempo consumido de CPU y wall clock time.
 * Optimizar algoritmos paralelos en OMP para problemas donde la cantidad de trabajo computacional a realizar por cada tarea no es el mismo.

## Compilación

```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Mandel -h
```

## Ejemplo de compilacion y ejecución
	En el script Run.sh

- - -

# Entrega:

## Speed-up Teórico:

1. **Rellena la siguiente tabla para la versión secuencial.**
 * Memoria: Es la memoria consumida por ppRed, ppGreen y ppBlue y otros datos usados.
 * T.Sec: El wall-clock time del programa secuencial. Parte real del ```$ time Mandel-OMP ...```.
 * T.CsPar: El tiempo del código secuencial que será paralelizado. 
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.
 * Se adjunta una hoja de cálculo para calcular SpA() y Sp(). Hay que rellenar los campos T.Sec, T.CsPar, T(p) y p. Se visualiza ejecutando:

```console 
$ oocalc Speed-up.ods
```

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
| Memoria(Gb) |     0.00425     |    0.00425      |
|T.Sec        |     11.767s     |    93.662s      |
|T.CsPar      |     10.952s     |    93.0805s     |
|SpA(2)       |      1.87       |     1.988       |
|SpA(4)       |     3.312       |     3.927       |


2. **¿Porqué ahora la salida gráfica no es tan influyente en SpA() como en la actividad Bandera-OMP?** 
 * Contesta teniendo en cuenta los porcentajes de código paralelizado y no paralelizado.

En Mandel-OMP, la salida gráfica no es tan influyente en SpA() como en la actividad Bandera-OMP, ya que la generación de imágenes es principalmente una operación secuencial y no paralelizable.

En este caso, gran parte del código es paralelizable (reflejado en T.CsPar) y solo una pequeña parte del código (la generación de imágenes) es secuencial. Por lo tanto, la generación de imágenes no tiene un impacto significativo en el rendimiento global del programa. El cálculo paralelizado es la parte que se beneficia del paralelismo y mejora el rendimiento con el aumento del número de hebras.

## Speed-up real Sp(p): 

La opción **-o** genera la imagen, que debe ser correcta al visualizarse con el comando *eog* al usar hebras.

3. **Describe que realiza el schedule(static) y qué chunk usa por defecto.**

La directiva "schedule(static)" en OpenMP se utiliza para controlar cómo se distribuyen las iteraciones de un bucle entre las hebras en un equipo paralelo. En el caso de "schedule(static)", se divide el bucle en fragmentos de tamaño uniforme y se asigna cada fragmento a una hebra. Cada hebra ejecutará su fragmento de manera secuencial. El tamaño del fragmento se especifica mediante el "chunk" (también conocido como "stride" o "block size"). Por defecto, el "chunk" para "schedule(static)" es 1.

4. **Rellena la siguiente tabla. Para la parte paralela se usará schedule(static) sin establecer el chunk.** 

 * T(p): Wall clock time del programa paralelo con p hebras.
 * Sp(p): ganancia en velocidad con p hebras.

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T(1)         |    12.905s      |    110.361s     | 
|T(2)         |    8.510s       |     80.668s     | 
|T(4)         |    6.171s       |     48.657s     | 
|Sp(1)        |    0.912        |     0.849       | 
|Sp(2)        |    1.383        |     1.161       | 
|Sp(4)        |    1.907        |     1.925       | 

5. **¿Hay diferencias entre T.Sec y T(1)?**
 * Ver el número de hebras se usan realmente en T(1). Hay más recursos? 

 El hecho de que T(1) sea más lento que T.Sec sugiere que no se están utilizando eficazmente los recursos de paralelización y, en lugar de mejorar el rendimiento, están agregando cierta sobrecarga de administración de hebras en la versión paralela.

6. **¿Porqué no mejora sustancialmente el Sp(4) respecto al Sp(2)?**
 * Ver la imagen generada. A cuantas hebras le ha tocado casi todo el trabajo para p=2 y p=4? 

7. **Describe que realiza el schedule(dynamic) y que chunk usa por defecto.**

El planificador schedule(dynamic) en OpenMP distribuye dinámicamente las iteraciones de un bucle entre las hebras disponibles. El planificador asigna a cada hebra un chunk de trabajo (un conjunto de iteraciones) para procesar. Cuando una hebra completa su chunk de trabajo, el planificador le asigna un nuevo chunk, y así sucesivamente hasta que todas las iteraciones se completen.

Por defecto, el tamaño del chunk es 1, lo que significa que a cada hebra se le asigna una sola iteración a la vez

schedule(dynamic) es útil en situaciones donde la carga de trabajo de las iteraciones es variable o desconocida de antemano. Permite una distribución dinámica de las iteraciones, lo que puede ayudar a equilibrar la carga de trabajo entre las hebras de manera más eficiente y adaptarse a diferentes condiciones de ejecución

8. **Rellena la siguiente tabla. Para la parte paralela se usará schedule(dynamic) sin establecer el chunk.**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T(1)         |     9.470s      |     98.846s     | 
|T(2)         |     6.691s      |     46.807s     | 
|T(4)         |     4.749s      |     33.060s     | 
|Sp(1)        |     1.243       |     0.948       | 
|Sp(2)        |     1.759       |     2.001       | 
|Sp(4)        |     2.478       |     2.833       | 


9. **Describe que realiza el schedule(guided) y que chunk usa por defecto.**

El planificador schedule(guided) en OpenMP distribuye las iteraciones de un bucle entre las hebras disponibles de manera similar a schedule(dynamic), pero con una diferencia importante. En schedule(guided), el tamaño de los chunks se asigna inicialmente grande y disminuye gradualmente a medida que las hebras trabajan en las iteraciones. Esta es una estrategia que se utiliza para equilibrar la carga de trabajo entre las hebras y para minimizar la sobrecarga de asignación de chunks.

El tamaño del chunk inicial en schedule(guided) es mayor que en schedule(dynamic), lo que permite que cada hebra reciba una cantidad sustancial de trabajo al principio. A medida que las hebras completan sus chunks, el planificador les asigna chunks más pequeños. El tamaño de los chunks disminuye de manera exponencial hasta que todas las iteraciones se completan.

Por defecto, el tamaño de chunk en schedule(guided) se establece en 1. Sin embargo, este valor predeterminado se ajusta dinámicamente durante la ejecución para adaptarse a la carga de trabajo y la cantidad de iteraciones restantes.

10. **Rellena la siguiente tabla. Para la parte paralela se usará schedule(guided) sin establecer el chunk.**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T(1)         |     9.557s      |     83.939s     | 
|T(2)         |     5.362s      |     47.251s     | 
|T(4)         |     5.834s      |     57.196s     | 
|Sp(1)        |     1.231       |     1.116       | 
|Sp(2)        |     2.195       |     1.982       | 
|Sp(4)        |     2.017       |     1.638       | 


11. **¿Qué scheduler se comporta mejor para -mi 1e5? ¿Porqué?**

schedule(dynamic), ya que es beneficioso cuando las iteraciones no son uniformes en términos de tiempo de ejecución, lo que es común en el cálculo del conjunto de Mandelbrot. Esto permite un uso más eficiente de los recursos y puede conducir a mejores tiempos de ejecución para -mi 1e5.

12. **¿Cual es el mejor chunk para el scheduler dynamic y -mi 1e5?**
* Hay que hacer una búsqueda dicotómica del mejor chunk.
* nIter= número de iteraciones totales a paralelizar.
* Se obtiene T(p).max con el chunk max = máximo chunk = nIter/p
* Se obtiene T(p).min con el chunk min = mínimo chunk = 1.
* Repetir
    - Se obtiene T(p).med con el chunk med = (max - min)/2
    - Si T(p).min < T(p).max
       + max=med
    - Si no min=med
* hasta que T(p).min sea similar a T(p).max

Para p=2, el mejor chunk es 3.
Para p=4, el mejor chunk es 4.

13. **Rellena la siguiente tabla para el scheduler y chunk del punto anterior.**

| Ejecución   | -mi 1e5         |
| ----------- | --------------- |
|Chuck p=2    |        3        |
|Chunk p=4    |        4        |
|T(2)         |     41.981s     |
|T(4)         |     32.048s     |
|Sp(2)        |      2.231      |
|Sp(4)        |      2.923      |

14. **Explica el porqué de los resultados usando chunk y sin usarlo.**

* Sin usar "chunk" o con "chunk" demasiado grande:

En el caso de no especificar un "chunk" o usar un "chunk" grande, el bucle se divide en pedazos muy grandes que se asignan a cada hilo. Esto significa que un hilo obtiene una gran cantidad de trabajo antes de que los demás comiencen, lo que podría llevar a una mala distribución de la carga de trabajo.
Algunos hilos pueden terminar más rápido que otros, lo que resulta en desequilibrio de carga y tiempo de inactividad para los hilos más rápidos.
Puede haber una sobrecarga de sincronización ya que los hilos tienen que coordinarse con frecuencia.

* Usando "chunk" adecuado:

Al ajustar adecuadamente el valor del "chunk", se divide el bucle en pedazos más pequeños y equitativos que se asignan a cada hilo.
Esto permite un mejor equilibrio de carga, ya que cada hilo obtiene aproximadamente la misma cantidad de trabajo y no hay un desequilibrio significativo en la ejecución.
Reduce la sobrecarga de sincronización, ya que los hilos no tienen que coordinarse con tanta frecuencia, lo que ahorra tiempo y recursos.

El objetivo de ajustar el "chunk" es encontrar un equilibrio entre el paralelismo y la distribución de carga de trabajo. Si el "chunk" es demasiado pequeño, la sobrecarga de sincronización puede superar los beneficios del paralelismo. Si es demasiado grande, el desequilibrio de carga y la inactividad del hilo pueden reducir la eficiencia.

15. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

Sí.

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

- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto. Informática, UAL.
