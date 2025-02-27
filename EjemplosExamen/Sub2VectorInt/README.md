# Examen OpenMP y PThreads.

## A rellenar por el alumno
  * Nombre y Apellidos: 
  * Grupo de trabajo (GTA1,GTA2 o GTA3):

## Arquitectura: 
  * Microprocesador:
  * Número de núcleos:
  * Cantidad de subprocesos por núcleo:
  * Tiene hyperthreading (SMT) activado en BIOS:
  * RAM:
  * Se usa máquina virtual:
    - Número de cores:
    - RAM: 
    - Capacidad HDD: 

## Instrucciones:
  * Se adjunta la versión serie donde con DEGUG=1 se pueden obtener los tiempos secuenciales de las partes de código serie  que se pueden paralelizar. 
  * El alumno debe realizar las versiones OpenMP y PThread.
  * Hay que aprobar ambas. OMP es un 30% y PTh un 70% de la nota.
  * Los parámetros de los programas se encuentran en el Run.sh, pero el número de iteraciones hay que establecerlo para que el secuencial dure al menos unos segundos.
  * La memoria se asigna de forma dinámica mediante las rutinas en getmem.h.
  * Los parámetros de entrada se obtienen mediante las rutinas en arghand.h.
  * Los vectores y matrices se imprimen mediante las rutinas en utils.h.
  * Se puede paralelizar la inicialización del (los) array(s) de datos aleatorios y la operación sobre el (los) array(s).
  * Es obligatorio paralelizar al menos la operación sobre el (los) array(s). Se valorará más si se paraleliza además la inicialización aleatoria de el (los) array(s).
  * Para la inicialización aleatoria de el (los) array(s) se usará lrand48\_r() (enteros) o drand48\_r() (double) para las versiones paralelas si se paraleliza esa rutina.  La semilla de la secuencia pseudoaleatoria de cada hebra se inicializa al identificador de la hebra. Si no se paraleliza la inicialización aleatoria de el (los) array(s) se usará lrand48() y la semilla de la secuencia pseudoaleatoria será 0.
  * El Run.sh hay que completarlo para la ejecución serie y paralela.
  * Usa un tamaño de iteraciones pequeño para ver que se inicializa bien y se realiza bien la operación.
  * Cuando se miden tiempos del programa completo hay que compilar sin -g ni -pg y las variables PRINT y DEBUG deben ser 0.

# Entrega.
## Speed-up teórico del algoritmo secuencial.

1. **¿Qué número de iteraciones (NIter) se ha elegido para que el programa secuencial dure al menos unos segundos?**

2. **Para el programa secuencial que se proporciona, rellena las siguientes tablas.**
* Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.


 **Si se paraleliza solo la operación** 

| Ejecución   |     NIter      | 
| ----------- | -------------- |
|T.Sec        |                |
|T.Oper.      |                | 
|T.CsPar      |                |
|SpA(2)       |                |
|SpA(4)       |                |

**Si se paraleliza la inicialización de el (los) array(s) y la operación** 

| Ejecución   |     NIter      | 
| ----------- | -------------- |
|T.Sec        |                |
|T.Inic.      |                |
|T.Oper.      |                | 
|T.CsPar      |                |
|SpA(2)       |                |
|SpA(4)       |                |



donde

* T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del `$time < programa > ... `
* T.Inic.: El tiempo de inicializar el (los) array(s).
* T.Oper: El tiempo de realizar la operación.
* T.CsPar: El tiempo de la parte del código secuencial que será paralelizado. 
* SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.


## Resultados del algoritmo paralelo con OpenMP

3. **¿Qué parte(s) has paralelizado de la versión secuencial?**

4. **Rellena la siguiente tabla para la ejecución paralela realizada en OpenMP.
Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.**

| Ejecución   |    NIter       | 
| ----------- | -------------- |
|T(1)         |                |
|T(2)         |                |
|T(4)         |                |
|Sp(1)		  |                |
|Sp(2)		  |                |
|Sp(4)        |                |

Donde 

* T(p): El tiempo total (parte real de la salida `$time < programa > ...)` del algoritmo paralelo con p hebras.
* Sp(p): Speed-up real con p hebras paralelas.


## Resultados del algoritmo paralelo con PThreads

4. **¿Qué parte(s) has paralelizado de la versión secuencial?**

5. **Rellena la siguiente tabla para la ejecución paralela realizada en PThreads.**
* Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.

| Ejecución   |    NIter       | 
| ----------- | -------------- |
|T(1)         |                |
|T(2)         |                |
|T(4)         |                |
|Sp(1)		  |                |
|Sp(2)		  |                |
|Sp(4)        |                |

Donde 

* T(p): El tiempo total (parte real de la salida `$time < programa > ...)` del algoritmo paralelo con p hebras.
* Sp(p): Speed-up real con p hebras paralelas.


- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
