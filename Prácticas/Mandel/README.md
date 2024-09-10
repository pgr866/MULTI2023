# Ejercicio: Mandelbrot serie

## A rellenar por el alumno
 * Nombre y apellidos alumno 1   : Pablo Gómez Rivas
 * Nombre y apellidos alumno 2   : Jesús Fornieles Muñoz
 * Mayoría en GTA1, GTA2 o GTA3  : GTA1
 * Nombre de grupo de actividades: Grupo Actividades 8

# Descripción de la arquitectura utilizada: 
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
* El ejemplo muestra como generar una imagen en tonos de gris del fractal de Mandelbrot.

> Información del fractal de Mandelbrot en la [wiki](https://es.wikipedia.org/wiki/Conjunto_de_Mandelbrot). 
>
> Mandelbrot set [Rosetta code](https://rosettacode.org/wiki/Mandelbrot_set#C)
>
> Mandelbrot set  [Techniques for computer generated pictures](https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index.php/Mandelbrot_set) 

* El fractal de Mandelbrot es en blanco y negro aunque se pueden generar tonos de gris o colores dependiendo del número de iteraciones "NIter" que se realicen sobre cada punto (pixel).
* Se proporciona el código que genera el fractal de Mandelbrot en tonos de gris usando la rutina NIterToGray().
* El alumno debe ampliar el código para que que opcionalmente (sin el parámetro -g) se  genere la imagen en color RGB (ver las líneas comentadas con "**//TODO:**"). Se aporta la rutina NIterToRGB(), que usa una paleta de 16 colores RGB donde mapear el número de iteraciones sobre cada pixel para obtener el color RGB. 
* Los parámetros se introducen por la línea de comandos.
* La memoria se asigna de forma dinámica. 
Aunque se podría escribir directamente en el fichero de salida, se usan:
    - Una matriz Gray para los tonos de gris o
    - Tres matrices (R, G y B) para generar los colores RGB de cada pixel.

**Todas las variables necesarias ya están declaradas.**

## Librerias
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.

## Objetivos
  * Usar lo aprendido en el ejercicio Bandera.
  * Ejemplo secuencial donde el trabajo computacional de cada pixel (NIter) no se sabe de antemano.
  * Es un código secuencial candidato a necesitar balanceo dinámico de la carga en su paralelización.

## Compilación
```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Mandel -h
```

## Ejemplo de compilación y ejecuciones
 * En el script Run.sh
 * Un ejemplo de los posibles valores de los parámetros se encuentra en la Imagen del mes del portal de matemáticas de Enero de 2005.
    - [Pic of the month.](https://en.wikipedia.org/wiki/File:Mandelpart2.jpg)

- - - 
# Entrega:
 * Hay que entregar el fichero modificado. Ver las líneas comentadas con "//TODO:".

1. **¿Has verificado que la imagen se genera correctamente?**

Sí, con el comando ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 1000 -g, y luego eog Image.png para abrir la imagen.

2. **¿Has verificado que no tienes perdida memoria?** 

Sí.

```console
root@Ubuntu:/home/user/Desktop/shared_folder/Mandel# valgrind ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 1000 -g
==13227== Memcheck, a memory error detector
==13227== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==13227== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==13227== Command: ./Mandel -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 1000 -g
==13227== 
Img.Rows=1024, Img.Cols=1024,
Mandel.MinX=-2.000000, Mandel.MinY=-2.000000,
Mandel.SizeX=4.000000, Mandel.SizeY=4.000000,
InGray=1, Max.N.iterations=1000,  Output=Image.
Command=rawtopgm 1024 1024 Image | pnmtopng > Image.png
pnmtopng: 204 colors found
==13227== 
==13227== HEAP SUMMARY:
==13227==     in use at exit: 0 bytes in 0 blocks
==13227==   total heap usage: 1,028 allocs, 1,028 frees, 1,066,456 bytes allocated
==13227== 
==13227== All heap blocks were freed -- no leaks are possible
==13227== 
==13227== For lists of detected and suppressed errors, rerun with: -s
==13227== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

3.  **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

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
### Markdown cheat sheet
Para añadir información a este README.md:
[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -
&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
