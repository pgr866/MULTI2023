PREFIX  = /home/$(shell whoami)
DEBUG   =  
#DEBUG   =  -g
FLAGS   = -Wall
#FLAGS   = -Wall -O3
INC     = -I.
#RPATH   = -Wl,-R$(PREFIX)/lib
LIB     = -L/usr/lib64 -lm
CC      = gcc

#------------------------------------------------------------------------------
PROGRAM = Mandel
default : $(PROGRAM)

#------------------------------------------------------------------------------
$(PROGRAM): $(PROGRAM).c makefile argshand.o getmem.o utils.o 
	$(CC) $(DEBUG) $(FLAGS) $(INC)  argshand.o getmem.o utils.o\
	$(RPATH) $(LIB) -o $(PROGRAM) $(PROGRAM).c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM) *.o *.~ *.bck 2>/dev/null || true
