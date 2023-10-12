Proyecto de Reducción de Calidad de Video

Descripción

Este proyecto consiste en un programa en C++ que reduce la calidad de un video y un script de Python que realiza un benchmark para medir el tiempo de respuesta y el speedup en función del número de hilos utilizados para la paralelización.

Requisitos Previos

OpenCV
Compilador C++
Python 3
Bibliotecas de Python (matplotlib, numpy)
Instrucciones de Uso

Ejecutar el C++:

g++ -o bin main.cpp -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -std=c++11 -fopenmp

Luego, ejecuta el programa C++ con los siguientes argumentos:


./bin video.mp4 video_salida.mp4 num_hilos

video.mp4: El nombre del video de entrada.
video_salida.mp4: El nombre del video de salida con calidad reducida.
num_hilos: El número de hilos a utilizar para la paralelización.


Ejecutar el Python:

python benchmark.py
