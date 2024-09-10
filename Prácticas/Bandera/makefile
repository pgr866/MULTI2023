PREFIX  = /home/$(shell whoami)
DEBUG   =  
#DEBUG   =  -pg
FLAGS   = -Wall
#FLAGS   = -Wall -O3
INC     = -I.
#RPATH   = -Wl,-R$(PREFIX)/lib
LIB     = -L/usr/lib64 -lm
CC      = gcc

#------------------------------------------------------------------------------

default : Bandera

all : Bandera Bandera2

#------------------------------------------------------------------------------
Bandera: Bandera.c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o Bandera Bandera.c

Bandera2: Bandera2.c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o Bandera2 Bandera2.c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm Bandera Bandera2 *.o *.~ *.bck 2>/dev/null || true
