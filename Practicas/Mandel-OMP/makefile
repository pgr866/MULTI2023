PREFIX  = /home/$(shell whoami)
DEBUG   =  
#DEBUG   =  -g
OMP = 
OMP = -fopenmp
FLAGS   = -Wall  
#FLAGS   = -Wall -O3 -ftree-parallelize-loops=4
INC     = -I.
#RPATH   = -Wl,-R$(PREFIX)/lib 
LIB     = -L/usr/lib64 -lm
CC      = gcc

#------------------------------------------------------------------------------
PROGRAM = Mandel-OMP
default : $(PROGRAM)

#------------------------------------------------------------------------------
$(PROGRAM): $(PROGRAM).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(OMP)  $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o $(PROGRAM) $(PROGRAM).c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(OMP)  $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(OMP)  $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(OMP)  $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM) *.o *.~ *.bck 2>/dev/null || true
