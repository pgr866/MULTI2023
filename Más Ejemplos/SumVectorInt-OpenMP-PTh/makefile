PREFIX	= /home/$(shell whoami)
DEBUG	=  
#DEBUG	=  -g
PROF	= 
#PROF	= -pg
FLAGS	= -Wall
#FLAGS   = -Wall -O3 
#OMP = 
OMP = -fopenmp
INC     = -I.
LIB     = -L/usr/lib64 -pthread -lm
CC      = gcc

#------------------------------------------------------------------------------
PROGRAM1 = SumVectorInt
PROGRAM2 = SumVectorInt-OMP
PROGRAM3 = SumVectorInt-PTh


default	: $(PROGRAM1)
all 	: $(PROGRAM1) $(PROGRAM2) $(PROGRAM3) 

#------------------------------------------------------------------------------
$(PROGRAM1): $(PROGRAM1).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM1) $(PROGRAM1).c
	
$(PROGRAM2): $(PROGRAM2).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) $(OMP) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM2) $(PROGRAM2).c

$(PROGRAM3): $(PROGRAM3).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) $(INC)  argshand.o getmem.o utils.o \
	$(LIB) -o $(PROGRAM3) $(PROGRAM3).c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM1) $(PROGRAM2) $(PROGRAM3) *.o *.~ *.bck 2>/dev/null || true
