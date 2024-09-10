# Ejercicio: Bandera MPI.

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
    - Número de cores: 4
    - RAM: 8 GB
    - Capacidad HDD: 50 GB

# Instrucciones:
  * Ya se asignan pixeles a las tareas, de forma que:
    + Hacen un trabajo similar
    + Ningún pixel debe quedarse sin hacer.
    + Que un pixel no se haga más de una vez.
  * El makefile y el  Run.sh ya se encuentran preparados para las ejecuciones secuencial y paralelas. 
    + El valor **2048** hay que cambiarlo para que el secuencial dure al menos varios segundos sin consumir la RAM. 
  * La salida gráfica se usará solo para verificar que se está haciendo correctamente el proceso. No se usará para medir tiempos.
  * La memoria consumida dependerá del tamaño de la imagen y el tipo de datos a almacenar. Hay que calcularlo. La memoria consumida no es la misma en la tarea 0 que en las demás tareas.
  * Debido a que MPI envía secuencias de datos contiguas y la imagen es 2D, se han cogido vectores de tamaño ROWs*COLs para los colores RGB.
  * Se hace una distribución estática, donde cada tarea (incluyendo la 0) calcula la posición y el  número de pixeles que debe procesar.
  * Cada tarea calcula los valores R y G  de los pixeles que le han tocado y los manda a la tarea 0 que los recoge y opcionalmente los escribe en un fichero para que se pueda visualizar la bandera. 
  * Busca que hacer en los TODO. Hay que realizarlos **todos**:
```console
$ grep TODO *
```

## Librerias
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.


## Objetivos
  * Familiarizar al alumno con 
	- El uso de la librería MPI. 
  * Usar MPI_Probe cuando no se sabe de quien se recibe el mensaje.
  * Usar MPI_Get_count cuando no se sabe el tamaño que hay que poner en el receive.
  * Entender la memoria consumida por las tareas.

### A tener en cuenta:
  * Para saber el funcionamiento de las funciones de la librería, una vez cargada con  (en Ubuntu no hay que cargarla):

```console 
$ module load < libreria mpi >  
```
hágase

```console 
$ man <rutina>  
```
Por ejemplo, 
```console
$ man MPI_Probe
$ man MPI_Scatterv
```

## Compilación

```console 
$ make Bandera
$ make Bandera-MPI
$ make all 
```

## Ayuda parámetros 
```console
$ ,/Bandera -h
$ ./Bandera-MPI -h
```

## Ejemplo de compilacion, establecer parámetros  y ejecución
 * En el script Run.sh
 * El valor 2048 habrá que cambiarlo.

# Entrega

**Se tiene que entregar una de las siguientes opciones**:

### Opcion 1: Realizar los //TODO:
  * En el ejemplo hay que realizar lo que se encuentra en **//TODO**
  * Es la opción que para la que está preparado el código. Es la más fácil, pero no se obtiene la máxima nota.

### Opcion 2: Usar MPI_Scatterv y MPI_Gatherv
* Se usan las versiones "v" porque a cada tarea le pueden corresponder un número de pixeles distinto ya que el número de pixeles puede no ser divisible por el número de tareas. Puede forzarse a que el uso de las versiones v (Scatterv) o (Gatherv) no sea necesario, en tal caso razonar la solución propuesta y se tiene menos nota.

 
## Speed-up teórico

* Al igual que en ejercicios anteriores, hay que elegir un número de filas y columnas (Rows=Col) múltiplo de 1024 (n*1204, n>1) que haga que el programa secuencial (Bandera.c) tarde varios segundos y que no consuma toda la RAM. Se puede usar el mismo valor que se usó en los ejercicios anteriores, si no se ha cambiado la arquitectura.
* Para la **Opción 2** hay que usar un valor que no sea divisible entre el número de hebras. Habrá que sumarle algo a Rows=Cols del punto anterior para que no sea múltiplo de 2.

1. **¿Que valor de Rows=Col has elegido? ¿Cuanta memoria (Mem.) ocupa la imagen?** 

Para Rows=Cols=20480:

Tamaño de la imagen en memoria = Rows x Cols x 3 bytes = 20480 x 20480 x 3 bytes = 12582912 bytes = **12 MB**

2. **Usa la versión secuencial *Bandera.c* para rellenar la siguiente tabla.**
 * Ejemplo de ejecución sin salida gráfica:
