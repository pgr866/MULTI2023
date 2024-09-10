#!/bin/bash

#Librerias necesarias
sudo apt update
sudo apt -y install netpbm eog valgrind linux-tools-common linux-tools-6.2.0-39-generic gnuplot gdb binutils ffmpeg mpich xterm

perf
echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid
perf
