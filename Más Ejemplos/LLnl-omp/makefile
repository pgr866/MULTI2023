#See https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

PREFIX = /home/$(shell whoami)
DEBUG =
#DEBUG = -g
FLAGS = -Wall
#FLAGS= -Wall -O3 
#OMP =
OMP = -fopenmp
INC = -I.
LIB = -L/usr/lib64 -lm
CC = gcc

#------------------------------------------------------------------------------
PROGRAMS = 01-omp_hello 02-omp_workshare1 03-omp_mm 04-omp_workshare2 \
	05-omp_critical 06-omp_thpriv 07-omp_reduction 07-omp_reduction2 \
	08-omp_orphan 09-omp_getEnvInfo 10-ser_pi_calc

all: $(PROGRAMS)

#------------------------------------------------------------------------------
$(PROGRAMS): %: %.c
	$(CC) $(DEBUG) $(FLAGS) $(OMP) $(INC) $(LIB) -o $@ $<


#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAMS)  *.o *.~ *.bck 2>/dev/null || true
