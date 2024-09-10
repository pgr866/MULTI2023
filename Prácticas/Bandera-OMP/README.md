# Ejercicio: Bandera OMP.

# A rellenar por el alumno/grupo
## Nombre y apellidos alumno 1   : Pablo Gómez Rivas
## Nombre y apellidos alumno 2   : Jesús Fornieles Muñoz
## Mayoría en GTA1, GTA2 o GTA3  : GTA1
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

## Instrucciones:

El ejemplo muestra como generar una imagen RGB de la bandera de España.

Los parámetros de anchura y altura se introducen por la línea de comandos.

La memoria se asigna de forma dinámica. Aunque se podría escribir directamente en el fichero de salida, se usan tres matrices (R, G y B) para generar los colores RGB de cada pixel.

Se pedirá al alumno que *decore* el programa con directivas OpenMP de forma que los colores ppRed, ppGren y ppBlue de cada pixel puedan calcularse en paralelo con varias hebras. 

El programa podrá ejecutarse en 
  * secuencial al compilarse con gcc  y 
  * en paralelo, al compilarse con gcc -fopenmp y distinto número de hebras.

En el makefile ya se ha puesto el flag OMP. 

Como tamaño de la bandera, se usarán los indicados en las tablas de la entrega. Se puede cambiar el Run.sh para todas las versiones paralelas.

Hay que comparar los tiempos obtenidos con distinto número de hebras y con la versión secuencial.

