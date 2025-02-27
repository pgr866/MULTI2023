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
PROGRAMS = \
	omp_bug1 omp_bug1fix \
	omp_bug2 omp_bug2fix \
	omp_bug3 omp_bug3fix \
	omp_bug4 omp_bug4fix \
	omp_bug5 omp_bug5fix \
	omp_bug6 omp_bug6fix 

all: $(PROGRAMS)

#------------------------------------------------------------------------------
$(PROGRAMS): %: %.c
	$(CC) $(DEBUG) $(FLAGS) $(OMP) $(INC) $(LIB) -o $@ $<


#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAMS)  *.o *.~ *.bck 2>/dev/null || true