```console 
$ time ./Bandera -r <Rows> -c <Cols>
```

| Ejecución   | -r 1024 -c 1024 | -r Rows -c Cols |
| ----------- | --------------- | --------------- |
|T.Sec        |      0.019s     |     14.406s     |
|T.CsPar      |    0.003983s    |     2.77649s    |
|SpA(2)       |   1.117088515   |   1.106642428   |
|SpA(4)       |   1.186554464   |   1.168973515   |

donde
 * T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del $time Bandera ... 
 * T.CsPar: El tiempo del código secuencial que será paralelizado: relleno de las matrices ppRed, ppGreen y ppBlue. 
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.

## Resultados del algoritmo paralelo

3. **¿Qué opción se ha seleccionado?**

Opcion 1: Realizar los //TODO

4. **Usa la versión paralela *Bandera-MPI.c* para rellenar la siguiente tabla.**
 * Ejemplo de ejecución sin salida gráfica:
```console 
$ time mpirun -np <p> ./Bandera-MPI -r <Rows> -c <Cols>
```

| Ejecución       | -r 768 -c 1024 | -r Rows -c Cols |
| --------------- | -------------- | --------------- | 
|T.Sec            |    0.019s      |     14.406s     |
|T(1)             |    0.436s      |     3.461s      |
| Mem. Task=0(Gb) |   0.0021972    |    1.171875     |
| --------------- | -------------- | --------------- |
|T(2)             |    0.468s      |    2.558s       |
| Mem. Task>0(Gb) |   0.0010986    |    0.5859       |
| --------------- | -------------- | --------------- |
|T(4)             |    0.553s      |    2.261s       |
| Mem. Task>0(Gb) |   0.0016479    |    0.8789       |
| --------------- | -------------- | --------------- |
|Sp(1)            |  0.043577982   |  4.162380815    |
|Sp(2)            |  0.040598291   |  5.63174355     |
|Sp(4)            |  0.034358047   |  6.371517028    |

Donde 
* T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del `$ time ./Bandera ...` 
* T(p): El tiempo total (parte real de la salida `$ time ./Bandera-MPI ...`) del algoritmo paralelo con p tareas.
* Mem. Task=0(Gb): Memoria consumida por la tarea cero.
* Mem. Task>0(Gb): Memoria consumida por las tareas distintas de la cero. 
* Sp(p): Speed-up real con p tareas paralelas.


5. **¿Es Sp(p)> SpA(p)? ¿Cuales pueden ser los motivos?**

En el caso de Rows=Cols=20480, se observa que el Speed-up real (Sp(p)) es mayor que el Speed-up teórico (SpA(p)), y esto puede deberse a varios factores:

- Reducción del Overhead Relativo:

A medida que el tamaño del problema crece, el overhead relativo asociado con la inicialización y finalización de MPI, así como las operaciones de envío y recepción, se reduce en comparación con el tiempo total de ejecución. En tamaños de problema más pequeños, este overhead puede tener un impacto más significativo en el rendimiento relativo.

- Mayor Balance de Carga:

En tamaños de problema más grandes, la probabilidad de un balance de carga más uniforme entre las tareas aumenta. Las tareas paralelas pueden trabajar de manera más equitativa en la distribución de la carga de trabajo, evitando la inactividad de algunas tareas mientras otras todavía están trabajando.

6. **Explica como has calculado las cantidades de memoria consumidas por la tarea 0 y las otras tareas.**

La memoria consumida por cada tarea se calcula considerando la matriz de colores (ppRed, ppGreen, ppBlue), que tiene un tamaño de Rows x Cols x 3 bytes, ya que cada píxel tiene tres componentes de color (RGB) representados por bytes.

- Memoria de la Tarea 0 (Task 0): La tarea 0 es responsable de almacenar la imagen completa, por lo que la memoria consumida por la Tarea 0 es simplemente el tamaño de la matriz completa, es decir, Rows x Cols x 3 bytes.

- Memoria de Otras Tareas (Tareas > 0): Cada una de las tareas que no es la Tarea 0 solo se encarga de una porción de la imagen, definida por su rango de píxeles asignados. Por lo tanto, la memoria consumida por estas tareas es proporcional al tamaño de la porción que están manejando.

7. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

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
