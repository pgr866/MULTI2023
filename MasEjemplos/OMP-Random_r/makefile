#See https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html
PREFIX	= /home/$(shell whoami)
DEBUG	=  
#DEBUG	=  -g
FLAGS	= -Wall
#FLAGS   = -Wall -O3 
#OMP = 
OMP = -fopenmp
INC     = -I.
LIB     = -L/usr/lib64 -pthread -lm
CC      = gcc

#------------------------------------------------------------------------------
PROGRAMS = AddVectorInt-OMP_R AddVectorInt-OMP_R_Vector
OBJS     = getmem.o argshand.o utils.o

default : 
	@echo "make <programa>" 

all: $(PROGRAMS)

#------------------------------------------------------------------------------
$(PROGRAMS): %: %.c makefile $(OBJS)
	$(CC) $(DEBUG) $(FLAGS) $(OMP) $(INC) $(OBJS) \
	$(LIB) -o $@ $<

%.o : %.c
	$(CC) $(DEBUG) $(OMP) -o $@ -c $<

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAMS) *.o *.~ *.bck 2>/dev/null || true
