PREFIX  = /home/$(shell whoami)
DEBUG   =  
#DEBUG   =  -g
PROF    =
#PROF    = -pg 
FLAGS   = -Wall
#FLAGS   = -Wall -O3
INC     = -I.
#RPATH   = -Wl,-R$(PREFIX)/lib
LIB     = -L/usr/lib64 -lm
CC      = gcc

#------------------------------------------------------------------------------
PROGRAM = Wa-tor
default : $(PROGRAM)

#------------------------------------------------------------------------------
$(PROGRAM): $(PROGRAM).c makefile argshand.o getmem.o utils.o animals.o \
	ocean.o
	$(CC) $(DEBUG) $(PROF) $(FLAGS) $(INC)  argshand.o getmem.o utils.o animals.o \
	ocean.o $(RPATH) $(LIB) -o $(PROGRAM) $(PROGRAM).c

ocean.o : ocean.c ocean.h 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) -c -o ocean.o ocean.c

animals.o : animals.c animals.h 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) -c -o animals.o animals.c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(PROF) $(FLAGS) -c -o argshand.o argshand.c

#------------------------------------------------------------------------------
clean:
	@rm $(PROGRAM) *.o *.~ *.bck *.ppm *.png *.mp4 *.data 2>/dev/null || true

