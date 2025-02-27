PREFIX  = /home/$(shell whoami)
DEBUG   =  
#DEBUG   =  -g
FLAGS   = -Wall 
#FLAGS   = -Wall -O3 -ftree-parallelize-loops=4
INC     = -I.
#RPATH   = -Wl,-R$(PREFIX)/lib 
LIB     = -L/usr/lib64 -lm
CC      = gcc
MPICC	= mpicc
#------------------------------------------------------------------------------
PROGRAM1 = Mandel-MPI
PROGRAM2 = Mandel
default : $(PROGRAM1)
all		: $(PROGRAM1) $(PROGRAM2)

#------------------------------------------------------------------------------
$(PROGRAM1): $(PROGRAM1).c makefile argshand.o getmem.o utils.o 
	$(MPICC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o $(PROGRAM1) $(PROGRAM1).c
	
$(PROGRAM2): $(PROGRAM2).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG)  $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o $(PROGRAM2) $(PROGRAM2).c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM1) $(PROGRAM2) *.o *.~ *.bck 2>/dev/null || true