Según los [consejos/trucos/gotchas](https://www.archer.ac.uk/training/course-material/2017/08/openmp-ox/) de EPCC:
  * En las regiones paralelas 
    - usad siempre **default(none)** y usad **private**, **share**, ... para las variables.
    - Es mejor que las variables sean locales a la región paralela.

### A tener en cuenta:
  * En el Run.sh hay que poner todas las ejecuciones con y sin salida gráfica.
  * Cuando se miden tiempos hay que quitar el PRINT=1 y poner PRINT=0, ya que la salida por pantalla consume mucho tiempo.
  * Si se usa portátil, hay medir tiempos con el portátil enchufado a la corriente ya que si no los cores reducen su rendimiento.
  * Si se miden tiempos hay que compilar sin el -g ni el -pg.
  * Hay que tener en cuanta la opción -o España. Es decir con y sin salida gráfica.
	- $Bandera-OMP -r 200 -c 300 -o España (genera España.png)
	- $Bandera-OMP -r 200 -c 300     (no genera España.png)
  * No se realizan dos ficheros fuentes con el código secuencial y el paralelo. Es el mismo fichero para la versión secuencial y paralela haciendo uso de #ifdef _OPENMP e #ifndef _OPENMP.
  * Al compilar no debe haber warnings.
  * La memoria consumida dependerá del tamaño de la imagen y el tipo de datos a almacenar por pixel. Hay que calcularla.
  * Hay que responder a las preguntas y argumentar los resultados.


## Librerias
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.
  * Normalmente, los comandos OpenMP no están en las man. Hay que instalarlos. Ver como en [OpenMP-man-pages](https://github.com/Shashank-Shet/OpenMP-man-pages).


## Objetivos
 * Familiarizar al alumno con 
    - El uso de OpenMP sobre bucles for. 
 * Como medir el tiempo consumido. 
    - Para el tiempo total se usará la parte real del comando ```$ time programa < parámetros > ``` en la consola. 
    - Para el tiempo consumido de alguna parte del programa: 
        + En secuencial: gettimeoftheday().
        + En paralelo omp_get_wtime(). Hay un ejemplo en OpenMP/solution/pi/pi2.c al descomprimir OpenMPsingle.tar.gz.

- - - 
## Compilación

```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Bandera-OMP -h
```

## Ejemplo de compilacion, establecer parámetros  y ejecución
 * En el script Run.sh
 * Ver las variables de entorno para OpenMP.

- - -

# Entrega:

## Speed-up Teórico:

1. **Describe la formula del speed-up o ganancia de velocidad teórica usando la ley de Amdahl (SpA), en términos del porcentaje del código secuencial a paralelizar y del porcentaje del código secuencial que no se va a paralelizar.**

 * Para calcular SpA(p) solo se miden tiempos del programa secuencial.
 * La fórmula del speed-up teórico utilizando la Ley de Amdahl (SpA) se expresa de la siguiente manera: SpA(p)= 1/(%T.CsPar/p+%T.CsnPar)
 * p = número de elementos de proceso (número de hilos o procesadores utilizados).
 * Se miden en secuencial dentro del código (con gettimeofday()):
    - T.CsPar: Tiempo del código secuencial a paralelizar.
    - T.CsnPar: Tiempo del código secuencial que no se va a paralelizar.
 * Se miden con la parte real de ```$ time programa < parámetros >  ```
    - T.Sec: Tiempo total del programa secuencial.

 * T.CsnPar = T.Sec - T.CsPar
 
 %T.CsPar es el porcentaje del tiempo que se gasta en la parte secuencial a paralelizar.
 * %T.CsPar = T.CsPar/T.Sec
 
 %T.CsnPar es el porcentaje del tiempo que se gasta en la parte secuencial que no se paraleliza.
 * %T.CsnPar = T.CsnPar/T.Sec

2. **¿Cuanto es el valor de SpA(p) si p=1?**

Cuando p = 1, significa que solo se está ejecutando la parte secuencial del programa y no hay paralelización. En este caso, la fórmula del speed-up se convierte en:

SpA(1) = 1 / (%T.CsPar/1 + %T.CsnPar) = 1

Dado que p = 1, el denominador en la fórmula es simplemente (%T.CsPar + %T.CsnPar), lo que es igual a 1. Esto implica que no hay ganancia de velocidad (speed-up) en absoluto, ya que el programa se está ejecutando de manera secuencial y no se está distribuyendo la carga entre múltiples procesos.

3. **¿Cuanto es el valor de SpA(p) si todo el código no es paralelizable?**

Si todo el código es no paralelizable, significa que %T.CsPar es igual a 0 y %T.CsnPar es igual a 1. En este caso, la fórmula del speed-up se convierte en:

SpA(p)= 1/(0/p+1) = 1

El valor de SpA(p) será igual a 1 en este caso. Esto significa que no hay ganancia en velocidad al paralelizar el programa, ya que todo el código se ejecuta secuencialmente.

4. **¿Cuanto es el valor de SpA(p) si se puede paralelizar todo el código?**

Si se puede paralelizar todo el código, significa que %T.CsPar es igual a 1 y %T.CsnPar es 0. En este caso, la fórmula del speed-up se simplifica a:

SpA(p)= 1/(1/p+0) = p

Cuando todo el código es paralelizable, el speed-up puede ser potencialmente muy alto, ya que el tiempo secuencial se reduce significativamente al distribuir la carga entre múltiples elementos de proceso. El speed-up aumentará a medida que aumenta p.

 * Hay que elegir un número de filas y columnas (Rows=Col) múltiplo de 1024 (n*1204, n>1) que haga que el programa secuencial tarde varios segundos y que no consuma toda la RAM.
 * Cada pixel necesita tres bytes, uno para cada color.
 * 1 kB =1024 bytes.

5. **¿Que valor de Rows=Col has elegido? ¿Cuanta memoria (Mem.) ocupa la imagen?** 

He elegido Rows=Cols=20480.

Mem. = Rows x Cols x 3 bytes

Mem. = 20480 x 20480 x 3 bytes = 1200 MB

6. **Rellena la siguiente tabla para la versión secuencial sin salida gráfica.**

 * Hay que compilar sin -fopenmp (en el makefile). _OPENMP no estará definido.
 * Ejemplo de ejecución
```console 
$ time Bandera-OMP -r Rows -c Cols
```

T.CsPar:
El tiempo del código secuencial que será paralelizado: relleno de las matrices ppRed, ppGreen y ppBlue. Ya está en el código la medición del tiempo secuencial que se tarda en la parte del código a paralelizar (bucles que establecen los colores de la bandera) de forma interna en el programa secuencial.

* Se adjunta una hoja de cálculo para calcular SpA() y Sp(). Hay que rellenar los campos T.Sec, T.CsPar, T(p) y p. Se visualiza ejecutando:

```console 
$ oocalc Speed-up.ods
```

Mem. = Rows x Cols x 3 bytes

* -r 1024 -r 1024

Mem. = 1024 x 1024 x 3 bytes = 3 MB

T.Sec = 0.014s

T.CsPar = 0.00511s

p = 2

T.CsnPar = T.Sec - T.CsPar = 0.014 s - 0.00511 s = 0.00889 s

%T.CsPar = T.CsPar / T.Sec = 0.00511 s / 0.014 s ≈ 0.365

%T.CsnPar = T.CsnPar / T.Sec = 0.00889 s / 0.014 s ≈ 0.635

SpA(2) = 1 / (%T.CsPar / p + %T.CsnPar) = 1 / (0.365 / 2 + 0.635) = 1.22

p = 4

SpA(4) = 1 / (%T.CsPar / p + %T.CsnPar) = 1 / (0.365 / 4 + 0.635) = 1.378

* -r 20480 -c 20480

Mem. = 20480 x 20480 x 3 bytes = 1200 MB

T.Sec = 15.194s

T.CsPar = 4.11617s

p = 2

T.CsnPar = T.Sec - T.CsPar =  15.194 s - 4.11617 s = 11.07783 s

%T.CsPar = T.CsPar / T.Sec = 4.11617 s / 15.194 s ≈ 0.271

%T.CsnPar = T.CsnPar / T.Sec = 11.07783 s / 15.194 s ≈ 0.729

SpA(2) = 1 / (%T.CsPar / p + %T.CsnPar) = 1 / (0.271 / 2 + 0.729) = 1.157

p = 4

SpA(4) = 1 / (%T.CsPar / p + %T.CsnPar) = 1 / (0.271 / 4 + 0.729) = 1.255

| Ejecución   | -r 1024 -r 1024 |-r 20480 -c 20480| 
| ----------- | --------------- | --------------- |
| Mem.        |       3 MB      |     1200 MB     |
|T.Sec        |      0.014s     |     15.194s     |
|T.CsPar      |     0.00511s    |     4.11617s    |
|SpA(2)       |       1.22      |      1.157      |
|SpA(4)       |       1.378     |      1.255      |


## Speed-up real Sp(p): 

7. **Describe la formula de la ganancia en velocidad o speed-up real, describiendo los términos utilizados.**
 
 * Sp(p)=T.Sec/T(p)
 * Donde:
    - T(p) : Tiempo total del programa paralelo con p elementos de proceso.

8. **Describe qué realiza el schedule(static) y qué chunk usa por defecto.**

* schedule(static) es una directiva de planificación utilizada en OpenMP para distribuir iteraciones de un bucle entre hilos o procesos. Cuando se utiliza schedule(static), se divide el bucle en trozos de tamaño fijo y se asigna a los hilos de manera estática, lo que significa que a cada hilo se le asigna un número igual de iteraciones.
* El "chunk" es la cantidad de iteraciones que se asigna a cada hilo en cada paso. Por defecto, OpenMP determina el tamaño del chunk automáticamente. El tamaño del chunk puede variar según la implementación de OpenMP y el número de hilos disponibles, pero generalmente se asigna de manera equitativa para mantener un equilibrio de carga.

9. **Rellena la siguiente tabla para la versión paralela. Se usará schedule(static) sin establecer el chunk.** 

 * Incluir el código OMP (decorar el código). Ver **//TODO**.
 * Hay que compilar con -fopenmp (en el makefile). _OPENMP estará definido.
 * Hay que establecer la variable de entorno con el número de hebras. Por ejemplo, para p=4 sería:
```console
$ export OMP_NUM_THREADS=4 
```
 * Al compilarse con -fopenmp, no se mide internamente el tiempo del código secuencial a paralelizar. Solo se mide si _OPENMP no está definido.

* -r 1024 -r 1024

T.Sec = 0.014s

T(2) = 0.014s

T(4) = 0.012s

Sp(2) = T.Sec / T(2) = 0.014s / 0.014s = 1

Sp(4) = T.Sec / T(4) = 0.014s / 0.012s = 1,167

* -r 20480 -c 20480

T.Sec = 15.194s

T(2) = 4.697s

T(4) = 3.656s

Sp(2) = T.Sec / T(2) = 15.194s / 4.697s = 3.235

Sp(4) = T.Sec / T(4) = 15.194s / 3.656s = 4.156


| Ejecución   |-r 1024 -r 1024 |-r 20480 -c 20480| 
| ----------- | -------------- | --------------- |
|T(2)         |     0.014s     |      4.697s     |
|T(4)         |     0.012s     |      3.656s     |
|Sp(2)        |       1        |      3.235      |
|Sp(4)        |     1,167      |      4.156      |


10. **¿Es el SpA(p) distinto del SP(p)? ¿Porqué?**

* -r 1024 -r 1024
SpA(2) = 1.22    Sp(2) = 1
SpA(4) = 1.378    Sp(4) = 1,167

* -r 20480 -c 20480
SpA(2) = 1.157    Sp(2) = 3.235
SpA(4) = 1.255    Sp(4) = 4.156

Sí, el SpA(p) es diferente del Sp(p) en ambos casos, y la diferencia se debe a la naturaleza de cómo se calculan y qué aspectos miden.

SpA(p) es una estimación teórica del speed-up basada en la proporción de código secuencial y paralelizable, y asume que el código secuencial se mantendrá constante. En este caso, SpA(p) es una estimación teórica del potencial de mejora.

Sp(p), por otro lado, mide el rendimiento real en un entorno paralelo. Se obtiene midiendo el tiempo de ejecución real en un programa paralelo. Sp(p) refleja cómo el programa se desempeña en la práctica y se ve afectado por diversos factores, como la eficiencia de la paralelización, la sobrecarga de la comunicación entre hilos, y otros factores que no se tienen en cuenta en SpA(p).

11. **Teóricamente, ¿Mejoraría el Sp() si se establece el tamaño del chunk en el  schedule(static,chunk)? ¿Y si se usa otro scheduler?**

* Tamaño del chunk en schedule(static, chunk):

Sp() puede mejorar: Si se elige un tamaño de chunk adecuado, puede mejorar la distribución de la carga de trabajo entre los hilos. Un chunk más grande reduce la sobrecarga de la planificación y la comunicación entre hilos, lo que puede resultar en un mejor rendimiento.

Sp() puede empeorar: Si se selecciona un tamaño de chunk demasiado grande, es posible que algunos hilos terminen su trabajo antes que otros, lo que podría causar desequilibrios en la carga de trabajo y reducir el rendimiento.

* Uso de otro scheduler:

Sp() puede mejorar: Dependiendo del programa y la carga de trabajo, otros schedulers como schedule(dynamic) o schedule(guided) pueden ser más adecuados. Estos schedulers pueden adaptarse mejor a cargas de trabajo dinámicas y redistribuir tareas de manera más eficiente.

Sp() puede empeorar: Si se utiliza un scheduler no adecuado para la carga de trabajo, es posible que la planificación de tareas resulte en una sobrecarga de comunicación entre hilos y una falta de equilibrio en la carga de trabajo, lo que podría reducir el rendimiento.

12. **¿Qué hace el collapse(2) en la directiva OMP?**

La cláusula collapse(n) en una directiva de OpenMP se utiliza para combinar múltiples bucles anidados en un solo bucle, lo que permite una paralelización más efectiva de estos bucles. El número n especifica cuántos bucles anidados se deben colapsar en un solo bucle, en este caso son 2.

Esto puede mejorar el rendimiento al reducir la sobrecarga de la creación y administración de hilos en comparación con paralelizar cada bucle por separado. Sin embargo, no siempre es beneficioso utilizar collapse(n), ya que esto depende de la naturaleza del programa y la carga de trabajo.

13. **Rellena la siguiente tabla para la versión paralela con schedule(static) y collapse(2)**

* -r 1024 -r 1024

T.Sec = 0.014s

T(2) = 0.014s

T(4) = 0.014s

Sp(2) = T.Sec / T(2) = 0.014s / 0.014s = 1

Sp(4) = T.Sec / T(4) = 0.014s / 0.014s = 1

* -r 20480 -c 20480

T.Sec = 15.194s

T(2) = 3.508s

T(4) = 2.660s

Sp(2) = T.Sec / T(2) = 15.194s / 3.508s = 4.331

Sp(4) = T.Sec / T(4) = 15.194s / 2.660s = 5.712

| Ejecución   |-r 1024 -r 1024 |-r 20480 -c 20480| 
| ----------- | -------------- | --------------- |
|T(2)         |     0.014s     |      3.508s     |
|T(4)         |     0.014s     |      2.660s     |
|Sp(2)        |       1        |      4.331      |
|Sp(4)        |       1        |      5.712      |

**14. ¿Mejora el Sp(p)? ¿Porqué?**

* -r 1024 -r 1024
sin collapse(2)       con collapse(2)
Sp(2) = 1             Sp(2) = 1
Sp(4) = 1,167         Sp(4) = 1

* -r 20480 -c 20480
sin collapse(2)       con collapse(2)
Sp(2) = 3.235         Sp(2) = 4.331
Sp(4) = 4,156         Sp(4) = 5.712

Los tiempos para -r 1024 -r 1024 no se ven mejorados significativamente por la adición de collapse(2) ya que los tiempos originales ya eran bastante pequeños, y cualquier mejora es marginal en este caso.

Sin embargo, para -r 20480 -c 20480, el uso de collapse(2) ha llevado a una mejora considerable en el Speed-Up (Sp(p)). Esto se debe a la naturaleza de la carga de trabajo. La adición de collapse(2) colapsa los dos bucles anidados en uno solo, permitiendo una mejor explotación de la paralelización. En esta configuración, el tamaño de los bucles es mucho mayor, lo que significa que hay más trabajo que puede ser paralelizado de manera eficiente.

En resumen, la mejora en el Sp(p) con collapse(2) se debe a la mejor explotación de la paralelización en bucles grandes, lo que resulta en una distribución más eficiente del trabajo entre los hilos, y en última instancia, en una mejora en el rendimiento global.

 * Incluye y compara la salida de los rendimientos ( ```$ perf stat -r 3 -ddd programa < parámetros >``` usando o no collapse (2). 

* Salida sin collapse(2)

``` console
OpenMP-parallel with 4 threads
OpenMP-parallel with 4 threads
OpenMP-parallel with 4 threads

 Performance counter stats for './Bandera-OMP -r 1024 -c 1024' (3 runs):

             45.69 msec task-clock                       #    2.629 CPUs utilized               ( +- 19.51% )
                70      context-switches                 #    2.308 K/sec                       ( +- 16.91% )
                 9      cpu-migrations                   #  296.728 /sec                        ( +- 19.25% )
             2,597      page-faults                      #   85.623 K/sec                       ( +- 19.22% )
  <not supported stats...>                                           

          0.017380 +- 0.000894 seconds time elapsed  ( +-  5.14% )

OpenMP-parallel with 4 threads
OpenMP-parallel with 4 threads
OpenMP-parallel with 4 threads

 Performance counter stats for './Bandera-OMP -r 20480 -c 20480' (3 runs):

         13,515.70 msec task-clock                       #    4.739 CPUs utilized               ( +- 17.60% )
               148      context-switches                 #   15.896 /sec                        ( +- 18.38% )
                 9      cpu-migrations                   #    0.967 /sec                        ( +- 19.25% )
           909,449      page-faults                      #   97.677 K/sec                       ( +- 19.24% )
   <not supported stats...>                                            

             2.852 +- 0.407 seconds time elapsed  ( +- 14.26% )
```

* Salida con collapse(2)

``` console
OpenMP-parallel with 4 threads
OpenMP-parallel with 4 threads
OpenMP-parallel with 4 threads

 Performance counter stats for './Bandera-OMP -r 1024 -c 1024' (3 runs):

             55.25 msec task-clock                       #    0.796 CPUs utilized               ( +- 20.82% )
                75      context-switches                 #    2.142 K/sec                       ( +- 18.10% )
                 8      cpu-migrations                   #  228.441 /sec                        ( +- 21.65% )
             2,596      page-faults                      #   74.129 K/sec                       ( +- 19.22% )

   <not supported stats...>
                                               
            0.0694 +- 0.0282 seconds time elapsed  ( +- 40.56% )

OpenMP-parallel with 4 threads
OpenMP-parallel with 4 threads
OpenMP-parallel with 4 threads

 Performance counter stats for './Bandera-OMP -r 20480 -c 20480' (3 runs):

         14,222.35 msec task-clock                       #    4.987 CPUs utilized               ( +- 18.44% )
               190      context-switches                 #   19.561 /sec                        ( +- 16.48% )
                 9      cpu-migrations                   #    0.927 /sec                        ( +- 19.25% )
           909,446      page-faults                      #   93.631 K/sec                       ( +- 19.25% )

   <not supported stats...>

            2.8518 +- 0.0462 seconds time elapsed  ( +-  1.62% )
```

## Experimentos con salida gráfica (-o España): 

15. **Rellena la siguiente tabla para la ejecución secuencial con salida gráfica.**
 * Hay que compilar sin -fopenmp (en el makefile). _OPENMP no estará definido.
 * Ejemplo de ejecución (Rows=Cols se establecieron en el punto 5):
```console 
$ time Bandera-OMP -r Rows -c Cols -o España
```

He elegido Rows=Cols=20480.

Mem. = Rows x Cols x 3 bytes

* -r 1024 -r 1024

Mem. = 1024 x 1024 x 3 bytes = 3 MB

T.Sec = 0.647s

T.CsPar = 0.004173s

p = 2

T.CsnPar = T.Sec - T.CsPar = 0.647s - 0.004173s = 0.643s

%T.CsPar = T.CsPar / T.Sec = 0.004173s / 0.647s = 0.00645

%T.CsnPar = T.CsnPar / T.Sec = 0.643s / 0.647s = 0.994

SpA(2) = 1 / (%T.CsPar / p + %T.CsnPar) = 1 / (0.00645 / 2 + 0,994) = 1.00278

p = 4

SpA(4) = 1 / (%T.CsPar / p + %T.CsnPar) = 1 / (0.00645 / 4 + 0,994) = 1.00441

* -r 20480 -c 20480

Mem. = 20480 x 20480 x 3 bytes = 1200 MB

T.Sec = 277.897s

T.CsPar = 3.65719s

p = 2

T.CsnPar = T.Sec - T.CsPar = 277.897s - 3.65719s = 274.24s

%T.CsPar = T.CsPar / T.Sec = 3.65719s / 277.897s = 0.0132

%T.CsnPar = T.CsnPar / T.Sec = 274.24s / 277.897s = 0.987

SpA(2) = 1 / (%T.CsPar / p + %T.CsnPar) = 1 / (0.0132 / 2 + 0.987) = 1.00644

p = 4

SpA(4) = 1 / (%T.CsPar / p + %T.CsnPar) = 1 / (0.0132 / 4 + 0.987) = 1.0098

| Ejecución   | -r 1024 -r 1024 |-r Rows -c Cols | 
| ----------- | --------------- | -------------- |
| Mem.        |       3 MB      |     1200 MB    |
|T.Sec        |       0.647s    |    277.897s    |
|T.CsPar      |    0.004173s    |    3.65719s    |
|SpA(2)       |     1.00278     |    1.00644     |
|SpA(4)       |     1.00441     |     1.0098     |


16. **Rellena la siguiente tabla para la ejecución paralela con salida gráfica, con schedure(static) y sin collapse().**

* -r 1024 -r 1024

T.Sec = 0.647s

T(2) = 0.599s

T(4) = 0.549s

Sp(2) = T.Sec / T(2) = 0.647s / 0.599s = 1.08

Sp(4) = T.Sec / T(4) = 0.647s / 0.549s = 1.179

* -r 20480 -c 20480

T.Sec = 277.897s

T(2) = 243.099s

T(4) = 239.075s

Sp(2) = T.Sec / T(2) = 277.897s / 243.099s = 1.143

Sp(4) = T.Sec / T(4) = 277.897s / 239.075s = 1.162

| Ejecución   |-r 1024 -r 1024 |-r 20480 -c 20480| 
| ----------- | -------------- | --------------- |
|T(2)         |     0.599s     |    243.099s     |
|T(4)         |     0.549s     |    239.075s     |
|Sp(2)        |      1.08      |      1.143      |
|Sp(4)        |     1.179      |      1.162      |



16. **¿Porqué ahora el SpA() y Sp(p) son peores que en la tablas 6 y 9?** 
* Responde usando como argumentos los porcentajes de código paralelizable y no paralelizable.

El SpA() y Sp(p) son peores en las tablas con salida gráfica (-o España) en comparación con las tablas sin salida gráfica debido a la mayor proporción de código no paralelizable en el caso de las imágenes más grandes.

Cuando se genera una salida gráfica, el proceso de escritura de la imagen consume una cantidad significativa de tiempo en comparación con el procesamiento de píxeles. Esto significa que el código secuencial (no paralelizable) representa una proporción mayor del tiempo total de ejecución en comparación con el procesamiento de píxeles. Como resultado, los porcentajes de código paralelizable (%T.CsPar) son más bajos y los porcentajes de código no paralelizable (%T.CsnPar) son más altos en las ejecuciones con salida gráfica.

Dado que SpA() y Sp(p) dependen de estos porcentajes, un aumento en %T.CsnPar y una disminución en %T.CsPar conducen a una disminución en el rendimiento de paralelización. Por lo tanto, el SpA() y Sp(p) son peores en las tablas con salida gráfica que en las tablas sin salida gráfica debido a la mayor presencia de código no paralelizable en las ejecuciones con salida gráfica.

17. **¿Porque SpA(p) y Sp(p) no mejoran sustancialmente al aumentar el tamaño de la imagen?**

SpA(p) y Sp(p) pueden no mejorar sustancialmente al aumentar el tamaño de la imagen debido a varias razones:

* Ley de Amdahl: La Ley de Amdahl establece que el speed-up de un programa paralelo está limitado por la fracción no paralelizable del código. A medida que aumenta el tamaño de la imagen, es probable que la cantidad de código no paralelizable también aumente, lo que limita el potencial de mejora del rendimiento.

* Overhead de Paralelización: A medida que se agregan más recursos de procesamiento para manejar imágenes más grandes, también puede aumentar el tiempo dedicado a la sincronización y la gestión de múltiples hilos. Este overhead de paralelización puede reducir la ganancia de velocidad obtenida al aumentar el número de hilos.

* Recursos Limitados: Si los recursos de hardware (como núcleos de CPU) son limitados, puede haber una saturación en la ganancia de velocidad. A medida que se aumenta el tamaño de la imagen, es posible que no haya suficientes recursos para paralelizar eficazmente el procesamiento, lo que limita la mejora del rendimiento.

18. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

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

### Markdown cheat sheet

Para añadir información a este README.md:

[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
