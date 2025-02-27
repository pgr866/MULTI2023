PREFIX	= /home/$(shell whoami)
DEBUG	=  
#DEBUG	=  -g
FLAGS	= -Wall
#FLAGS   = -Wall -O3 
#OMP = 
INC     = -I.
LIB     = -L/usr/lib64 -lm
CC      = gcc
MPICC 	= mpicc

#------------------------------------------------------------------------------
PROGRAM1 = Add2VectorInt
PROGRAM2 = Add2VectorInt-MPI
PROGRAM3 = Add2VectorInt-MPI-InPlace
PROGRAM4 = Add2VectorInt-MPI-ParInit
PROGRAM5 = Add2VectorInt-MPI-ParInit-InPlace

default	: $(PROGRAM2)
all		: $(PROGRAM1) $(PROGRAM2) $(PROGRAM3) $(PROGRAM4) $(PROGRAM5)

#------------------------------------------------------------------------------
$(PROGRAM1): $(PROGRAM1).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM1) $(PROGRAM1).c

$(PROGRAM2): $(PROGRAM2).c makefile argshand.o getmem.o utils.o 
	$(MPICC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM2) $(PROGRAM2).c

$(PROGRAM3): $(PROGRAM3).c makefile argshand.o getmem.o utils.o 
	$(MPICC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM3) $(PROGRAM3).c

$(PROGRAM4): $(PROGRAM4).c makefile argshand.o getmem.o utils.o 
	$(MPICC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM4) $(PROGRAM4).c

$(PROGRAM5): $(PROGRAM5).c makefile argshand.o getmem.o utils.o 
	$(MPICC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM5) $(PROGRAM5).c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM1) $(PROGRAM2) $(PROGRAM3) $(PROGRAM4) $(PROGRAM5) \
	*.o *.~ *.bck 2>/dev/null || true
