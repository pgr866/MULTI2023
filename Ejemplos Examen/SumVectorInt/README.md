# Ejemplo de examen: Programas OMP y PTh.

## A rellenar por el alumno/grupo
* Nombre y apellidos alumno:
* Grupo de actividades:
* Grupo de trabajo (GTA1,GTA2 o GTA3): 

## Arquitectura: 
  * Microprocesador:
  * Número de núcleos:
  * Cantidad de subprocesos por nucleo:
  * Tiene hyperthreading (SMT) activado en BIOS:
  * HDD/SDD: 
  * RAM:
  * Se usa máquina virtual:
    - Número de cores:
    - RAM: 
    - Capacidad HDD: 


## Instrucciones:
 * Se adjunta la versión serie donde con DEGUG=1 se pueden obtener los tiempos secuenciales de las partes de código serie que se pueden paralelizar y el T.Cspar.
 * El alumno debe realizar las versiones OpenMP y PThread.
 * Los parámetros de los programas se encuentran en el Run.sh.
 * La memoria se asigna de forma dinámica mediante las rutinas en getmem.h.
 * Los parámetros de entrada se obtienen mediante las rutinas en arghand.h.
 * Los vectores y matrices se imprimen mediante las rutinas en utils.h.
 * Se puede paralelizar la inicialización del vector y su suma.
 * Es obligatorio paralelizar al menos la suma. Se valorará más si se paraleliza además la inicialización aleatoria del vector.
 * Si se paraleliza la inicialización aleatótria del vector, se usará lrand48_r() para ello.  La semilla de la secuencia pseudoaleatoria de cada hebra se inicializa al identificador de la hebra. Si no se paraleliza la inicialización aleatoria del vector se usará lrand48() y la semilla de la secuencia pseudoaleatoria será 0.
 * El Run.sh ya está preparado para la ejecución del programa serie y del paralelo.
 * Usa un vector pequeño para ver que se inicializa bien y se realiza bien la suma.
 * Cuando se miden tiempos del programa completo hay que compilar sin -g ni -pg y las variables PRINT y DEBUG deben ser 0.

- - - 

# Entrega

## Speed-up teórico del algoritmo secuencial.

1. **Para el programa secuencial que se proporciona, rellena las siguientes tablas.**
* Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.
* Si con un tamño del vector -r 1e-9 tarda mucho, hay elegir un valor que haga que la versión secuencial tarde al menos varios segundos.
* Ese valor hay que usarlo en todas las tablas.

 **Si se paraleliza solo la suma** 

| Ejecución   | -r 1000000000   | 
| ----------- | --------------- |
|T.Sec        | 4,815           |
|T.suma       | 0,1315          |
|T.CsPar      | 0,1315          |
|SpA(2)       | 1,01384429120387|
|SpA(4)       | 1,02091118708754|

**Si se paraleliza la inicialización del vector y la suma** 

| Ejecución   | -r 1000000000   | 
| ----------- | --------------- |
|T.Sec        | 4,815           |
|T.Inic.      | 4,578           |
|T.Suma       | 0,1315          | 
|T.CsPar      | 4,709           |
|SpA(2)       | 1,95691932534038|
|SpA(4)       | 3,75219170075979|


donde:
* T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del `$ time < programa > ... `
* T.Inic.: El tiempo de inicializar el vector.
* T.Suma:  El tiempo de realizar la suma del vector.
* T.CsPar: El tiempo del código secuencial que será paralelizado. 
* SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.


## Resultados del algoritmo paralelo con OpenMP

2. **¿Qué parte(s) has paralelizado de la versión secuencial?**

3. **Rellena la siguiente tabla para la ejecución paralela realizada en OpenMP.**
* Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.

| Ejecución   | -r 1000000000  | 
| ----------- | -------------- |
|T(1)         |                |
|T(2)         |                |
|T(3)         | 1,366          |
|Sp(1)		  |                |
|Sp(2)		  |                |
|Sp(4)        |                |

Donde 
* T(p): El tiempo total (parte real de la salida `$ time < programa > ...` del algoritmo paralelo con p hebras.
* Sp(p): Speed-up real con p hebras paralelas.


## Resultados del algoritmo paralelo con PThreads

4. **¿Qué parte(s) has paralelizado de la versión secuencial?**

5. **Rellena la siguiente tabla para la ejecución paralela realizada en PThreads.**
* Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.

| Ejecución   | -r 1000000000  | 
| ----------- | -------------- |
|T(1)         |                |
|T(2)         |                |
|T(4)         |                |
|Sp(1)		  |                |
|Sp(2)		  |                |
|Sp(4)        |                |

Donde 
Donde 
* T(p): El tiempo total (parte real de la salida `$ time < programa > ...` del algoritmo paralelo con p hebras.
* Sp(p): Speed-up real con p hebras paralelas.


- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
