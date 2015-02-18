objects = primegap.o processinfo.o result.o
executable = primegap
CFLAGS += -Wall -std=c99 -lgmp

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

sanity : build
	mpirun -np 4 ./$(executable) 2 100

.PHONY : clean
clean:
	rm $(objects) $(executable)
