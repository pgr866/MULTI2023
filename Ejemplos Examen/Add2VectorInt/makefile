PREFIX	= /home/$(shell whoami)
DEBUG	=  
#DEBUG	=  -g
FLAGS	= -Wall
#FLAGS   = -Wall -O3 
#OMP = 
INC     = -I.
LIB     = -L/usr/lib64 -pthread -lm
CC      = gcc
MPICC 	= mpicc

#------------------------------------------------------------------------------
PROGRAM1 = Add2VectorInt
PROGRAM2 = Add2VectorInt-PTh
PROGRAM3 = Add2VectorInt-MPI

default	: $(PROGRAM1)
all	: $(PROGRAM1) $(PROGRAM2) $(PROGRAM3)

#------------------------------------------------------------------------------
$(PROGRAM1): $(PROGRAM1).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM1) $(PROGRAM1).c

$(PROGRAM2): $(PROGRAM2).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM2) $(PROGRAM2).c

$(PROGRAM3): $(PROGRAM3).c makefile argshand.o getmem.o utils.o 
	$(MPICC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM3) $(PROGRAM3).c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM1) $(PROGRAM2) $(PROGRAM3)	*.o *.~ *.bck 2>/dev/null || true
	
