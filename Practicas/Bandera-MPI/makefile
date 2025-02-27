PREFIX  = /home/$(shell whoami)
DEBUG   =  
#DEBUG   =  -g
#OMP = 
FLAGS   = -Wall 
INC     = -I.
#RPATH   = -Wl,-R$(PREFIX)/lib
LIB     = -L/usr/lib64 -pthread -lm
CC      = gcc
MPICC	= mpicc

#------------------------------------------------------------------------------
PROGRAM1 = Bandera-MPI
PROGRAM2 = Bandera
default : $(PROGRAM1)
all		: $(PROGRAM1) $(PROGRAM2)

#------------------------------------------------------------------------------
$(PROGRAM1): $(PROGRAM1).c makefile argshand.o getmem.o utils.o 
	$(MPICC) $(DEBUG) $(OMP) $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o $(PROGRAM1) $(PROGRAM1).c
	
$(PROGRAM2): $(PROGRAM2).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(OMP) $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o $(PROGRAM2)  $(PROGRAM2).c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(OMP) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(OMP) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(OMP) $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM1) $(PROGRAM2) *.o *.~ *.bck 2>/dev/null || true
