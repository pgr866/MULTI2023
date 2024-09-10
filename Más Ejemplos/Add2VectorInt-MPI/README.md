# Ejemplo de examen para MPI. 

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
  * Se adjunta la versión serie donde con DEGUG=1 se pueden obtener los tiempos secuenciales de las partes de código serie  que se pueden paralelizar. 
  * El alumno debe realizar la version MPÎ.
  * Los parámetros de los programas se encuentran en el Run.sh.
  * La memoria se asigna de forma dinámica mediante las rutinas en getmem.h.
  * Los parámetros de entrada se obtienen mediante las rutinas en arghand.h.
  * Los vectores y matrices se imprimen mediante las rutinas en utils.h.
  * Es obligatorio paralelizar al menos la suma. Se puede paralelizr opcionalmente la inicialización de los vectores.
  * Se obtine más nota si se realizan comunicaciones colectivas y si estas son IN_PLACE
  * El Run.sh ya está preparado para la ejecución del programa serie y del paralelo.
  * Usa un vector pequeño para ver que se inicializa bien y se realiza bien la suma.
  * Cuando se miden tiempos del programa completo hay que compilar sin -g ni -pg y las variables PRINT y DEBUG deben ser 0.

- - -

# Entrega

## Speed-up teórico del algoritmo secuencial.

1. **Para el programa secuencial que se proporciona, rellena las siguientes tablas.** 
* Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.


 **Si se paraleliza solo la suma** 

| Ejecución   | -r 500000000  | 
| ----------- | -------------- |
|T.Sec        |                |
|T.Suma       |                | 
|T.CsPar      |                |
|SpA(2)       |                |
|SpA(4)       |                |

**Si se paraleliza la inicialización del vector y la suma** 

| Ejecución   | -r 500000000  | 
| ----------- | -------------- |
|T.Sec        |                |
|T.Inic.      |                |
|T.Suma       |                | 
|T.CsPar      |                |
|SpA(2)       |                |
|SpA(4)       |                |



donde
* T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del `$time < programa > ... `
* T.Inic.: El tiempo de inicializar el vector.
* T.Suma:  El tiempo de realizar la suma del vector.
* T.CsPar: El tiempo de la parte del código secuencial que será paralelizadA. 
* SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.


## Resultados del algoritmo paralelo con MPI

2. **¿Qué parte(s) has paralelizado de la versión secuencial?**

3. **¿Has usado colectivas?**

4. **¿Has usado IN_PLACE?**

5. **Rellena la siguiente tabla para la ejecución paralela realizada.**
* Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.

| Ejecución   | -r 500000000  | 
| ----------- | -------------- |
|T(1)         |                |
|T(2)         |                |
|T(4)         |                |
|Sp(1)		  |                |
|Sp(2)		  |                |
|Sp(4)        |                |

Donde 
*T(p): El tiempo total (parte real de la salida `$time < programa > ...`) del algoritmo paralelo con p tareas.
* Sp(p): Speed-up real con p tareas paralelas.


- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
