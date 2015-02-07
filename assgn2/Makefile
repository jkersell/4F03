objects = primegap.o processinfo.o
executable = primegap
CFLAGS += -Wall -std=c99

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

sanity : build
	mpirun -np 4 ./$(executable) 2 100

.PHONY : clean
clean:
	rm $(objects) $(executable)
