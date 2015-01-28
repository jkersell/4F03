objects = main.o genmatvec.o
executable = matvec
CFLAGS+=-Wall -std=gnu99

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

test : build
	mpirun -np 4 ./$(executable)

.PHONY : clean
clean :
	-rm matvec $(objects)
	
