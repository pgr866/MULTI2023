# Examen MPI.

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
  * El alumno debe realizar la versión MPÎ.
  * Los parámetros de los programas se encuentran en el Run.sh.
  * La memoria se asigna de forma dinámica mediante las rutinas en getmem.h.
  * Los parámetros de entrada se obtienen mediante las rutinas en arghand.h.
  * Los vectores y matrices se imprimen mediante las rutinas en utils.h.
  * Es obligatorio paralelizar al menos la operación a realizar en el vector. Se puede paralelizar opcionalmente la inicialización del vector, se obtiene más nota.
  * Se obtiene más nota si se realizan comunicaciones colectivas y si estas son IN\_PLACE.
  * El Run.sh ya está preparado para la ejecución del programa serie y del paralelo.
  * Usa un vector pequeño para ver que se inicializa bien y se realiza bien la operación sobre el vector.
  * Cuando se miden tiempos del programa completo hay que compilar sin -g ni -pg y las variables PRINT y DEBUG deben ser 0.

- - - 

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


## Resultados del algoritmo paralelo con MPI

3. **¿Qué parte(s) has paralelizado de la versión secuencial?**

4. **¿Has usado colectivas?**

5. **¿Has usado IN_PLACE?**

5. **Rellena la siguiente tabla para la ejecución paralela realizada en MPI.**
* Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.
* Se recuerda que no es necesario medir tiempos dentro del programa paralelo.

| Ejecución   |    NIter       | 
| ----------- | -------------- |
|T(1)         |                |
|T(2)         |                |
|T(4)         |                |
|Sp(1)		  |                |
|Sp(2)		  |                |
|Sp(4)        |                |

Donde: 
* T(p): El tiempo total (parte real de la salida `$time < programa > ...)` del algoritmo paralelo con p hebras.
* Sp(p): Speed-up real con p hebras paralelas.

- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
